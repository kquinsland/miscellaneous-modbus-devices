# miscellaneous-modbus-devices

An assortment of demo code and datasheets for various modbus devices.

## Python code

Most everything here uses `uv` to make the `venv` and dependency management easier. 

[Installing `uv` is easy](https://docs.astral.sh/uv/) as it's a static rust binary.

Beyond that, I've done my best to keep the `pyproject.toml` _and_ the [script headers (PEP 723)](https://peps.python.org/pep-0723/) up to date with the dependencies so running the script should be as simple as invoking it as if it were any other binary:

```shell
❯ ./uv-demo.py
Reading inline script metadata from `./uv-demo.py`
Installed 1 package in 1ms
2024-12-01 07:50:09 [info     ] Look ma! no manual venv setup!
```

## Prior work

Previously, I would create a single repo per device or would casually include the manufacturer provided datasheets deep inside the repo for the project using the device.

This ... does not scale well.

Anyways, here's links to the old repos:

- https://github.com/kquinsland/qdx50d-air-pressure-sensor-poc
- https://github.com/kquinsland/generic-chinese-water-quality-sensor
