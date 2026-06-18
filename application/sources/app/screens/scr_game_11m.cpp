#include "scr_game_11m.h"

/*****************************************************************************/
/* Variable Declaration - Game 11m */
/*****************************************************************************/

static uint8_t em_game_state;

void em_game_frame_display()
{
    view_render.clear();

    // Khung thành từ mép trên màn hình
    view_render.drawRect(10, 0, 92, 28, WHITE);
    view_render.drawRect(14, 4, 84, 24, WHITE);

    // Vạch ngang sân
    view_render.drawLine(0, 27, LCD_WIDTH - 1, 27, WHITE);

    // Vạch sân phối cảnh
    view_render.drawLine(18, 27, 2, LCD_HEIGHT - 9, WHITE);
    view_render.drawLine(106, 31, LCD_WIDTH - 3, LCD_HEIGHT - 5, WHITE);
    view_render.drawLine(2, LCD_HEIGHT - 5, LCD_WIDTH - 3, LCD_HEIGHT - 5, WHITE);

    // Chấm pen
    view_render.fillRect(61, 43, 3, 3, WHITE);
}

/*****************************************************************************/
/* View - Game 11m */
/*****************************************************************************/

static void view_scr_game_11m();

view_dynamic_t dyn_view_11m = {
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
	if (em_game_state == GAME_PLAY)
	{
        em_game_frame_display();
        view_render.drawBitmap(61, 43, bitmap_ball, 7, 7, WHITE);
	}
	else if (em_game_state == GAME_OVER)
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
        em_game_state = GAME_PLAY;
	}
	break;

	case EM_GAME_EXIT_GAME:
	{
		APP_DBG_SIG("AC_DISPLAY_GAME_11M_EXIT\n");
		em_game_state = GAME_OFF;
		//SCREEN_TRAN(scr_game_over_handle, &scr_game_over);
	}
	break;

	default:
		break;
	}
}
