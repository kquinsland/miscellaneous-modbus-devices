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
Quick PoC for interrogating the QD liquid level sensor.

The mfgr provided documentation seems to imply that the unit can work with
DC from 10 to 36v, 24v nominal.
I had no trouble powering this from a cheap USB to RS485 adapter... that does not have
    a voltage boost circuit so there doesn't seem to be any issue with using logic level power.
"""

import minimalmodbus
import structlog

log = structlog.get_logger(__name__)

# Change as needed
PORT = "/dev/ttyUSB0"
# Standard 9600/8n1
BAUDRATE = 9600
# Default device address is 0x01
DEVICE_ADDRESS = 0x01

# Datasheet describes 8 registers
REG_SLAVE_ADDRESS = 0x00

# 0 - 1200
# 1 - 2400
# 2 - 4800
# 3 - 9600
# 4 - 19200
REG_BAUDRATE = 0x01

# 0 - MPa
# 1 - KPa
# 2 - Pa
# 3 - bar
# 4 - Mbar
# 5 - kg/cm2
# 6 - PSI
# 7 - mH₂O
# 8 - mmH₂O
REG_MEASUREMENT_UNIT = 0x02

# 0 - NNNN
# 1 - NNN.N
# 2 - NN.NN
# 3 - N.NNN
REG_PRECISION = 0x03

# Range between -32768 - 32767
REG_MEASUREMENT_VALUE = 0x04

# Not sure what this is, value is between -32768 - 32767
REG_RANGE_ZERO = 0x05

# Not sure what this is, value is between -32768 - 32767
REG_RANGE_FULL = 0x05

# Not sure what this is, value is between -32768 - 32767
REG_RANGE_ZERO_OFFSET = 0x0C


def main():
    """Does the needful"""
    instrument = minimalmodbus.Instrument(PORT, DEVICE_ADDRESS, debug=False)

    if instrument.serial is None:
        log.error("Serial is None")
        return

    instrument.serial.baudrate = BAUDRATE
    instrument.serial.timeout = 1

    instrument.clear_buffers_before_each_transaction = True
    # For my application, depth in mmH₂O is what I want
    instrument.write_register(REG_MEASUREMENT_UNIT, 0x08)
    # Confirm the unit was set
    unit = instrument.read_register(REG_MEASUREMENT_UNIT)
    if unit != 0x08:
        log.error("Unit not set to mmH₂O", unit=unit)
        return
    
    precision = instrument.read_register(REG_PRECISION)
    # It just so happens that the precision matches the number of decimal places
    # do a quick sanity check and then use it directly with the next read
    if precision > 3 or precision < 0:
        log.error("Precision out of range", precision=precision)
        return
    value = instrument.read_register(REG_MEASUREMENT_VALUE, number_of_decimals=precision, signed=True)

    log.debug("Reading!", unit=unit, precision=precision, value=value)


if __name__ == "__main__":
    main()
