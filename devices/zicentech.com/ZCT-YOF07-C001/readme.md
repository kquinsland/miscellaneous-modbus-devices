# ZCT-YLoC1

This is a new (?) liquid level sensor.
I say "new" because I only see a few [references to it on AliExpress](https://www.aliexpress.us/item/3256804396134789.html) and I don't recall seeing it a few years ago when I was last looking for a liquid level sensor.

In any case, it looks like 8 distinct traditional capacitive sensors monitored by a single chip.
That chip can then be interrogated over I2C.

It looks like the I2C details are in: `ZCT-YLOC1连续液位传感器_IIC通信协议中文手册_V00.{docx,pdf}`

A ChatGPT translated version of the document is in [`datasheet-translated.md`](datasheet-translated.md).

I put together a _super_ basic [`test.py`](test.py) which does manage to read from the device... and the values do change when I cover the sensor so I'm pretty sure it works.
