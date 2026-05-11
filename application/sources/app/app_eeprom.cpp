#include "app_eeprom.h"
#include "eeprom.h"

typedef struct {
	uint32_t         magic_number;
	zw_game_score_t  score;
	uint8_t          check_sum;
} __attribute__((packed)) zw_score_ee_t;

typedef struct {
	uint32_t           magic_number;
	zw_game_setting_t  setting;
	uint8_t            check_sum;
} __attribute__((packed)) zw_setting_ee_t;

static uint8_t calc_checksum(const uint8_t *data, uint32_t len) {
	uint8_t sum = 0;
	for (uint32_t i = 0; i < len; i++) {
		sum += data[i];
	}
	return sum;
}

#ifdef __cplusplus
extern "C" {
#endif

zw_game_score_t   gamescore;
zw_game_setting_t settingdata;

bool zw_game_score_read(zw_game_score_t* data) {
	zw_score_ee_t ee;
	eeprom_read(EEPROM_SCORE_START_ADDR, (uint8_t*)&ee, sizeof(ee));

	uint8_t sum = calc_checksum((const uint8_t*)&ee.magic_number,
	                            sizeof(ee.magic_number) + sizeof(ee.score));

	if (ee.magic_number != ZW_GAME_EEPROM_MAGIC_NUMBER || ee.check_sum != sum) {
		data->score_now = 0;
		data->score_1st = 0;
		data->score_2nd = 0;
		data->score_3rd = 0;
		zw_game_score_write(data);
		return false;
	}

	*data = ee.score;
	return true;
}

bool zw_game_score_write(zw_game_score_t* data) {
	zw_score_ee_t ee;
	ee.magic_number = ZW_GAME_EEPROM_MAGIC_NUMBER;
	ee.score        = *data;
	ee.check_sum    = calc_checksum((const uint8_t*)&ee.magic_number,
	                                sizeof(ee.magic_number) + sizeof(ee.score));
	eeprom_write(EEPROM_SCORE_START_ADDR, (uint8_t*)&ee, sizeof(ee));
	return true;
}

bool zw_game_setting_read(zw_game_setting_t* data) {
	zw_setting_ee_t ee;
	eeprom_read(EEPROM_SETTING_START_ADDR, (uint8_t*)&ee, sizeof(ee));

	uint8_t sum = calc_checksum((const uint8_t*)&ee.magic_number,
	                            sizeof(ee.magic_number) + sizeof(ee.setting));

	if (ee.magic_number != ZW_GAME_EEPROM_MAGIC_NUMBER || ee.check_sum != sum) {
		data->silent           = ZW_GAME_SETTING_DEFAULT_SILENT;
		data->num_car          = ZW_GAME_SETTING_DEFAULT_NUM_CAR;
		data->bullet_speed     = ZW_GAME_SETTING_DEFAULT_BULLET_SPEED;
		data->zombie_speed     = ZW_GAME_SETTING_DEFAULT_ZOMBIE_SPEED;
		data->tombstone_lane_1 = ZW_GAME_SETTING_DEFAULT_TOMBSTONE_L1;
		data->tombstone_lane_2 = ZW_GAME_SETTING_DEFAULT_TOMBSTONE_L2;
		zw_game_setting_write(data);
		return false;
	}

	*data = ee.setting;
	return true;
}

bool zw_game_setting_write(zw_game_setting_t* data) {
	zw_setting_ee_t ee;
	ee.magic_number = ZW_GAME_EEPROM_MAGIC_NUMBER;
	ee.setting      = *data;
	ee.check_sum    = calc_checksum((const uint8_t*)&ee.magic_number,
	                                sizeof(ee.magic_number) + sizeof(ee.setting));
	eeprom_write(EEPROM_SETTING_START_ADDR, (uint8_t*)&ee, sizeof(ee));
	return true;
}

#ifdef __cplusplus
}
#endif
