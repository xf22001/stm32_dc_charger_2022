

/*================================================================
 *   
 *   
 *   文件名称：storage_config.c
 *   创 建 者：肖飞
 *   创建日期：2021年08月24日 星期二 21时28分12秒
 *   修改日期：2022年07月30日 星期六 10时03分48秒
 *   描    述：
 *
 *================================================================*/
#include "storage_config.h"
#include "storage.h"
#include "main.h"

extern SPI_HandleTypeDef hspi2;

static storage_config_t storage_config_24lc128 = {
	.type = STORAGE_TYPE_24LC128,
	.hstorage = &hspi2,
	.gpio_port_spi_cs = e2cs_GPIO_Port,
	.gpio_pin_spi_cs = e2cs_Pin,
};

static storage_config_t *storage_config_sz[] = {
	&storage_config_24lc128,
};

storage_config_t *get_storage_config(void *hstorage)
{
	uint8_t i;
	storage_config_t *storage_config = NULL;
	storage_config_t *storage_config_item = NULL;

	for(i = 0; i < ARRAY_SIZE(storage_config_sz); i++) {
		storage_config_item = storage_config_sz[i];

		if(hstorage == storage_config_item->hstorage) {
			storage_config = storage_config_item;
			break;
		}
	}

	return storage_config;
}
