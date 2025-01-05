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
Interact with the R46CA01 module.
Set the address or poll temperature data
"""

import argparse
import time

import minimalmodbus
import serial
import structlog

# Configure structlog for logging
structlog.configure(
    processors=[
        structlog.processors.TimeStamper(fmt="iso"),
        structlog.processors.JSONRenderer(),
    ]
)
log = structlog.get_logger()

# Constants
SERIAL_PORT = "/dev/ttyUSB0"

# Datasheet says default of 1; change the value below after running this with --change-address
DEFAULT_SLAVE_ADDRESS = 0x14

# Reg 0x00 is the temperature
REGISTER_TEMPERATURE = 0x0000

# Device addr register
REGISTER_DEV_ADDRESS = 0x0002
REGISTER_BAUD_RATE = 0x0003
# TODO: add support for calibrating?
REGISTER_TEMP_OFFSET = 0x0004

# Device has auto-poll?
REGISTER_TEMP_AUTO_POLL_INTERVAL = 0x0005

PROBE_DELAY_SECONDS = 1  # Time delay between temperature readings (in seconds)


def change_address(instrument, new_address):
    """
    Function to change the 485 address of the module.
    """
    try:
        log.info(
            "Attempting to change module address",
            current_address=instrument.address,
            new_address_dec=new_address,
            new_address_hex=hex(new_address),
        )
        # Change the 485 address (function code 0x06 for writing)
        instrument.write_register(REGISTER_DEV_ADDRESS, new_address, functioncode=6)
        log.info(
            "Address changed successfully",
            new_address_dec=new_address,
            new_address_hex=hex(new_address),
        )
    except Exception as e:
        log.error("Failed to change address", error=str(e))


def read_temperature(instrument):
    """
    Function to read temperature from the sensor.
    """
    try:
        log.debug("Reading temperature from sensor")

        response = instrument.read_register(
            REGISTER_TEMPERATURE, number_of_decimals=1, signed=True
        )
        temperature_value = (
            response  #  / 10  # Convert raw data to temperature in Celsius
        )
        log.info("Temperature read successfully", temperature=temperature_value)
    except Exception as e:
        log.error("Failed to read temperature", error=str(e))


def main():
    parser = argparse.ArgumentParser(
        description="Interact with the R46CA01 temperature sensor."
    )
    parser.add_argument(
        "--change-address",
        type=int,
        metavar="NEW_ADDRESS",
        help="Change the sensor address to NEW_ADDRESS (1-247)",
    )
    args = parser.parse_args()

    # Configure the sensor communication
    log.info(
        "Configuring module communication",
        serial_port=SERIAL_PORT,
        slave_address=DEFAULT_SLAVE_ADDRESS,
    )
    instrument = minimalmodbus.Instrument(
        SERIAL_PORT, DEFAULT_SLAVE_ADDRESS, debug=True
    )
    instrument.serial.baudrate = 9600  # Default baud rate
    instrument.serial.bytesize = 8
    instrument.serial.parity = serial.PARITY_NONE
    instrument.serial.stopbits = 1
    instrument.serial.timeout = 1  # Timeout in seconds

    if args.change_address:
        change_address(instrument, args.change_address)
    else:
        log.info("Starting temperature reading loop. Press Ctrl+C to stop.")
        try:
            while True:
                read_temperature(instrument)
                time.sleep(PROBE_DELAY_SECONDS)  # Delay between temperature readings
        except KeyboardInterrupt:
            log.info("Exiting temperature reading loop.")


if __name__ == "__main__":
    main()
