#include "button.h"

#include "sys_dbg.h"

#include "app.h"
#include "app_bsp.h"
#include "app_dbg.h"
#include "app_if.h"

#include "task_list.h"

button_t btn_mode;
button_t btn_up;
button_t btn_down;

static zw_game_input_fn_t s_game_input_fn = (zw_game_input_fn_t)0;

void zw_game_register_input(zw_game_input_fn_t fn) {
	s_game_input_fn = fn;
}

void zw_game_unregister_input(void) {
	s_game_input_fn = (zw_game_input_fn_t)0;
}

void zw_game_dispatch_input(uint8_t sig) {
	if (s_game_input_fn) s_game_input_fn(sig);
	else task_post_pure_msg(ZW_GAME_SCREEN_ID, sig);
}

void btn_mode_callback(void* b) {
	button_t* me_b = (button_t*)b;
	switch (me_b->state) {
	case BUTTON_SW_STATE_PRESSED: {
		APP_DBG("[btn_mode_callback] BUTTON_SW_STATE_PRESSED\n");
	}
		break;

	case BUTTON_SW_STATE_RELEASED: {
		APP_DBG("[btn_mode_callback] BUTTON_SW_STATE_RELEASED\n");
		zw_game_dispatch_input(ZW_GAME_BTN_MODE_RELEASED);
	}
		break;

	default:
		break;
	}
}

void btn_up_callback(void* b) {
	button_t* me_b = (button_t*)b;
	switch (me_b->state) {
	case BUTTON_SW_STATE_PRESSED: {
		APP_DBG("[btn_up_callback] BUTTON_SW_STATE_PRESSED\n");
		zw_game_dispatch_input(ZW_GAME_BTN_UP_PRESSED);
	}
		break;
	case BUTTON_SW_STATE_RELEASED: {
		APP_DBG("[btn_up_callback] BUTTON_SW_STATE_RELEASED\n");
		zw_game_dispatch_input(ZW_GAME_BTN_UP_RELEASED);
	}
		break;
	default:
		break;
	}
}

void btn_down_callback(void* b) {
	button_t* me_b = (button_t*)b;
	switch (me_b->state) {
	case BUTTON_SW_STATE_PRESSED: {
		APP_DBG("[btn_down_callback] BUTTON_SW_STATE_PRESSED\n");
		zw_game_dispatch_input(ZW_GAME_BTN_DOWN_PRESSED);
	}
		break;
	case BUTTON_SW_STATE_RELEASED: {
		APP_DBG("[btn_down_callback] BUTTON_SW_STATE_RELEASED\n");
		zw_game_dispatch_input(ZW_GAME_BTN_DOWN_RELEASED);
	}
		break;
	default:
		break;
	}
}
