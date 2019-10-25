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

/*******************************************************************************
 * Control Define
 *******************************************************************************/
/* Magnet Motro Control Define -----------------------------------------------------*/
#define MAG_MTR_SID                     0x15                // 驱动电磁铁电机CAN标准id
#define SPR_MTR_EID                     33                  // 压缩弹簧电机CAN扩张id
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
    KICK_READY = 0,
    KICK_MAG_CONNECTED,
    KICK_SPRING_READY,
    KICKING
} KICK_MS; // Kick Module State

typedef enum mag_mtr_state {
    MAG_MTR_READY,
    MAG_MTR_UP,
    MAG_MTR_FREE
} MAG_MTR_S;

typedef enum kick_test_state {
    T_KICK_IDLE,
    T_KICK_READY,
    T_KICK_MAG_CON,
    T_KICK_MAG_MTR_UP
} KICK_TMS;

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
    MMTR_S_DEP_POS
} MAG_MTR_MODE;

/*******************************************************************************
 * Variables
 *******************************************************************************/
extern MAGNET_STATE magnet_state;

int kick_init(void);
/* Kick Control -----------------------------------------------------*/
void kick(void);
void kick_test(void);
void kick_spring_free(void);

/* Motor Control -----------------------------------------------------*/
void mag_mtr_up(void);                      // 最终使用函数,测试决定控制实现方式
void mag_mtr_up_test(uint8_t mode, int arg);         // 测试实现函数
void mag_mtr_stop(void);

void spr_mtr_back(uint32_t time);

/* Magnet Control -----------------------------------------------------*/
MAGNET_STATE magnet_toggle(void);
MAGNET_STATE magnet_set(void);
MAGNET_STATE magnet_free(void);



#ifdef __cplusplus
}
#endif

#endif /* __KICK_H */
