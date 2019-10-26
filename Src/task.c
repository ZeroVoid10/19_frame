#include "kick.h"
#include "cmd.h"
#include "utils.h"

static void sl_error_handle(void);

uint8_t task_init(void) {
    uint8_t state = SL_OK;
    // TODO: ZeroVoid	due:10/25	init failed reinit
    state |= kick_ctrl_init(&kick_ctrl);

    if (state != SL_OK) {
        uprintf("init failed.\r\n");
        sl_error_handle();
        return SL_ERROR;
    }
    return SL_OK;
}

void sl_error_handle(void) {
    // TODO: ZeroVoid	due:10/26	实现error handle功能
    while(1) {

    }
}