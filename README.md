# A nRF5 TWI driver for the BME280 sensor

This is a driver for the BME280 temperature, humidity and pressure sensor. It
supports the version using TWI (aka I2C).

## Usage

Clone this repository into the `examples` folder in the nRF5 SDK. There is an
example project you can use to test your device in the example folder. There
are build scripts for GCC, Keil and IAR. Pick builds for the version of the SDK
you are using (e.g. use `sdk13_pca10040` if you're using SDK 13 with a PCA10040
board).

The example will print the temperature to the UART exposed by the Segger
debugger. This UART is usually exposed as the virtual serial ports COM3 on
Windows and /dev/ttyACM0 on Linux. The device numbers are possibly higher if
you have a lot of devices or plug/unplug the device a few times. The serial
port settings are as usual with the nRF5: 115200 bps, no parity, 1 stop bit and
hardware flow control.

### Pins

The pins used by the example are the default ones for Nordic's TWI examples:

| Board         | SCL   | SDA   |
|---------------|-------|-------|
| PCA10001\*    | P0.07 | P0.30 |
| PCA10028      | P0.07 | P0.30 |
| PCA10036      | P0.27 | P0.26 |
| PCA10040      | P0.27 | P0.26 |
| PCA10056      | P0.27 | P0.26 |
| Arduino Primo |    27 |    26 |

VCC can be hooked up to any 3.3V VDD pin.

\*) PCA10001 isn't officially supported by SDK 12, but I have been able to get
it to work using the build files for PCA10028. The UART doesn't seem to work
using armgcc, but does when using Keil or IAR.

## Driver limitations
 - The driver only supports a single BME280 (I can't think of a use-case where
   you'd need multiple on the same board)
 - Only temperature sensing is supported in the current release, but the driver
   can easily be expanded to support humidity and pressure

## Licence

nrf5-bme280-twi is released under an MIT licence.
