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
#include "mtr_driver.h"

#include "main.h"

/* Variables -----------------------------------------------------*/
KICK_MS kick_ms = KICK_READY;  // 初始就是就绪,有待商榷
MAGNET_STATE magnet_state = NO_MAGNET;
uint32_t mag_mtr_up_time = 5000;    // TODO: ZeroVoid	due:10/26	write to flash
uint32_t mag_delay_time = 50;
int32_t  kicking_mag_pos = -100;    // 释放弹簧后电磁的复位版位置
int32_t  spr_mtr_back_duty = 10;
int32_t  spr_mtr_free_duty = -5;

/* Kick Part Init -----------------------------------------------------*/
int kick_init(void) {
    magnet_state = HAL_GPIO_ReadPin(MAGNET_PORT, MAGNET_PIN_1);
    if (magnet_state != NO_MAGNET) {
        uprintf("Pin State Error\r\n");
        return SL_ERROR;
    }
    return SL_OK;
}

/*******************************************************************************
 * Kick
 *******************************************************************************/
void kick(void) {
    switch (kick_ms)
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


void kick_test(void) {
    switch (kick_ms)
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

void kick_spring_free(void) {
    magnet_free();
    vesc_set_duty(SPR_MTR_EID, spr_mtr_free_duty);
    md_set_position(MAG_MTR_SID, kicking_mag_pos);
    HAL_Delay(50);
    vesc_set_duty(SPR_MTR_EID, 0);
}

/*******************************************************************************
 * 带电磁铁运动电机控制
 *******************************************************************************/
/**
 * @brief	电磁铁电机上升控制函数
 * @note    最后调用函数,测试使用mag_mtr_up_test function
 */
void mag_mtr_up(void) {

}

/**
 * @brief	电磁铁电机上升测试
 * @param   mode:   选择上升控制方式
 * @note
 */
void mag_mtr_up_test(uint8_t mode, int arg) {
    switch (mode) {
    case MMTR_DUTY:
        md_set_duty(MAG_MTR_SID, arg);
        vesc_set_duty(SPR_MTR_EID, spr_mtr_free_duty);
        HAL_Delay(mag_mtr_up_time);
        vesc_set_duty(SPR_MTR_EID, 0);
        magnet_set();
        HAL_Delay(mag_delay_time);
        md_set_duty(MAG_MTR_SID, 0);
        break;
    case MMTR_SPEED:
        md_set_duty(MAG_MTR_SID, arg);
        vesc_set_duty(SPR_MTR_EID, spr_mtr_free_duty);
        md_set_speed(MAG_MTR_SID, arg);
        HAL_Delay(mag_mtr_up_time);
        vesc_set_duty(SPR_MTR_EID, 0);
        magnet_set();
        HAL_Delay(mag_delay_time);

    case MMTR_POS:
        vesc_set_duty(SPR_MTR_EID, spr_mtr_free_duty);
        md_set_position(MAG_MTR_SID, arg);
        HAL_Delay(mag_mtr_up_time);
        vesc_set_duty(SPR_MTR_EID, 0);
        magnet_set();
        HAL_Delay(mag_delay_time);
        md_set_duty(MAG_MTR_SID, 0);
        break;
    
    default:
        break;
    }
}

/**
 * @brief	拉伸弹簧
 * @param   time:   电机运行时间(ms)
 */
void spr_mtr_back(uint32_t time) {
    vesc_set_duty(SPR_MTR_EID, spr_mtr_back_duty);
    HAL_Delay(time);
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