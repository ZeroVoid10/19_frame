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

MAGNET_STATE magnet_state = NO_MAGNET;


int kick_init(void) {
    magnet_state = HAL_GPIO_ReadPin(MAGNET_PORT, MAGNET_PIN_1);
    if (magnet_state != NO_MAGNET) {
        uprintf("Pin State Error\r\n");
        return SL_ERROR;
    }
    return SL_OK;
}

MAGNET_STATE magnet_toggle(void) {
    uprintf("magnet toggle\r\n");
    HAL_GPIO_TogglePin(MAGNET_PORT, MAGNET_PIN_1);
    HAL_GPIO_TogglePin(MAGNET_PORT, MAGNET_PIN_2);
    return HAL_GPIO_ReadPin(MAGNET_PORT, MAGNET_PIN_1); 
}

MAGNET_STATE magnet_set(void) {
    uprintf("magnet set\r\n");
    HAL_GPIO_WritePin(MAGNET_PORT, MAGNET_PIN_1, MAGNET_SET);
    HAL_GPIO_WritePin(MAGNET_PORT, MAGNET_PIN_2, MAGNET_SET);
    return HAL_GPIO_ReadPin(MAGNET_PORT, MAGNET_PIN_1);
}

MAGNET_STATE magnet_free(void) {
    uprintf("magnet free\r\n");
    HAL_GPIO_WritePin(MAGNET_PORT, MAGNET_PIN_1, MAGNET_FREE);
    HAL_GPIO_WritePin(MAGNET_PORT, MAGNET_PIN_2, MAGNET_FREE);
    return HAL_GPIO_ReadPin(MAGNET_PORT, MAGNET_PIN_2);
}