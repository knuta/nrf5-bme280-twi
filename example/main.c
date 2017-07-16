/*
Copyright 2017 Knut Auvor Grythe

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

/** @file
 * @defgroup nrf5-bme280-twi-example main.c
 * @{
 * @ingroup nrf5-bme280-twi-example
 * @brief BME280 TWI Sensor Example main file.
 *
 * This file contains the source code for a sample application using BME280 over TWI.
 */

#include <stdio.h>
#include "boards.h"
#include "app_error.h"
#include "nrf_drv_twi.h"
#include "nrf_delay.h"
#include "bme280_twi.h"

#define NRF_LOG_MODULE_NAME "APP"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"

static volatile bool m_measurement_fetched;
static const nrf_drv_twi_t m_twi = NRF_DRV_TWI_INSTANCE(0);

void twi_handler(nrf_drv_twi_evt_t const * p_event, void * p_context)
{
	// Forward the TWI event to the driver
	bme280_twi_evt_handler(p_event, p_context);
}

void bme280_handler(bme280_twi_evt_t const * p_event, void * p_context)
{
	switch (p_event->type) {
		case BME280_TWI_MEASUREMENT_FETCHED:
			m_measurement_fetched = true;
			break;
		default:
			break;	
	}
}

void twi_init(void)
{
	const nrf_drv_twi_config_t twi_config = {
		.scl                = ARDUINO_SCL_PIN,
		.sda                = ARDUINO_SDA_PIN,
		.frequency          = NRF_TWI_FREQ_100K,
		.interrupt_priority = APP_IRQ_PRIORITY_HIGH,
		.clear_bus_init     = false
	};

	ret_code_t err_code = nrf_drv_twi_init(&m_twi, &twi_config, twi_handler, NULL);
	APP_ERROR_CHECK(err_code);

	nrf_drv_twi_enable(&m_twi);
}

void bme280_init(void) {
	const bme280_twi_config_t bme280_twi_config = {
		.addr = BME280_TWI_ADDR_0,
		.standby = BME280_TWI_STANDBY_250_MS,
		.filter = BME280_TWI_FILTER_OFF,
		.temp_oversampling = BME280_TWI_OVERSAMPLING_X4,
	};

	bme280_twi_init(&m_twi, &bme280_twi_config, bme280_handler, NULL);
	bme280_twi_enable();
}

static void log_temp(void)
{
	bme280_twi_data_t data;
	bme280_twi_measurement_get(&data);

	NRF_LOG_INFO("Temperature: " NRF_LOG_FLOAT_MARKER " degrees Celsius.\r\n",
			NRF_LOG_FLOAT(((float)data.temp)/100));
	NRF_LOG_FLUSH();
}

int main(void)
{
	APP_ERROR_CHECK(NRF_LOG_INIT(NULL));

	NRF_LOG_INFO("BME280 TWI sensor example\r\n");
	NRF_LOG_FLUSH();

	twi_init();
	bme280_init();

	while (true) {
		nrf_delay_ms(500);

		m_measurement_fetched = false;
		bme280_twi_measurement_fetch();

		do {
			__WFE();
		} while (m_measurement_fetched == false);

		log_temp();
	}
}

/** @} */
