#!/usr/bin/env -S uv run
# /// script
# requires-python = ">=3.13"
# dependencies = [
#   "structlog",
# ]
# ///
"""
Quick and dirty demo for embedded metadata in a script using PEP 723 format
"""

import structlog

log = structlog.get_logger(__name__)


def main():
    """Does the needful"""
    log.info("Look ma! no manual venv setup!")


if __name__ == "__main__":
    main()
