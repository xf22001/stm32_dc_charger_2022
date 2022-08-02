

/*================================================================
 *
 *
 *   文件名称：channels_config.c
 *   创 建 者：肖飞
 *   创建日期：2021年01月18日 星期一 09时26分44秒
 *   修改日期：2022年08月02日 星期二 08时45分23秒
 *   描    述：
 *
 *================================================================*/
#include "channels_config.h"

#include "main.h"

#include "os_utils.h"
#include "power_modules.h"

extern CAN_HandleTypeDef hcan1;
extern CAN_HandleTypeDef hcan2;
extern SPI_HandleTypeDef hspi3;
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart3;
extern UART_HandleTypeDef huart4;
extern UART_HandleTypeDef huart6;
extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc3;

char *get_channel_config_channel_type_des(channel_type_t type)
{
	char *des = "unknow";

	switch(type) {
			add_des_case(CHANNEL_TYPE_NONE);
			add_des_case(CHANNEL_TYPE_NATIVE);
			add_des_case(CHANNEL_TYPE_PROXY_REMOTE);
			add_des_case(CHANNEL_TYPE_PROXY_LOCAL);

		default: {
		}
		break;
	}

	return des;
}

char *get_channel_config_charger_bms_type_des(channel_charger_bms_type_t type)
{
	char *des = "unknow";

	switch(type) {
			add_des_case(CHANNEL_CHARGER_BMS_TYPE_NONE);
			add_des_case(CHANNEL_CHARGER_BMS_TYPE_GB);
			add_des_case(CHANNEL_CHARGER_BMS_TYPE_AC);
			add_des_case(CHANNEL_CHARGER_BMS_TYPE_NOBMS);
			add_des_case(CHANNEL_CHARGER_BMS_TYPE_CUSTOM);

		default: {
		}
		break;
	}

	return des;
}

char *get_channel_config_energy_meter_type_des(energy_meter_type_t type)
{
	char *des = "unknow";

	switch(type) {
			add_des_case(ENERGY_METER_TYPE_NONE);
			add_des_case(ENERGY_METER_TYPE_AC);
			add_des_case(ENERGY_METER_TYPE_AC_HLW8032);
			add_des_case(ENERGY_METER_TYPE_DC);
			add_des_case(ENERGY_METER_TYPE_AC_SDM_220);
			add_des_case(ENERGY_METER_TYPE_AC_SDM_630);
			add_des_case(ENERGY_METER_TYPE_PROXY);

		default: {
		}
		break;
	}

	return des;
}

char *get_channel_config_function_board_type_des(function_board_type_t type)
{
	char *des = "unknow";

	switch(type) {
			add_des_case(FUNCTION_BOARD_TYPE_NONE);
			add_des_case(FUNCTION_BOARD_TYPE_485);
			add_des_case(FUNCTION_BOARD_TYPE_MODBUS);
			add_des_case(FUNCTION_BOARD_TYPE_V5);

		default: {
		}
		break;
	}

	return des;
}

char *get_card_reader_type_des(card_reader_type_t type)
{
	char *des = "unknow";

	switch(type) {
			add_des_case(CARD_READER_TYPE_PSEUDO);
			add_des_case(CARD_READER_TYPE_MT_318_626);
			add_des_case(CARD_READER_TYPE_MT_318_628);
			add_des_case(CARD_READER_TYPE_ZLG);

		default: {
		}
		break;
	}

	return des;
}

char *get_power_manager_type_des(power_manager_type_t type)
{
	char *des = "unknow";

	switch(type) {
			add_des_case(POWER_MANAGER_TYPE_NONE);
			add_des_case(POWER_MANAGER_TYPE_NATIVE);

		default: {
		}
		break;
	}

	return des;
}

static energy_meter_config_item_t energy_meter_config_item_0_0 = {
	.type = ENERGY_METER_TYPE_DC,
	.huart = &huart4,
	//.led_gpio = ,
	//.led_pin = ,
};

static energy_meter_config_item_t *energy_meter_config_item_0_sz[] = {
	&energy_meter_config_item_0_0,
};

static function_board_config_item_t function_board_config_item_0_0 = {
	.type = FUNCTION_BOARD_TYPE_485,
	.huart = &huart3,
};

static function_board_config_item_t *function_board_config_item_0_sz[] = {
	&function_board_config_item_0_0,
};

static channel_config_t channel0_config = {
	.channel_type = CHANNEL_TYPE_NATIVE,
	.charger_config = {
		.charger_type = CHANNEL_CHARGER_BMS_TYPE_GB,
		.output_relay_gpio = rey3_GPIO_Port,
		.output_relay_pin = rey3_Pin,
		//.output_relay_gpio_fb = ,
		//.output_relay_pin_fb = ,
		.hcan_bms = &hcan2,
		.relay_charger_lock_p_gpio = lock_a_GPIO_Port,
		.relay_charger_lock_p_pin = lock_a_Pin,
		.relay_charger_lock_n_gpio = unlock_a_GPIO_Port,
		.relay_charger_lock_n_pin = unlock_a_Pin,
		.charger_lock_state_gpio = bk_a_GPIO_Port,
		.charger_lock_state_pin = bk_a_Pin,
		.charger_auxiliary_power_choose_gpio = rey4_GPIO_Port,
		.charger_auxiliary_power_choose_pin = rey4_Pin,
		.charger_auxiliary_power_onoff_gpio = rey1_GPIO_Port,//rey2_GPIO_Port
		.charger_auxiliary_power_onoff_pin = rey1_Pin,//rey2_Pin
	},
	.energy_meter_config = {
		.default_type = ENERGY_METER_TYPE_DC,
		.request_addr = 0,
		.size = ARRAY_SIZE(energy_meter_config_item_0_sz),
		.items = energy_meter_config_item_0_sz,
	},
	.function_board_config = {
		.default_type = FUNCTION_BOARD_TYPE_485,
		.size = ARRAY_SIZE(function_board_config_item_0_sz),
		.items = function_board_config_item_0_sz,
	},
	.charger_temperature_p_adc = &hadc3,
	.charger_temperature_p_adc_rank = 1,
	.charger_temperature_n_adc = &hadc3,
	.charger_temperature_n_adc_rank = 2,
	.cp_gpio = cc1_a_GPIO_Port,
	.cp_pin = cc1_a_Pin,
	//.adhe_p_gpio = ,
	//.adhe_p_pin = ,
	//.adhe_n_gpio = ,
	//.adhe_n_pin = ,
	.fault_port = rey7_GPIO_Port,
	.fault_pin = rey7_Pin,
	.led_charge_port = rey5_GPIO_Port,
	.led_charge_pin = rey5_Pin,
	.led_full_port = rey6_GPIO_Port,
	.led_full_pin = rey6_Pin,
};

static energy_meter_config_item_t energy_meter_config_item_1_0 = {
	.type = ENERGY_METER_TYPE_DC,
	.huart = &huart4,
	//.led_gpio = ,
	//.led_pin = ,
};

static energy_meter_config_item_t *energy_meter_config_item_1_sz[] = {
	&energy_meter_config_item_1_0,
};

static function_board_config_item_t function_board_config_item_1_0 = {
	.type = FUNCTION_BOARD_TYPE_485,
	.huart = &huart3,
};

static function_board_config_item_t *function_board_config_item_1_sz[] = {
	&function_board_config_item_1_0,
};

static channel_config_t channel1_config = {
	.channel_type = CHANNEL_TYPE_NATIVE,
	.charger_config = {
		.charger_type = CHANNEL_CHARGER_BMS_TYPE_GB,
		.output_relay_gpio = rey10_GPIO_Port,
		.output_relay_pin = rey10_Pin,
		//.output_relay_gpio_fb = ,
		//.output_relay_pin_fb = ,
		.hcan_bms = &hspi3,
		.relay_charger_lock_p_gpio = lock_b_GPIO_Port,
		.relay_charger_lock_p_pin = lock_b_Pin,
		.relay_charger_lock_n_gpio = unlock_b_GPIO_Port,
		.relay_charger_lock_n_pin = unlock_b_Pin,
		.charger_lock_state_gpio = bk_b_GPIO_Port,
		.charger_lock_state_pin = bk_b_Pin,
		.charger_auxiliary_power_choose_gpio = rey11_GPIO_Port,
		.charger_auxiliary_power_choose_pin = rey11_Pin,
		.charger_auxiliary_power_onoff_gpio = rey8_GPIO_Port,//rey9_GPIO_Port
		.charger_auxiliary_power_onoff_pin = rey8_Pin,//rey9_Pin
	},
	.energy_meter_config = {
		.default_type = ENERGY_METER_TYPE_DC,
		.request_addr = 0,
		.size = ARRAY_SIZE(energy_meter_config_item_1_sz),
		.items = energy_meter_config_item_1_sz,
	},
	.function_board_config = {
		.default_type = FUNCTION_BOARD_TYPE_485,
		.size = ARRAY_SIZE(function_board_config_item_1_sz),
		.items = function_board_config_item_1_sz,
	},
	.charger_temperature_p_adc = &hadc3,
	.charger_temperature_p_adc_rank = 3,
	.charger_temperature_n_adc = &hadc3,
	.charger_temperature_n_adc_rank = 4,
	.cp_gpio = cc1_b_GPIO_Port,
	.cp_pin = cc1_b_Pin,
	//.adhe_p_gpio = ,
	//.adhe_p_pin = ,
	//.adhe_n_gpio = ,
	//.adhe_n_pin = ,
	.fault_port = rey14_GPIO_Port,
	.fault_pin = rey14_Pin,
	.led_charge_port = rey12_GPIO_Port,
	.led_charge_pin = rey12_Pin,
	.led_full_port = rey13_GPIO_Port,
	.led_full_pin = rey13_Pin,
};

static channel_config_t *channel_config_sz[] = {
	&channel0_config,
	&channel1_config,
};

static card_reader_config_item_t card_reader_config_item_0 = {
	.type = CARD_READER_TYPE_ZLG,
	.huart = &huart1,
};

//static card_reader_config_item_t card_reader_config_item_0 = {
//	.type = CARD_READER_TYPE_PSEUDO,
//	.huart = NULL,
//};

static card_reader_config_item_t *card_reader_config_item_sz[] = {
	&card_reader_config_item_0,
};

static channels_config_t channels_config_0 = {
	.id = 0,
	.channel_number = ARRAY_SIZE(channel_config_sz),
	.channel_config = channel_config_sz,
	.power_module_config = {
		.hcan = &hcan1,
		.power_module_default_type = POWER_MODULE_TYPE_WINLINE,
	},
	.power_manager_config = {
		.power_manager_default_type = POWER_MANAGER_TYPE_NATIVE,
	},
	.voice_config = {
	},
	.card_reader_config = {
		.default_type = CARD_READER_TYPE_ZLG,
		.size = ARRAY_SIZE(card_reader_config_item_sz),
		.items = card_reader_config_item_sz,
	},
	.display_config = {
		//.huart = &huart6,
	},
	.proxy_channel_info = {
		.hcan = &hcan1,
	},
	.board_temperature_adc = &hadc3,
	.board_temperature_adc_rank = 8,
	.force_stop_port = GPIOF,
	.force_stop_pin = GPIO_PIN_2,
	.force_stop_normal_state = GPIO_PIN_RESET,
	.fan_port = GPIOG,
	.fan_pin = GPIO_PIN_1,
	.door_port = GPIOE,
	.door_pin = GPIO_PIN_7,
	.door_normal_state = GPIO_PIN_RESET,
	.fault_port = GPIOI,
	.fault_pin = GPIO_PIN_8,
};

static channels_config_t *channels_config_sz[] = {
	&channels_config_0,
};

channels_config_t *get_channels_config(uint8_t id)
{
	int i;
	channels_config_t *channels_config = NULL;
	channels_config_t *channels_config_item = NULL;

	for(i = 0; i < ARRAY_SIZE(channels_config_sz); i++) {
		channels_config_item = channels_config_sz[i];

		if(channels_config_item->id == id) {
			channels_config = channels_config_item;
			break;
		}
	}

	return channels_config;
}

energy_meter_config_item_t *get_energy_meter_config_item(channel_config_t *channel_config, energy_meter_type_t type)
{
	int i;
	energy_meter_config_t *energy_meter_config = &channel_config->energy_meter_config;
	energy_meter_config_item_t *energy_meter_config_item = NULL;

	for(i = 0; i < energy_meter_config->size; i++) {
		energy_meter_config_item_t *_energy_meter_config_item = energy_meter_config->items[i];

		if(_energy_meter_config_item->type == type) {
			energy_meter_config_item = _energy_meter_config_item;
			break;
		}
	}

	return energy_meter_config_item;
}

function_board_config_item_t *get_function_board_config_item(channel_config_t *channel_config, function_board_type_t type)
{
	int i;
	function_board_config_t *function_board_config = &channel_config->function_board_config;
	function_board_config_item_t *function_board_config_item = NULL;

	for(i = 0; i < function_board_config->size; i++) {
		function_board_config_item_t *_function_board_config_item = function_board_config->items[i];

		if(_function_board_config_item->type == type) {
			function_board_config_item = _function_board_config_item;
			break;
		}
	}

	return function_board_config_item;
}

card_reader_config_item_t *get_card_reader_config_item(channels_config_t *channels_config, card_reader_type_t type)
{
	int i;
	card_reader_config_t *card_reader_config = &channels_config->card_reader_config;
	card_reader_config_item_t *card_reader_config_item = NULL;

	for(i = 0; i < card_reader_config->size; i++) {
		card_reader_config_item_t *_card_reader_config_item = card_reader_config->items[i];

		if(_card_reader_config_item->type == type) {
			card_reader_config_item = _card_reader_config_item;
			break;
		}
	}

	return card_reader_config_item;
}

proxy_channel_item_t *get_proxy_channel_item_by_proxy_channel_index(proxy_channel_info_t *proxy_channel_info, uint8_t proxy_channel_index)
{
	proxy_channel_item_t *item = NULL;
	int i;

	if(proxy_channel_info->proxy_channel_number == 0) {
		return item;
	}

	if(proxy_channel_info->items == NULL) {
		return item;
	}

	for(i = 0; i < proxy_channel_info->proxy_channel_number; i++) {
		proxy_channel_item_t *_item = proxy_channel_info->items + i;

		if(_item->proxy_channel_index == proxy_channel_index) {
			item = _item;
			break;
		}
	}

	return item;
}

proxy_channel_item_t *get_proxy_channel_item_by_channel_id(proxy_channel_info_t *proxy_channel_info, uint8_t channel_id)
{
	proxy_channel_item_t *item = NULL;
	int i;

	if(proxy_channel_info->proxy_channel_number == 0) {
		return item;
	}

	if(proxy_channel_info->items == NULL) {
		return item;
	}

	for(i = 0; i < proxy_channel_info->proxy_channel_number; i++) {
		proxy_channel_item_t *_item = proxy_channel_info->items + i;

		if(_item->channel_id == channel_id) {
			item = _item;
			break;
		}
	}

	return item;
}

int adc_value_helper(adc_value_type_t adc_value_type, uint16_t adc_value)
{
	int value = 0;

	switch(adc_value_type) {
		case ADC_VALUE_TYPE_BOARD_TEMPERATURE: {
			value = get_ntc_temperature(10000, adc_value, 4095);
		}
		break;

		case ADC_VALUE_TYPE_TEMPERATURE_P:
		case ADC_VALUE_TYPE_TEMPERATURE_N: {
			value = 260 * (adc_value / 1241 / 2 - 1);
		}
		break;

		default: {
			app_panic();
		}
		break;
	}

	return value;
}
