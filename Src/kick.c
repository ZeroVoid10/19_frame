/*******************************************************************************
 * Copyright:		BUPT
 * File Name:		kick.c
 * Description:		踢球控制
 * Author:			ZeroVoid
 * Version:			0.1
 * Data:			2019/10/20 Sun 09:44
 *******************************************************************************/
#include "kick.h"
#include "utils.h"
#include "cmd.h"
#include "flags.h"

#include "main.h"

/* Variables -----------------------------------------------------*/
struct kick_controller kick_ctrl = {
    // init state
    .state = KICK_IDLE,
    .mag_mtr_can_state = {0},
    .mag_mtr_state = MAG_MTR_READY,
    .magnet_state = NO_MAGNET,
    // init control param
    .mag_mtr_mode = MMTR_POS,
    .mag_mtr_up_arg = 72,
    .mag_mtr_kick_pos = 0,
    .spr_mtr_back_duty = 60,
    .spr_mtr_free_duty = -50,
    // init time param
    .mag_mtr_up_time = 3700,
    .mag_delay_time = 100,
    .spr_mtr_back_time = 1000
};

/* Kick Part Init -----------------------------------------------------*/
int kick_ctrl_init(struct kick_controller *ctrl) {
    ctrl->magnet_state = HAL_GPIO_ReadPin(MAGNET_PORT, MAGNET_PIN_1);
    if (ctrl->magnet_state != NO_MAGNET) {
        uprintf("Pin State Error\r\n");
        return SL_ERROR;
    }
    return SL_OK;
}

/*******************************************************************************
 * Kick
 *******************************************************************************/
void kick(struct kick_controller *ctrl) {
    switch (ctrl->state)
    {
    case KICK_READY:
        break;

    case KICK_MAG_CONNECTED:
        break;
    
    case KICK_SPRING_READY:
        break;
    
    case KICKING:
        break;
    
    default:
        // TODO: ZeroVoid	due:10/30	ERROR handle part
        break;
    }
}


void kick_test(struct kick_controller *ctrl) {
    switch (ctrl->state)
    {
    case KICK_READY:
        if (kick_test_flag & (MAG_MTR_UP_SPEED_MSK|MAG_MTR_UP_POSITION_MSK)) {
            mag_mtr_up_test(ctrl);
            ctrl->state = KICK_MAG_CONNECTED;
        }
        break;

    case KICK_MAG_CONNECTED:
        if (kick_test_flag & SPR_MTR_BACK_MSK) {
            spr_mtr_back(ctrl);
            ctrl->state = KICK_SPRING_READY;
        }
        break;
    
    case KICK_SPRING_READY:
        if (kick_test_flag & KICK_SPR_FREE_MSK) {
            kick_spring_free(ctrl);

        }
        break;
    
    case KICKING:
        break;
    
    default:
        // TODO: ZeroVoid	due:10/30	ERROR handle part
        break;
    }
}

void kick_test_reset(struct kick_controller *ctrl) {
    kick_test_flag = 0;
    ctrl->magnet_state = MAG_MTR_READY;
    ctrl->state = KICK_IDLE;
    magnet_free();
}

void kick_spring_free(struct kick_controller *ctrl) {
    switch (ctrl->mag_mtr_state) {
    case MAG_MTR_FREE:
        magnet_free();
        vesc_set_duty(SPR_MTR_EID, ctrl->spr_mtr_free_duty);
        md_set_position(MAG_MTR_SID, ctrl->mag_mtr_kick_pos);
        ctrl->mag_mtr_state = MAG_MTR_BACKING;
        break;
    
    case MAG_MTR_BACKING:
        if (ctrl->mag_mtr_can_state.position >= ctrl->mag_mtr_kick_pos) {
            md_set_duty(MAG_MTR_SID, 0);
            vesc_set_duty(SPR_MTR_EID, 0);
            ctrl->mag_mtr_state = MAG_MTR_READY;
        }
        break;

    default:
        break;
    }

}

/*******************************************************************************
 * 带电磁铁运动电机控制
 *******************************************************************************/
/**
 * @brief	电磁铁电机上升控制函数
 * @note    最后调用函数,测试使用mag_mtr_up_test function
 */
void mag_mtr_up(void) {}

/**
 * @brief	电磁铁电机上升测试
 * @param   mode:   选择上升控制方式
 * @note
 */
void mag_mtr_up_test(struct kick_controller *ctrl) {
    switch (ctrl->mag_mtr_mode) {
    // @deprecate DUTY Control. Not be tested
    case MMTR_DUTY:
        md_set_duty(MAG_MTR_SID, ctrl->mag_mtr_up_arg);
        vesc_set_duty(SPR_MTR_EID, ctrl->spr_mtr_free_duty);
        HAL_Delay(ctrl->mag_mtr_up_time);
        vesc_set_duty(SPR_MTR_EID, 0);
        magnet_set();
        HAL_Delay(ctrl->mag_mtr_up_time);
        md_set_duty(MAG_MTR_SID, 0);
        break;
    case MMTR_SPEED:
        // test find for md
        /*
        md_set_speed(MAG_MTR_SID, ctrl->mag_mtr_up_arg);
        vesc_set_duty(SPR_MTR_EID, ctrl->spr_mtr_free_duty);
        HAL_Delay(ctrl->mag_mtr_up_time);
        vesc_set_duty(SPR_MTR_EID, 0);
        magnet_set();
        HAL_Delay(ctrl->mag_delay_time);
        md_set_duty(MAG_MTR_SID, 0);
        ctrl->mag_mtr_state = MAG_MTR_UPPING;
        */
        break;

    case MMTR_POS:
        // Proper range -20 70
        switch (ctrl->mag_mtr_state) {
        case MAG_MTR_READY:
            vesc_set_duty(SPR_MTR_EID, ctrl->spr_mtr_free_duty);
            md_set_position(MAG_MTR_SID, ctrl->mag_mtr_up_arg);
            ctrl->mag_mtr_state = MAG_MTR_UPPING;
            break;
        
        case MAG_MTR_UPPING:
            if (ctrl->mag_mtr_can_state.position >= ctrl->mag_mtr_up_arg - 2) {
                vesc_set_duty(SPR_MTR_EID, 0);
                magnet_set();
                md_set_duty(MAG_MTR_SID, 0);
                HAL_Delay(ctrl->mag_delay_time);
                ctrl->magnet_state = MAG_MTR_FREE;
            }
            break;
        
        default:
            break;
        }
        //HAL_Delay(ctrl->mag_mtr_up_pos_time);
        break;
    
    default:
        break;
    }
}

/**
 * @brief	拉伸弹簧
 * @param   time:   电机运行时间(ms)
 */
void spr_mtr_back(struct kick_controller *ctrl) {
    vesc_set_duty(SPR_MTR_EID, ctrl->spr_mtr_back_duty);
    HAL_Delay(ctrl->spr_mtr_back_time);
    vesc_set_duty(SPR_MTR_EID, 0);
}

void mag_mtr_stop(void) {
    md_set_duty(MAG_MTR_SID, 0);
}

/*******************************************************************************
 * Relay Control
 *******************************************************************************/
MAGNET_STATE magnet_toggle(void) {
    uprintf("magnet toggle\r\n");
    HAL_GPIO_TogglePin(MAGNET_PORT, MAGNET_PIN_1 | MAGNET_PIN_2);
    return HAL_GPIO_ReadPin(MAGNET_PORT, MAGNET_PIN_1); 
}

MAGNET_STATE magnet_set(void) {
    uprintf("magnet set\r\n");
    HAL_GPIO_WritePin(MAGNET_PORT, MAGNET_PIN_1 | MAGNET_PIN_2, MAGNET_SET);
    return HAL_GPIO_ReadPin(MAGNET_PORT, MAGNET_PIN_1);
}

MAGNET_STATE magnet_free(void) {
    uprintf("magnet free\r\n");
    HAL_GPIO_WritePin(MAGNET_PORT, MAGNET_PIN_1 | MAGNET_PIN_2, MAGNET_FREE);
    return HAL_GPIO_ReadPin(MAGNET_PORT, MAGNET_PIN_2);
}