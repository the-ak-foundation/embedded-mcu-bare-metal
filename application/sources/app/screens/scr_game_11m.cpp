#include "scr_game_11m.h"

/*****************************************************************************/
/* Variable Declaration - Game 11m */
/*****************************************************************************/

static uint8_t 11m_game_state;

void 11m_game_frame_display()
{
	view_render.setTextSize(1);
	view_render.setTextColor(WHITE);
	view_render.setCursor(8, 55);
	view_render.drawLine(0, LCD_HEIGHT - 11, LCD_WIDTH, LCD_HEIGHT - 11, WHITE);
	view_render.drawRect(0, 0, 128, 64, 1);
}

/*****************************************************************************/
/* View - Game 11m */
/*****************************************************************************/

static void view_scr_game_11m();

view_dynamic_t dyn_view_zomwar = {
    {
        .item_type = ITEM_TYPE_DYNAMIC,
    },
    view_scr_game_11m};

view_screen_t scr_game_11m = {
    &dyn_view_11m,
    ITEM_NULL,
    ITEM_NULL,

    .focus_item = 0,
};

void view_scr_game_11m()
{
	if (11m_game_state == GAME_PLAY)
	{
	}
	else if (11m_game_state == GAME_OVER)
	{
		view_render.clear();
	}
}

/*****************************************************************************/
/* Handle - Game 11m */
/*****************************************************************************/

void scr_game_11m_handle(ak_msg_t* msg)
{
	switch (msg->sig)
	{
	case SCREEN_ENTRY:
	{
		APP_DBG_SIG("11M_GAME SCREEN_ENTRY\n");
	}
	break;

	case 11M_GAME_EXIT_GAME:
	{
		APP_DBG_SIG("11M_GAME_EXIT_GAME\n");
		11m_game_state = GAME_OFF;
		//SCREEN_TRAN(scr_game_over_handle, &scr_game_over);
	}
	break;

	default:
		break;
	}
}