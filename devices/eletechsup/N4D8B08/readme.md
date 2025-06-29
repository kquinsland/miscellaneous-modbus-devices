# Eletechsup N4D8B08

This is a [basic 8 channel relay board](https://www.485io.com/rs485-relays-c-2_3_19/n4d8b08-dc-12v-8i8o-multifunction-modbus-rtu-relay-module-support-03-06-16-function-code-rs485-switch-control-board-din35-rail-box-p-1333.html).

The manufacturer does [provide 15 Mb zip file](https://485io.com/eletechsup/N4D8B08-1.rar) with a lot of information / demos / software ... etc.

Should that link go dead, a copy is provided in [`mfgr-provided/N4D8B08-1.rar`](./mfgr-provided/N4D8B08-1.rar)

The critical information is in two `docx` files that i've extracted from the zip file so you don't have to download the entire 15 MB zip file just to get the documentation:

- [`N4D8B08 8-channel RS485 IO input and output controller commamd.docx`](mfgr-provided/N4D8B08%208-channel%20RS485%20IO%20input%20and%20output%20controller(en)/N4D8B08%208-channel%20RS485%20IO%20input%20and%20output%20controller%20commamd.docx)

- [`N4D8B08 8-channel RS485 IO input and output controller Manual.docx`](./mfgr-provided/N4D8B08%208-channel%20RS485%20IO%20input%20and%20output%20controller(en)/N4D8B08%208-channel%20RS485%20IO%20input%20and%20output%20controller%20Manual.docx)

I have converted them to PDF for quick reference and to avoid the need for Microsoft Word:

- [Command.pdf](./mfgr-provided/Command.pdf)
- [Manual.pdf](./mfgr-provided/Manual.pdf)

## TL;DR

> [!NOTE]
> ChatGPT extracted the summary. It's probably (mostly) accurate, but I haven't verified it yet.
> If in doubt, consult the PDF/DOCX files above.

- Baud rate: 9600, 8N1
- Slave ID: Matches DIP switches (A0–A5)
  - There are 6 address switches. 0x00 to 0x3F (0 to 63) are valid addresses. I don't know if 0x00 is valid, but I assume it is a broadcast address?

### Read Status (0x03)

| What         | Register Address  | Description                                                 |
| ------------ | ----------------- | ----------------------------------------------------------- |
| Output port  | `0x0001`–`0x0008` | Relay state: `0x0000` = Closed, `0x0001` = Open             |
| Input port   | `0x0081`–`0x0088` | NPN input: `0x0000` = Off, `0x0001` = On                    |
| I/O relation | `0x00FD`          | `0x0000`–`0x0003`                                           |
| Baud rate    | `0x00FE`          | `0:1200`, `1:2400`, `2:4800`, `3:9600 (default)`, `4:19200` |

Example: Read channel 1 state

```bytes
01 03 00 01 00 01 $CRC16
```

### Set Status (0x06)

| Action       | Register          | Data Value        | Description                    |
| ------------ | ----------------- | ----------------- | ------------------------------ |
| Open         | `0x0001`–`0x0008` | `0x01`            | Relay Open                     |
| Close        | `0x0001`–`0x0008` | `0x02`            | Relay Close                    |
| Toggle       | `0x0001`–`0x0008` | `0x03`            | Self-locking                   |
| Latch        | `0x0001`–`0x0008` | `0x04`            | Interlocking                   |
| Momentary    | `0x0001`–`0x0008` | `0x05`            | Non-locking                    |
| Delay        | `0x0001`–`0x0008` | `0x06` + Delay    | Delay 0–255 sec                |
| Open All     | `0x0000`          | `0x07`            | Open all relays                |
| Close All    | `0x0000`          | `0x08`            | Close all relays               |
| I/O relation | `0x00FD`          | `0x0000`–`0x0003` | Relationship                   |
| Baud rate    | `0x00FE`          | `0x0000`–`0x0005` | 0–4 = Speed, 5 = Factory reset |

Example: Open channel 1

```bytes
01 06 00 01 01 00 CRC16
```
