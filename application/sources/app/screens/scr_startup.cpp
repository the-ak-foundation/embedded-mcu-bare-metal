#include "scr_startup.h"

/*****************************************************************************/
/* View - startup */
/*****************************************************************************/
static void view_scr_startup();

view_dynamic_t dyn_view_startup = {
	{
		.item_type = ITEM_TYPE_DYNAMIC,
	},
	view_scr_startup
};

view_screen_t scr_startup = {
	&dyn_view_startup,
	ITEM_NULL,
	ITEM_NULL,

	.focus_item = 0,
};

void view_scr_startup() {
#define AK_LOGO_AXIS_X		(23)
#define AK_LOGO_TEXT		(AK_LOGO_AXIS_X + 4)
	/* ak logo */
	BUZZER_PlayTones(tones_startup);
	view_render.clear();
	view_render.setTextSize(1);
	view_render.setTextColor(WHITE);
	view_render.setCursor(AK_LOGO_AXIS_X, 3);
	view_render.print("   __    _  _ ");
	view_render.setCursor(AK_LOGO_AXIS_X, 10);
	view_render.print("  /__\\  ( )/ )");
	view_render.setCursor(AK_LOGO_AXIS_X, 20);
	view_render.print(" /(__)\\ (   (");
	view_render.setCursor(AK_LOGO_AXIS_X, 30);
	view_render.print("(__)(__)(_)\\_)");
	view_render.setCursor(AK_LOGO_TEXT, 42);
	view_render.print("Active Kernel");
	view_render.update();
}

/*****************************************************************************/
/* Handle - startup */
/*****************************************************************************/
void scr_startup_handle(ak_msg_t* msg) {
	switch (msg->sig) {
	case AC_DISPLAY_INITIAL: {
		APP_DBG_SIG("AC_DISPLAY_INITIAL\n");
		view_render.initialize();
		view_render_display_on();
		timer_set(	AC_TASK_DISPLAY_ID, \
					AC_DISPLAY_SHOW_LOGO, \
					AC_DISPLAY_STARTUP_INTERVAL, \
					TIMER_ONE_SHOT);

		/* restore default settings on every boot/reset */
		settingdata.silent           = ZW_GAME_SETTING_DEFAULT_SILENT;
		settingdata.num_car          = ZW_GAME_SETTING_DEFAULT_NUM_CAR;
		settingdata.bullet_speed     = ZW_GAME_SETTING_DEFAULT_BULLET_SPEED;
		settingdata.zombie_speed     = ZW_GAME_SETTING_DEFAULT_ZOMBIE_SPEED;
		settingdata.tombstone_lane_1 = ZW_GAME_SETTING_DEFAULT_TOMBSTONE_L1;
		settingdata.tombstone_lane_2 = ZW_GAME_SETTING_DEFAULT_TOMBSTONE_L2;
		eeprom_write(	EEPROM_SETTING_START_ADDR, \
						(uint8_t*)&settingdata, \
						sizeof(settingdata));

		/* reset leaderboard to 0 on every boot */
		zw_game_score_t def_score;
		def_score.score_now = 0;
		def_score.score_1st = 0;
		def_score.score_2nd = 0;
		def_score.score_3rd = 0;
		eeprom_write(	EEPROM_SCORE_START_ADDR, \
						(uint8_t*)&def_score, \
						sizeof(def_score));

		BUZZER_Sleep(settingdata.silent);
	}
		break;

	case AC_DISPLAY_BUTTON_MODE_RELEASED: {
		APP_DBG_SIG("AC_DISPLAY_BUTTON_MODE_RELEASED\n");
		SCREEN_TRAN(scr_menu_game_handle, &scr_menu_game);
	}
		break;

	case AC_DISPLAY_SHOW_LOGO: {
		APP_DBG_SIG("AC_DISPLAY_SHOW_LOGO\n");
		SCREEN_TRAN(scr_menu_game_handle, &scr_menu_game);
	}
		break;

	default:
		break;
	}
}
