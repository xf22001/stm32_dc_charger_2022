

/*================================================================
 *
 *
 *   文件名称：function_board_handler_custom.c
 *   创 建 者：肖飞
 *   创建日期：2022年08月01日 星期一 11时35分01秒
 *   修改日期：2022年08月01日 星期一 15时38分46秒
 *   描    述：
 *
 *================================================================*/
#include "function_board_handler_custom.h"
#include "uart_data_task.h"
#include "hw_adc.h"

#include "log.h"

#pragma pack(push, 1)
typedef struct {
	uint16_t magic;//0x5aa5
	uint8_t id;
	uint8_t cmd : 7;
	uint8_t response : 1;
	uint8_t len;
	uint8_t data[0];
} function_board_cmd_header_t;

typedef struct {
	uint8_t crc;
} function_board_cmd_tail_t;

typedef struct {
	uint8_t discharge : 1;
	uint8_t insulation_detect : 1;
	uint8_t adhesion_detect : 1;
	uint8_t insulation_detect_continue_start : 1;
	uint8_t insulation_detect_continue_stop : 1;
	uint8_t unused : 3;
} function_board_control_t;

typedef union {
	function_board_control_t s;
	uint8_t v;
} u_function_board_control_t;

typedef struct {
	uint16_t version;
	uint8_t in_discharge : 1;
	uint8_t in_insulation_detecte : 1;
	uint8_t in_adhesion_detect : 1;
	uint8_t unused : 2;
	uint8_t discharge_resistor_over_temperature : 1;
	uint8_t adhesion_n : 1;
	uint8_t adhesion_p : 1;
	uint8_t charger_voltage;//4.44v
	uint8_t battery_voltage;//4.44v
	uint16_t charger_voltage_dec;//1v
	uint8_t insulation_resistor;//0.1M
	uint8_t dc_p_temperature;//+20
	uint8_t dc_n_temperature;//+20
	uint8_t error;
	uint8_t addr_485;
} function_board_telemetering_data_t;

#pragma pack(pop)

#define FUNCTION_BOARD_BUFFER_LENGTH 64

typedef struct {
	uart_data_task_info_t *uart_data_task_info;
	function_board_action_t request_action;

	uint8_t tx_buffer[FUNCTION_BOARD_BUFFER_LENGTH];
	uint8_t rx_buffer[FUNCTION_BOARD_BUFFER_LENGTH];
	uint32_t periodic_periodic;
} function_board_handler_ctx_t;

static void uart_txrx_mode_cb(void *fn_ctx, uart_rxrx_mode_t mode)
{
	function_board_config_item_t *function_board_config_item = (function_board_config_item_t *)fn_ctx;

	if(mode == UART_RXRX_MODE_RX) {
		HAL_GPIO_WritePin(function_board_config_item->con_gpio, function_board_config_item->con_pin, GPIO_PIN_RESET);
	} else {
		HAL_GPIO_WritePin(function_board_config_item->con_gpio, function_board_config_item->con_pin, GPIO_PIN_SET);
	}
}

static int encode_functon_board_485(uint8_t *tx_buffer, uint8_t tx_buffer_size, uint8_t id, char cmd, uint8_t *data, uint8_t len)
{
	int ret = 0;
	function_board_cmd_header_t *function_board_cmd_header = (function_board_cmd_header_t *)tx_buffer;
	function_board_cmd_tail_t *function_board_cmd_tail;

	if((sizeof(function_board_cmd_header_t) + len + sizeof(function_board_cmd_tail_t)) > tx_buffer_size) {
		debug("");
		return ret;
	}

	function_board_cmd_header->magic = 0x5aa5;
	function_board_cmd_header->id = id;
	function_board_cmd_header->cmd = cmd;
	function_board_cmd_header->response = 0;
	function_board_cmd_header->len = len;

	if(len > 0) {
		memcpy(function_board_cmd_header->data, data, len);
	}

	function_board_cmd_tail = (function_board_cmd_tail_t *)(function_board_cmd_header->data + len);
	function_board_cmd_tail->crc = sum_crc8(function_board_cmd_header, (uint8_t *)function_board_cmd_tail - (uint8_t *)function_board_cmd_header);

	ret = 0;

	return ret;
}

static int decode_function_board_485(uint8_t *rx_buffer, uint8_t rx_buffer_size, uint8_t *id, uint8_t *cmd, uint8_t **data, uint8_t *len)
{
	int ret = -1;
	function_board_cmd_header_t *function_board_cmd_header = (function_board_cmd_header_t *)rx_buffer;
	function_board_cmd_tail_t *function_board_cmd_tail;

	if((sizeof(function_board_cmd_header_t) + function_board_cmd_header->len + sizeof(function_board_cmd_tail_t)) > rx_buffer_size) {
		debug("");
		return ret;
	}

	if(function_board_cmd_header->magic != 0x5aa5) {
		debug("");
		return ret;
	}

	if(function_board_cmd_header->response != 1) {
		debug("");
		return ret;
	}

	function_board_cmd_tail = (function_board_cmd_tail_t *)(function_board_cmd_header->data + function_board_cmd_header->len);

	if(function_board_cmd_tail->crc != sum_crc8(function_board_cmd_header, (uint8_t *)function_board_cmd_tail - (uint8_t *)function_board_cmd_header)) {
		debug("");
		return ret;
	}

	*id = function_board_cmd_header->id;
	*cmd = function_board_cmd_header->cmd;
	*len = function_board_cmd_header->len;
	*data = function_board_cmd_header->data;
	ret = 0;
	return ret;
}

static int control(function_board_info_t *function_board_info, function_board_action_t action)
{
	int ret = -1;
	function_board_handler_ctx_t *function_board_handler_ctx = (function_board_handler_ctx_t *)function_board_info->ctx;
	charger_info_t *charger_info = function_board_info->charger_info;
	channel_info_t *channel_info = charger_info->channel_info;
	u_function_board_control_t u_function_board_control;
	uint8_t received;
	uint8_t id;
	uint8_t cmd;
	uint8_t len;
	uint8_t *response;

	u_function_board_control.v = 0;

	switch(action) {
		case FUNCTION_BOARD_ACTION_DISCHARGE: {
			u_function_board_control.s.discharge = 1;
		}
		break;

		case FUNCTION_BOARD_ACTION_INSULATION_DETECT: {
			u_function_board_control.s.insulation_detect = 1;
		}
		break;

		case FUNCTION_BOARD_ACTION_ADHESION_DETECT: {
			u_function_board_control.s.adhesion_detect = 1;
		}
		break;

		default:
			break;
	}

	if(encode_functon_board_485(function_board_handler_ctx->tx_buffer,
	                            FUNCTION_BOARD_BUFFER_LENGTH,
	                            channel_info->channel_id,
	                            0x15,
	                            (uint8_t *)&u_function_board_control,
	                            sizeof(u_function_board_control)) != 0) {
		debug("");
		return ret;
	}

	received = uart_tx_rx_data(function_board_handler_ctx->uart_data_task_info->uart_info,
	                           function_board_handler_ctx->tx_buffer,
	                           sizeof(function_board_cmd_header_t) + sizeof(u_function_board_control) + sizeof(function_board_cmd_tail_t),
	                           function_board_handler_ctx->rx_buffer,
	                           sizeof(function_board_cmd_header_t) + sizeof(u_function_board_control) + sizeof(function_board_cmd_tail_t),
	                           1000);

	if(received == 0) {
		debug("");
		return ret;
	}

	if(decode_function_board_485(function_board_handler_ctx->rx_buffer, received, &id, &cmd, &response, &len) != 0) {
		_hexdump("control rx", (const char *)function_board_handler_ctx->rx_buffer, received);
		return ret;
	}

	if(id != channel_info->channel_id) {
		debug("");
		return ret;
	}

	if(cmd != 0x15) {
		debug("");
		return ret;
	}

	ret = 0;

	return ret;
}

static int telemetering(function_board_info_t *function_board_info)
{
	int ret = -1;
	function_board_handler_ctx_t *function_board_handler_ctx = (function_board_handler_ctx_t *)function_board_info->ctx;
	charger_info_t *charger_info = function_board_info->charger_info;
	channel_info_t *channel_info = charger_info->channel_info;
	uint8_t received;
	uint8_t id;
	uint8_t cmd;
	uint8_t len = 0;
	uint8_t *response;
	function_board_telemetering_data_t *function_board_telemetering_data;

	if(encode_functon_board_485(function_board_handler_ctx->tx_buffer,
	                            FUNCTION_BOARD_BUFFER_LENGTH,
	                            channel_info->channel_id,
	                            0x11,
	                            NULL,
	                            0) != 0) {
		debug("");
		return ret;
	}

	//_hexdump("telemetering tx", (const char *)function_board_handler_ctx->tx_buffer, sizeof(function_board_cmd_header_t) + 0 + sizeof(function_board_cmd_tail_t));

	received = uart_tx_rx_data(function_board_handler_ctx->uart_data_task_info->uart_info,
	                           function_board_handler_ctx->tx_buffer,
	                           sizeof(function_board_cmd_header_t) + 0 + sizeof(function_board_cmd_tail_t),
	                           function_board_handler_ctx->rx_buffer,
	                           sizeof(function_board_cmd_header_t) + sizeof(function_board_telemetering_data_t) + sizeof(function_board_cmd_tail_t),
	                           1000);
	//_hexdump("telemetering rx", (const char *)function_board_handler_ctx->rx_buffer, received);

	if(received == 0) {
		debug("");
		return ret;
	}

	if(decode_function_board_485(function_board_handler_ctx->rx_buffer, received, &id, &cmd, &response, &len) != 0) {
		_hexdump("telemetering rx", (const char *)function_board_handler_ctx->rx_buffer, received);
		return ret;
	}

	if(id != channel_info->channel_id) {
		debug("");
		return ret;
	}

	if(cmd != 0x11) {
		debug("");
		return ret;
	}

	if(len != sizeof(function_board_telemetering_data_t)) {
		debug("");
		return ret;
	}

	function_board_telemetering_data = (function_board_telemetering_data_t *)response;

	channel_info->function_board_version = function_board_telemetering_data->version;
	channel_info->in_discharge = function_board_telemetering_data->in_discharge;
	channel_info->in_insulation_detecte = function_board_telemetering_data->in_insulation_detecte;
	channel_info->in_adhesion_detect = function_board_telemetering_data->in_adhesion_detect;

	if(function_board_telemetering_data->discharge_resistor_over_temperature != 0) {
		set_fault(channel_info->faults, CHANNEL_FAULT_DISCHARGE_RESISTOR_OVER_TEMPERATURE, 1);
	}

	if(function_board_telemetering_data->adhesion_n != 0) {
		set_fault(channel_info->faults, CHANNEL_FAULT_ADHESION_P, 1);
	}

	if(function_board_telemetering_data->adhesion_p != 0) {
		set_fault(channel_info->faults, CHANNEL_FAULT_ADHESION_N, 1);
	}

	channel_info->function_board_charger_voltage = function_board_telemetering_data->charger_voltage * 4.44 * 10;
	channel_info->function_board_battery_voltage = function_board_telemetering_data->battery_voltage * 4.44 * 10;
	//channel_info->charger_voltage = function_board_telemetering_data->charger_voltage_dec * 10;
	channel_info->insulation_resistor = function_board_telemetering_data->insulation_resistor;
	//channel_info->temperature_p = function_board_telemetering_data->dc_p_temperature - 20;
	//channel_info->temperature_n = function_board_telemetering_data->dc_n_temperature - 20;

	//debug("channel %d function_board_charger_voltage:%lu", channel_info->channel_id, channel_info->function_board_charger_voltage);
	//debug("channel %d function_board_battery_voltage:%lu", channel_info->channel_id, channel_info->function_board_battery_voltage);
	//debug("channel %d charger_voltage_dec:%lu", channel_info->channel_id, function_board_telemetering_data->charger_voltage_dec * 10);
	//debug("channel %d insulation_resistor:%lu", channel_info->channel_id, function_board_telemetering_data->insulation_resistor);
	//debug("channel %d dc_p_temperature:%lu", channel_info->channel_id, function_board_telemetering_data->dc_p_temperature);
	//debug("channel %d dc_n_temperature:%lu", channel_info->channel_id, function_board_telemetering_data->dc_n_temperature);

	//function_board_telemetering_data->error;
	if(function_board_telemetering_data->error != 0) {
		debug("channel %d function board error %x", channel_info->channel_id, function_board_telemetering_data->error);
	}

	//function_board_telemetering_data->addr_485;

	ret = 0;

	return ret;
}

static void uart_data_request(void *fn_ctx, void *chain_ctx)
{
	function_board_info_t *function_board_info = (function_board_info_t *)fn_ctx;
	function_board_handler_ctx_t *function_board_handler_ctx = (function_board_handler_ctx_t *)function_board_info->ctx;
	//charger_info_t *charger_info = function_board_info->charger_info;
	uint32_t ticks = osKernelSysTick();

	if(ticks_duration(ticks, function_board_handler_ctx->periodic_periodic) < 200) {
		return;
	}

	//debug("function_board_handler_ctx->request_action:%s", get_function_board_action_des(function_board_handler_ctx->request_action));

	switch(function_board_handler_ctx->request_action) {
		case FUNCTION_BOARD_ACTION_DISCHARGE:
		case FUNCTION_BOARD_ACTION_INSULATION_DETECT:
		case FUNCTION_BOARD_ACTION_ADHESION_DETECT: {
			if(control(function_board_info, function_board_handler_ctx->request_action) == 0) {
				function_board_info->alive_stamps = ticks;
				function_board_handler_ctx->request_action = FUNCTION_BOARD_ACTION_NONE;
			}
		}
		break;

		case FUNCTION_BOARD_ACTION_TELEMETERING: {
			if(telemetering(function_board_info) == 0) {
				function_board_info->alive_stamps = ticks;
				function_board_handler_ctx->request_action = FUNCTION_BOARD_ACTION_NONE;
			}
		}
		break;

		default: {
			if(telemetering(function_board_info) == 0) {
				function_board_info->alive_stamps = ticks;
			}
		}
		break;
	}
}

static void function_board_action_callback(void *fn_ctx, void *chain_ctx)
{
	function_board_info_t *function_board_info = (function_board_info_t *)fn_ctx;
	function_board_action_args_t *function_board_action_args = (function_board_action_args_t *)chain_ctx;
	function_board_handler_ctx_t *function_board_handler_ctx = (function_board_handler_ctx_t *)function_board_info->ctx;

	function_board_action_args->ret = -1;

	if(function_board_handler_ctx->request_action == FUNCTION_BOARD_ACTION_NONE) {
		function_board_handler_ctx->request_action = function_board_action_args->request_action;
		function_board_action_args->ret = 0;
	}
}

static void handle_charger_temperature(channel_info_t *channel_info)
{
	adc_info_t *adc_info;
	channel_config_t *channel_config = channel_info->channel_config;

	if(channel_config->charger_temperature_p_adc != NULL) {
		adc_info = get_or_alloc_adc_info(channel_config->charger_temperature_p_adc);
		OS_ASSERT(adc_info != NULL);

		channel_info->temperature_p_ad = get_adc_value(adc_info, channel_config->charger_temperature_p_adc_rank);
		channel_info->temperature_p = adc_value_helper(ADC_VALUE_TYPE_TEMPERATURE_P, channel_info->temperature_p_ad);
	}

	if(channel_config->charger_temperature_n_adc != NULL) {
		adc_info = get_or_alloc_adc_info(channel_config->charger_temperature_n_adc);
		OS_ASSERT(adc_info != NULL);

		channel_info->temperature_n_ad = get_adc_value(adc_info, channel_config->charger_temperature_n_adc_rank);
		channel_info->temperature_n = adc_value_helper(ADC_VALUE_TYPE_TEMPERATURE_N, channel_info->temperature_n_ad);
	}
}

static void function_board_handler_periodic(void *fn_ctx, void *chain_ctx)
{
	//function_board_info_t *function_board_info = (function_board_info_t *)fn_ctx;
	channel_info_t *channel_info = (channel_info_t *)chain_ctx;

	handle_charger_temperature(channel_info);
}

static int init(void *_function_board_info)
{
	int ret = -1;
	function_board_info_t *function_board_info = (function_board_info_t *)_function_board_info;
	charger_info_t *charger_info = function_board_info->charger_info;
	channel_info_t *channel_info = charger_info->channel_info;
	channel_settings_t *channel_settings = &channel_info->channel_settings;
	channel_config_t *channel_config = channel_info->channel_config;
	function_board_handler_ctx_t *function_board_handler_ctx;
	function_board_config_item_t *function_board_config_item;

	function_board_config_item = get_function_board_config_item(channel_config, channel_settings->function_board_type);

	if(function_board_config_item == NULL) {
		debug("");
		return ret;
	}

	OS_ASSERT(function_board_info->ctx == NULL);
	function_board_handler_ctx = (function_board_handler_ctx_t *)os_calloc(1, sizeof(function_board_handler_ctx_t));
	OS_ASSERT(function_board_handler_ctx != NULL);
	function_board_info->ctx = function_board_handler_ctx;
	function_board_handler_ctx->uart_data_task_info = get_or_alloc_uart_data_task_info(function_board_config_item->huart);
	OS_ASSERT(function_board_handler_ctx->uart_data_task_info != NULL);

	if(function_board_config_item->con_gpio != NULL) {
		set_uart_txrx_mode_cb(function_board_handler_ctx->uart_data_task_info->uart_info, uart_txrx_mode_cb, function_board_config_item);
	}

	function_board_info->uart_data_request_cb.fn = uart_data_request;
	function_board_info->uart_data_request_cb.fn_ctx = function_board_info;
	add_uart_data_task_info_cb(function_board_handler_ctx->uart_data_task_info, &function_board_info->uart_data_request_cb);

	function_board_info->function_board_action_cb.fn = function_board_action_callback;
	function_board_info->function_board_action_cb.fn_ctx = function_board_info;
	OS_ASSERT(register_callback(function_board_info->function_board_action_chain, &function_board_info->function_board_action_cb) == 0);

	function_board_info->function_board_handler_periodic.fn = function_board_handler_periodic;
	function_board_info->function_board_handler_periodic.fn_ctx = function_board_info;
	OS_ASSERT(register_callback(channel_info->channel_periodic_chain, &function_board_info->function_board_handler_periodic) == 0);

	ret = 0;

	return ret;
}

static int deinit(void *_function_board_info)
{
	int ret = 0;
	function_board_info_t *function_board_info = (function_board_info_t *)_function_board_info;
	function_board_handler_ctx_t *function_board_handler_ctx;
	charger_info_t *charger_info = function_board_info->charger_info;
	channel_info_t *channel_info = charger_info->channel_info;

	if(function_board_info->ctx == NULL) {
		debug("");
		return ret;
	}

	remove_callback(function_board_info->function_board_action_chain, &function_board_info->function_board_action_cb);

	function_board_handler_ctx = (function_board_handler_ctx_t *)function_board_info->ctx;
	uart_info_signal(function_board_handler_ctx->uart_data_task_info->uart_info);
	remove_uart_data_task_info_cb(function_board_handler_ctx->uart_data_task_info, &function_board_info->uart_data_request_cb);

	remove_callback(channel_info->channel_periodic_chain, &function_board_info->function_board_handler_periodic);

	function_board_info->ctx = NULL;
	os_free(function_board_handler_ctx);

	return ret;
}

function_board_handler_t function_board_handler_custom = {
	.type = FUNCTION_BOARD_TYPE_CUSTOM,
	.init = init,
	.deinit = deinit,
};
