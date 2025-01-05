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
A very simple script to set baud rate / address of the LD58A08 module and to
read/write gpio pin state.
"""

import argparse

import minimalmodbus
import serial
import structlog

# Configure structlog for JSON formatted logging
structlog.configure(
    processors=[
        structlog.processors.TimeStamper(fmt="iso"),
        structlog.processors.JSONRenderer(),
    ]
)
log = structlog.get_logger()

# Constants
DEFAULT_SERIAL_PORT = "/dev/ttyUSB0"

DEFAULT_SLAVE_ADDRESS = 1
DEFAULT_BAUDRATE = 9600
DEFAULT_TIMEOUT_SECONDS = 1  # seconds


class ModbusIOExpander:
    """A toy class to interact with the LD58A08 module"""

    def __init__(self, port=DEFAULT_SERIAL_PORT, slave_address=DEFAULT_SLAVE_ADDRESS):
        self.instrument = minimalmodbus.Instrument(port, slave_address)
        self.instrument.serial.baudrate = DEFAULT_BAUDRATE
        # 8 and 1
        self.instrument.serial.bytesize = 8
        self.instrument.serial.parity = serial.PARITY_NONE
        self.instrument.serial.stopbits = 1
        self.instrument.serial.timeout = DEFAULT_TIMEOUT_SECONDS
        log.info("ModbusIOExpander initialized", port=port, slave_address=slave_address)

    def set_address(self, new_address):
        """Set the device address"""
        # Register address 0x00FD (253) is used to set the RS485 address
        self.instrument.write_register(0x00FD, new_address, functioncode=6)
        log.info("Device address set", new_address=hex(new_address))

    def set_baud_rate(self, baud_rate_code):
        """Set the device baud rate"""
        # Register address 0x00FE (254) is used to set the baud rate
        self.instrument.write_register(0x00FE, baud_rate_code, functioncode=6)
        log.info("Baud rate set", baud_rate_code=baud_rate_code)

    def read_gpio(self, pin):
        """Read the state of a GPIO pin"""
        # Read digital output status for pin (0-7)
        value = self.instrument.read_bit(pin, functioncode=1)
        status = "ON" if value else "OFF"
        log.info("GPIO state read", pin=pin, status=status)

    def set_gpio(self, pin, state):
        """Set the state of a GPIO pin"""
        # Write single digital output for pin (0-7)
        value = 1 if state else 0
        self.instrument.write_bit(pin, value, functioncode=5)
        action = "ON" if state else "OFF"
        log.info("GPIO state set", pin=pin, action=action)


def main():
    """entry point"""
    parser = argparse.ArgumentParser(description="Interact with Modbus device")
    parser.add_argument(
        "--set-address",
        type=lambda x: int(x, 0),
        help="Set the device address (in hex)",
    )
    parser.add_argument(
        "--set-baud-rate",
        type=int,
        choices=range(8),
        # pylint: disable=line-too-long
        help="Set the device baud rate (0:1200, 1:2400, 2:4800, 3:9600, 4:19200, 5:38400, 6:57600, 7:115200)",
    )
    parser.add_argument(
        "--read", type=int, choices=range(8), help="Read GPIO pin state (0-7)"
    )
    parser.add_argument(
        "--on", type=int, choices=range(8), help="Set GPIO pin ON (0-7)"
    )
    parser.add_argument(
        "--off", type=int, choices=range(8), help="Set GPIO pin OFF (0-7)"
    )

    args = parser.parse_args()
    device = ModbusIOExpander()

    if args.set_address is not None:
        device.set_address(args.set_address)
    elif args.set_baud_rate is not None:
        device.set_baud_rate(args.set_baud_rate)
    elif args.read is not None:
        device.read_gpio(args.read)
    elif args.on is not None:
        device.set_gpio(args.on, True)
    elif args.off is not None:
        device.set_gpio(args.off, False)
    else:
        log.error("No valid command provided. Use -h for help.")


if __name__ == "__main__":
    main()
