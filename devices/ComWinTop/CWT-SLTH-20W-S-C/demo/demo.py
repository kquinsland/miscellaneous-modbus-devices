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
Quick PoC for interrogating the ceiling mount lux/temp/humidity sensor from ComWinTop.

"""

import minimalmodbus
import structlog

log = structlog.get_logger(__name__)

# Change as needed
PORT = "/dev/ttyUSB0"
# Standard 4800/8n1
BAUDRATE = 4800
# Default device address is 0x01
DEVICE_ADDRESS = 0x01

# 0.1% RH
REG_HUMIDITY = 0x00
# 0.1C
REG_TEMP = 0x01

##
# Datasheet is a bit confusing about this
# There are two SKUs? Either or: 
#   - 0-65,535 lux, resolution of 1 lux 
#   - 0-200,000 lux, resolution of 0.1 lux
#
# If you have the lower range, you won't(?) be able to read
#   from the HIGH/LOW registers.
# But in both cases, you can read from the LUX register.
# If you have the lower range, the LUX register will be units of 1.0 lux.
# If you have the higher range, the LUX register will be units of 0.01 lux (?!)
# 


# 0-200,000 lux 1.0 resolution
REG_LUX_HIGH= 0x02
REG_LUX_LOW= 0x03

# 0-200,000 lux .1 resolution
REG_LUX= 0x06



def main():
    """Does the needful"""
    instrument = minimalmodbus.Instrument(PORT, DEVICE_ADDRESS, debug=False)

    if instrument.serial is None:
        log.error("Serial is None")
        return

    instrument.serial.baudrate = BAUDRATE
    instrument.serial.timeout = 1

    instrument.clear_buffers_before_each_transaction = True

    hum = instrument.read_register(REG_HUMIDITY, number_of_decimals=1, signed=False)
    temp = instrument.read_register(REG_TEMP, number_of_decimals=1, signed=True)
    # Datasheet says to scale by 100 if the unit has the higher range
    lux = instrument.read_register(REG_LUX, number_of_decimals=0, signed=False) * 100
    # Read the 32bit lux value
    lux_hl = instrument.read_long(REG_LUX_HIGH)
    log.debug("Individual data", temp=temp, relative_humidity=hum, lux=lux, lux_hl=lux_hl)


if __name__ == "__main__":
    main()
