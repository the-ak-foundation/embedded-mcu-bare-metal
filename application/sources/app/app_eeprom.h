#ifndef __APP_EEPROM_H__
#define __APP_EEPROM_H__

#include <stdint.h>
#include "app.h"

/**
  *****************************************************************************
  * EEPROM define address.
  *
  *****************************************************************************
  */
#define EEPROM_START_ADDR           (0X0000)
#define EEPROM_END_ADDR             (0X1000)

#define EEPROM_SCORE_START_ADDR     (0X0010)
#define EEPROM_SCORE_PLAY_ADDR      (0X00FA)

#define EEPROM_SETTING_START_ADDR   (0X0100)

/******************************************************************************/
/* Archey game */
/******************************************************************************/
typedef struct {
  /* score data */
  uint32_t score_now;
  uint32_t score_1st;
  uint32_t score_2nd;
  uint32_t score_3rd;
} zw_game_score_t;

typedef struct {
  /* setting data */
  bool silent;
  uint8_t num_car;
  uint8_t bullet_speed;
  uint8_t zombie_speed;
  uint8_t tombstone_lane_1;  /* bit i = lane i có ít nhất 1 bia mộ */
  uint8_t tombstone_lane_2;  /* bit i = lane i có 2 bia mộ (cần lane_1 bit cũng set) */
} zw_game_setting_t;

/* default settings applied on every boot/reset */
#define ZW_GAME_SETTING_DEFAULT_SILENT          (true)
#define ZW_GAME_SETTING_DEFAULT_NUM_CAR         (0x15)  /* lanes 0,2,4 */
#define ZW_GAME_SETTING_DEFAULT_BULLET_SPEED    (3)
#define ZW_GAME_SETTING_DEFAULT_ZOMBIE_SPEED    (1)
#define ZW_GAME_SETTING_DEFAULT_TOMBSTONE_L1    (0x15)  /* lanes 0,2,4 have >=1 tomb */
#define ZW_GAME_SETTING_DEFAULT_TOMBSTONE_L2    (0x04)  /* lane 2 has 2 tombs */

#define ZW_GAME_EEPROM_MAGIC_NUMBER   ((uint32_t)0xDEAD1234)

#ifdef __cplusplus
extern "C" {
#endif

extern zw_game_score_t gamescore;
extern bool zw_game_score_read(zw_game_score_t* data);
extern bool zw_game_score_write(zw_game_score_t* data);

extern zw_game_setting_t settingdata;
extern bool zw_game_setting_read(zw_game_setting_t* data);
extern bool zw_game_setting_write(zw_game_setting_t* data);

#ifdef __cplusplus
}
#endif

#endif //__APP_EEPROM_H__
