#!/usr/bin/env -S uv run
# /// script
# requires-python = ">=3.13"
# dependencies = [
#   "structlog",
#   "minimalmodbus"
# ]
# ///
# -*- coding: utf-8 -*-
"""
Quick PoC for interrogating the logoele 4-in-1 sensor.
There are 4 sensors, each value has a high/low byte stuffed into a single register.

So if we read address 0x00 + length 4, we get the 4 values.

Setting the device address uses... function code 0x00?!

I could not reliably communicate with the device with supply voltages less than 10v.
"""

import structlog
import minimalmodbus

log = structlog.get_logger(__name__)

# Change as needed
PORT = "/dev/ttyUSB0"
# Standard 9600/8n1
BAUDRATE = 9600
# Default device address is 0x01
DEVICE_ADDRESS = 0x01

REG_CO2 = 0x00
REG_TEMP = 0x01
REG_HUMIDITY = 0x02
REG_LUX = 0x03


def main():
    """Does the needful"""
    instrument = minimalmodbus.Instrument(PORT, DEVICE_ADDRESS, debug=False)

    if instrument.serial is None:
        log.error("Serial is None")
        return

    instrument.serial.baudrate = BAUDRATE
    instrument.serial.timeout = 1

    instrument.clear_buffers_before_each_transaction = True

    # Start from 0x00 read 4 registers
    all_data = instrument.read_registers(REG_CO2, number_of_registers=4)

    # Datasheet says that data will be
    #   co2_high, co2_low, temp_high, temp_low, hum_high, hum_low, lux_high, lux_low
    ##
    co2_high = (all_data[0] >> 8) & 0xFF
    co2_low = all_data[0] & 0xFF
    co2 = co2_high * 256 + co2_low

    temp_high = (all_data[1] >> 8) & 0xFF
    temp_low = all_data[1] & 0xFF
    # Check if positive or negative
    if temp_high & 0x80:
        temp = -1 * ((temp_high & 0x7F) * 256 + temp_low) / 100
    else:
        temp = (temp_high * 256 + temp_low)/100

    # Humidity
    hum_high = (all_data[2] >> 8) & 0xFF
    hum_low = all_data[2] & 0xFF
    hum = (hum_high * 256 + hum_low) / 100

    # Lux
    lux_high = (all_data[3] >> 8) & 0xFF
    lux_low = all_data[3] & 0xFF
    lux = (lux_high * 256 + lux_low) * 4

    log.debug("All data", all_data=all_data, co2=co2, temp=temp, relative_humidity=hum, lux=lux)
    
    # Just for giggles, let's read individual registers
    co2 = instrument.read_register(REG_CO2, number_of_decimals=0, signed=False)
    temp = instrument.read_register(REG_TEMP, number_of_decimals=2, signed=True)
    hum = instrument.read_register(REG_HUMIDITY, number_of_decimals=2, signed=False)
    lux = instrument.read_register(REG_LUX, number_of_decimals=0, signed=False) * 4
    log.debug("Individual data", co2=co2, temp=temp, relative_humidity=hum, lux=lux)


if __name__ == "__main__":
    main()
