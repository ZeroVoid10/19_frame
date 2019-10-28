/*******************************************************************************
 * Copyright:		BUPT
 * File Name:		kick.h
 * Description:		踢球控制
 * Author:			ZeroVoid
 * Version:			0.1
 * Data:			2019/10/20 Sun 09:35
 * Encoding:		UTF-8
 *******************************************************************************/
#ifndef __KICK_H
#define __KICK_H

#ifdef __cplusplus
extern "C" {
#endif
#include "gpio.h"
#include "mtr_driver.h"

/*******************************************************************************
 * Control Define
 *******************************************************************************/
/* Magnet Motro Control Define -----------------------------------------------------*/
#define MAG_MTR_SID                     66                // 驱动电磁铁电机CAN标准id
#define SPR_MTR_EID                     33                  // 压缩弹簧电机CAN扩张id
#define KICK_IDLE_POS                   0
#define MAG_MTR_UP_SPEED_POS			1
#define MAG_MTR_UP_POSITION_POS         2
#define SPR_MTR_BACK_POS                3
#define KICK_SPR_FREE_POS               4
#define KICK_IDLE_MSK                   (1<<KICK_IDLE_POS)
#define MAG_MTR_UP_SPEED_MSK            (1<<MAG_MTR_UP_SPEED_POS)
#define MAG_MTR_UP_POSITION_MSK         (1<<MAG_MTR_UP_POSITION_POS)
#define SPR_MTR_BACK_MSK                (1<<SPR_MTR_BACK_POS)
#define KICK_SPR_FREE_MSK               (1<<KICK_SPR_FREE_POS)
/* Magnet/Relay Control Define -----------------------------------------------------*/
#define MAGNET_PORT					    GPIOE // 
#define MAGNET_PIN_1					GPIO_PIN_7 // 
#define MAGNET_PIN_2					GPIO_PIN_8 // 
#define MAGNET_SET					    GPIO_PIN_SET // 
#define MAGNET_FREE                     GPIO_PIN_RESET

/*******************************************************************************
 * Type Define
 *******************************************************************************/
/**
 * @brief	踢球装置状态
 * @note	note
 */
typedef enum kick_state {
    KICK_IDLE = 0,
    KICK_READY,
    KICK_MAG_CONNECTED,
    KICK_SPRING_READY,
    KICKING
} KICK_MS; // Kick Module State

typedef enum mag_mtr_state {
    MAG_MTR_READY,
    MAG_MTR_UPPING,
    MAG_MTR_BACKING,
    MAG_MTR_FREE
} MAG_MTR_S;

/**
 * @brief	电磁铁状态
 * @note	0 无磁力; 1 带磁力
 */
typedef enum {
    NO_MAGNET = GPIO_PIN_RESET,
    MAGNET
} MAGNET_STATE;

typedef enum mag_mtr_mode {
    MMTR_DUTY,
    MMTR_SPEED,
    MMTR_POS,
    MMTR_LIN_SPEED
} MAG_MTR_MODE;

struct kick_controller {
    // TODO: ZeroVoid	due:10/28	use driver id in struct
    // driver id
    uint8_t             mag_mtr_id;
    uint8_t             spr_mtr_id;
    // state param
    KICK_MS             state;
    struct md_state     mag_mtr_can_state;
    MAG_MTR_S           mag_mtr_state;
    MAGNET_STATE        magnet_state;    // TODO: ZeroVoid	due:10/28	merge outside varablies
    // control param
    MAG_MTR_MODE        mag_mtr_mode;
    int32_t             mag_mtr_up_arg;  // speed or position depend on mag_mtr_mode
    int32_t             mag_mtr_up_pos;
    int32_t             mag_mtr_kick_pos; // 释放弹簧后 电磁铁电机位置
    int32_t             spr_mtr_back_duty;
    int32_t             spr_mtr_free_duty;
    // time param
    uint32_t            mag_mtr_up_time;
    uint32_t            mag_delay_time;
    uint32_t            spr_mtr_back_time;
    uint32_t            spr_trans_time;     // 释放电磁铁后弹簧到达稳定时间
};

/*******************************************************************************
 * Variables
 *******************************************************************************/
extern struct kick_controller kick_ctrl;

int kick_ctrl_init(struct kick_controller *ctrl);
/* Kick Control -----------------------------------------------------*/
void kick(struct kick_controller *ctrl);
void kick_test(struct kick_controller *ctrl);
void kick_test_reset(struct kick_controller *ctrl);
void kick_spring_free(struct kick_controller *ctrl);

/* Motor Control -----------------------------------------------------*/
void mag_mtr_up(void);                      // 最终使用函数,测试决定控制实现方式
void mag_mtr_up_test(struct kick_controller *ctrl);         // 测试实现函数
void mag_mtr_stop(void);

void spr_mtr_back(struct kick_controller *ctrl);

/* Magnet Control -----------------------------------------------------*/
MAGNET_STATE magnet_toggle(void);
MAGNET_STATE magnet_set(void);
MAGNET_STATE magnet_free(void);

float set_spr_mtr_duty(struct kick_controller *ctrl);

#ifdef __cplusplus
}
#endif

#endif /* __KICK_H */
