#!/usr/bin/env -S uv run
# /// script
# requires-python = ">=3.13"
# dependencies = [
#   "structlog",
#   "smbus2",
# ]
# ///
"""
The sensor came with 6 wires. It took a bit of trial/error but this is what I found:

- W        (?)
- R        VCC
- B        GND
- Y        SDA
- O        SCL
- G        (?)


"""

from smbus2 import SMBus, i2c_msg

# Constants
##
# Did a ❯ sudo ls -lah /dev/i2c-* before/after plugging in to see which one was created
# And for me, it was /dev/i2c-12
# Note that /dev/i2c* is owned by `root:i2c` so I had to `newgrp i2c` to be able to access it
I2C_BUS_NUMBER = 12

# Translated datasheet tells me that it's 0x40
DEVICE_ADDRESS = 0x40
# And that the reg of import is 0x00
REGISTER_WL = 0x00  # Liquid level register


def read_liquid_level():
    with SMBus(I2C_BUS_NUMBER) as bus:
        # Step 1: Write the register address we want to read (no data)
        write = i2c_msg.write(DEVICE_ADDRESS, [REGISTER_WL])
        bus.i2c_rdwr(write)

        # Step 2: Read 1 byte from the device (liquid level value)
        read = i2c_msg.read(DEVICE_ADDRESS, 1)
        bus.i2c_rdwr(read)

        # Convert result to integer
        level = list(read)[0]
        return level


if __name__ == "__main__":
    level = read_liquid_level()
    # With the sensor facing up-side-down, I get 255.
    # When I place my hand over it, I get a value of ~ 180-140 depending on how much I cover it.
    print(f"Liquid Level (0–255): {level}")
