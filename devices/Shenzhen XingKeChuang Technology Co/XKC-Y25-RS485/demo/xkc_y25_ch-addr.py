#!/usr/bin/env -S uv run
# /// script
# requires-python = ">=3.13"
# dependencies = [
#   "structlog",
#   "minimalmodbus"
# ]
# ///
# -*- coding: utf-8 -*-
"""Change address / probe the sensor

Wire colors:

Brown: VCC  (Really does need at least 12V. Reliable at 15+)
Blue: GND
Black: RS485 A
Yellow: RS485 B
"""

import argparse
import time

import minimalmodbus
import serial
import structlog

log = structlog.get_logger()

# Constants
SERIAL_PORT = "/dev/ttyUSB0"

# Default is 1, change this value after running this script with --change-address
DEFAULT_SLAVE_ADDRESS = 0x01
REGISTER_ADDRESS_NEW_ADDRESS = 4
PROBE_DELAY_SECONDS = 1


def change_address(instrument, new_address):
    try:
        log.info(
            "Attempting to change sensor address",
            current_address=instrument.address,
            new_address=new_address,
        )
        # Set sensor address command as per datasheet
        instrument.write_register(
            REGISTER_ADDRESS_NEW_ADDRESS, new_address, functioncode=6
        )
        log.info("Address changed successfully", new_address=new_address)
    except Exception as e:
        log.error("Failed to change address", error=str(e))


def probe_sensor(instrument):
    try:
        log.debug("Probing sensor state")
        # Reading sensor signal value
        response = instrument.read_registers(1, 2, functioncode=3)
        high_level_status = response[0]
        low_level_status = response[1]
        log.info(
            "Sensor status",
            high_level_status=high_level_status,
            low_level_status=low_level_status,
        )
    except Exception as e:
        log.error("Failed to read sensor", error=str(e))


def main():
    parser = argparse.ArgumentParser(
        description="Interact with the XKC-Y25-RS485 sensor."
    )
    parser.add_argument(
        "--change-address",
        type=int,
        metavar="NEW_ADDRESS",
        help="Change the sensor address to NEW_ADDRESS (1-254)",
    )
    args = parser.parse_args()

    # Configure the sensor
    log.info(
        "Configuring sensor communication",
        serial_port=SERIAL_PORT,
        slave_address=DEFAULT_SLAVE_ADDRESS,
    )
    instrument = minimalmodbus.Instrument(
        SERIAL_PORT, DEFAULT_SLAVE_ADDRESS, debug=True
    )  # Port name and slave address
    instrument.serial.baudrate = 9600
    instrument.serial.bytesize = 8
    instrument.serial.parity = serial.PARITY_NONE
    instrument.serial.stopbits = 1
    instrument.serial.timeout = 1  # seconds

    if args.change_address:
        change_address(instrument, args.change_address)
    else:
        log.info("Starting sensor probing loop. Press Ctrl+C to stop.")
        try:
            while True:
                probe_sensor(instrument)
                time.sleep(PROBE_DELAY_SECONDS)
        except KeyboardInterrupt:
            log.info("Exiting probe loop.")


if __name__ == "__main__":
    main()
