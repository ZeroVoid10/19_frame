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

#define MAGNET_PORT					    GPIOE // 
#define MAGNET_PIN_1					GPIO_PIN_7 // 
#define MAGNET_PIN_2					GPIO_PIN_8 // 
#define MAGNET_SET					    GPIO_PIN_SET // 
#define MAGNET_FREE                     GPIO_PIN_RESET

/**
 * @brief	电磁铁状态
 * @note	0 无磁力; 1 带磁力
 */
typedef enum {
    NO_MAGNET = GPIO_PIN_RESET,
    MAGNET
} MAGNET_STATE;

extern MAGNET_STATE magnet_state;

int kick_init(void);
MAGNET_STATE magnet_toggle(void);
MAGNET_STATE magnet_set(void);
MAGNET_STATE magnet_free(void);



#ifdef __cplusplus
}
#endif

#endif /* __KICK_H */
