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

/**@file
 *
 * @defgroup bme280_twi TWI driver for BME280
 * @{
 *
 * @brief the BME280 TWI driver interface
 */

#ifndef BME280_TWI_H
#define BME280_TWI_H

#include <stdint.h>
#include <stdbool.h>
#include "nrf_drv_twi.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup bme280_twi_addr Possible TWI addresses for the BME280
 * @{
 */
#define BME280_TWI_ADDR_0          0x76U
#define BME280_TWI_ADDR_1          0x77U
/** @} */

/**
 * @defgroup bme280_twi_oversampling Oversampling values
 * @{
 */
#define BME280_TWI_OVERSAMPLING_SKIPPED  0x00U
#define BME280_TWI_OVERSAMPLING_X1       0x01U
#define BME280_TWI_OVERSAMPLING_X2       0x02U
#define BME280_TWI_OVERSAMPLING_X4       0x03U
#define BME280_TWI_OVERSAMPLING_X8       0x04U
#define BME280_TWI_OVERSAMPLING_X16      0x05U
/** @} */

/**
 * @defgroup bme280_twi_standby Standby values
 * @{
 */
#define BME280_TWI_STANDBY_0p5_MS  0x00U
#define BME280_TWI_STANDBY_10_MS   0x06U
#define BME280_TWI_STANDBY_20_MS   0x07U
#define BME280_TWI_STANDBY_62p5_MS 0x01U
#define BME280_TWI_STANDBY_125_MS  0x02U
#define BME280_TWI_STANDBY_250_MS  0x03U
#define BME280_TWI_STANDBY_500_MS  0x04U
#define BME280_TWI_STANDBY_1000_MS 0x05U
/** @} */

/**
 * @defgroup bme280_twi_filter Filter values
 * @{
 */
#define BME280_TWI_FILTER_OFF      0x00U
#define BME280_TWI_FILTER_2        0x01U
#define BME280_TWI_FILTER_4        0x02U
#define BME280_TWI_FILTER_8        0x03U
#define BME280_TWI_FILTER_16       0x04U
/** @} */

/**
 * @defgroup bme280_twi_mode Measurement modes
 * @{
 */
#define BME280_TWI_MODE_SLEEP  0x00U
#define BME280_TWI_MODE_FORCED 0x01U
#define BME280_TWI_MODE_NORMAL 0x03U
/** @} */

/**
 * @brief Structure for the driver config
 */
typedef struct {
	uint8_t	addr;
	uint8_t	standby;
	uint8_t	filter;
	uint8_t	temp_oversampling;
} bme280_twi_config_t;

/**
 * @brief Structure for the output data
 */
typedef struct {
	int32_t temp;
} bme280_twi_data_t;

/**
 * @brief BME280 TWI driver event types
 */
typedef enum {
    BME280_TWI_MEASUREMENT_FETCHED,  ///< A measurement is ready for reading
} bme280_twi_evt_type_t;

/**
 * @brief Structure for a BME280 event
 */
typedef struct {
	bme280_twi_evt_type_t  type;      ///< Event type.
} bme280_twi_evt_t;

/**
 * @brief BME280 event handler prototype
 */
typedef void (* bme280_twi_evt_handler_t)(bme280_twi_evt_t const * p_event,
                                          void *                   p_context);
/**
 * @brief Function for forwarding a TWI event to the driver
 */
void bme280_twi_evt_handler(nrf_drv_twi_evt_t const * p_event, void * p_context);

/**
 * @brief Initialize the driver, without starting measurements
 *
 * Initializes the driver by writing the config registers to the chip.
 * This function will block until all registers have been written to.
 *
 * @param[in] p_twi         The TWI driver instance to use for communication
 * @param[in] p_config      The config for the BME280 driver
 * @param[in] event_handler A handler for BME280 driver events (mandatory)
 * @param[in] p_context     Context passed to the event handler
 */
void bme280_twi_init(nrf_drv_twi_t const *       p_twi,
                     bme280_twi_config_t const * p_config,
                     bme280_twi_evt_handler_t    event_handler,
                     void *                      p_context);

/**
 * @brief Function for enabling the BME280
 *
 * Starts collection of the data. This is done by writing to the CTRL_MEAS
 * register on the chi. This function will block until the write has completed.
 * To start fetching the collected data from the chip, allow it to make at least
 * one measurement and call bme280_twi_measurement_fetch().
 */
void bme280_twi_enable(void);

/**
 * @brief Function for starting a fetch of the latest measurement data from the chip
 *
 * Starts an asynchronous fetch of measurement data from the chip.
 * When completed, the supplied callback method is called with the event
 * type BME280_TWI_MEASUREMENT_FETCHED. You may then get the data using
 * bme280_twi_measurement_get().
 */
void bme280_twi_measurement_fetch(void);

/**
 * @brief Function for getting the last data the driver has fetched from the chip
 *
 * @param[out] data The data which was last fetched from the chip
 */
void bme280_twi_measurement_get(bme280_twi_data_t *data);

#ifdef __cplusplus
}
#endif

#endif //BME280_TWI_H

/** @} */
