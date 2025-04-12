
# ZCT-YLOC1 Continuous Liquid Level Sensor

## I²C Communication Protocol Manual (Translated Summary – V0.0)

---

## 1. Overview

The **ZCT-YLOC1** is a multi-channel capacitive sensor chip used for continuous liquid level detection. It calculates liquid height by analyzing changes in sampling values from different sensing channels.

---

## 2. Features

- Supports **2 sampling channels**
- **Low system cost**
- Simultaneous **PWM** and **I²C** outputs
- Reduced system complexity, improved stability
- Built-in **common-mode interference removal**
- **RoHS-compliant SOP-16** package

---

## 3. Applications

- Water dispensers  
- Coffee machines  
- Industrial devices  
- Home appliances

---

## 4. Pinout Diagram

```
SOP-16 Pin Layout:

VSS   VREG  SEN   CX0   CX1   CX2   CX3   CX4  
CX7   CX6   CX5   PWM   SCL   SDA   PRG   VCC
```

---

## 5. Pin Descriptions

| Pin  | Name    | I/O    | Description                                        |
| ---- | ------- | ------ | -------------------------------------------------- |
| 1    | VSS     | —      | Ground                                             |
| 2    | VREG    | Out    | Internal reference output (use 4.7nF cap)          |
| 3    | SEN     | In/Out | Sensitivity capacitor (10–100pF, recommended 20pF) |
| 4–11 | CX0–CX7 | In/Out | Capacitive sensing inputs (3kΩ series resistors)   |
| 12   | PWM     | Out    | PWM output proportional to liquid level            |
| 13   | SCL     | In     | I²C clock input                                    |
| 14   | SDA     | I/O    | I²C data (with weak pull-up)                       |
| 15   | PRG     | I/O    | Programming pin                                    |
| 16   | VCC     | —      | Power supply input                                 |

---

## 6. Chip Functions

### 6.1 Initialization Time

- 600ms after power-on to calibrate sensing pins

### 6.2 Liquid Level Response Time

- Sampling every 4.8ms  
- Output refresh every 48ms after debounce

### 6.3 Output Logic

- PWM increases with liquid level
- Low PWM duty = low level; High duty = high level
- PWM period: 100µs, Duty range: 0–255

---

## 7. Application Circuit Diagram

- VCC/VSS must be routed independently
- CP, CVREG, CSEN capacitors close to chip
- Series resistors (CX0–CX7) placed near the chip
- Large ground plane recommended
- Keep sensing traces isolated

---

## 8. I²C Interface

### 8.1 Start/Stop Conditions

- **Start (S)**: SDA goes LOW while SCL is HIGH  
- **Stop (P)**: SDA goes HIGH while SCL is HIGH  

### 8.2 Data Validity

- SDA must be stable when SCL is HIGH

### 8.3 Byte Format

- 8 data bits + 1 ACK/NACK

### 8.4 Device Address

| Operation           | Address |
| ------------------- | ------- |
| Device base address | `0x40`  |
| Write               | `0x80`  |
| Read                | `0x81`  |

### 8.5 I²C Operation

#### Write

```
Start → Address (Write) → Register → Data [+MoreData] → Stop
```

#### Read

```
Start → Address (Write) → Register → Stop  
Start → Address (Read) → [Data + ACK...] → Stop
```

### 8.6 Register List

| Name | Address | R/W | Default | Description          |
| ---- | ------- | --- | ------- | -------------------- |
| WL   | `0x00`  | RW  | `0x00`  | Liquid level (0–255) |
| REG1 | `0x01`  | RW  | `0x00`  | Parameter config     |
| REG2 | `0x02`  | RW  | `0x00`  | Parameter config     |
| REG3 | `0x03`  | RW  | `0x00`  | Parameter config     |

---

## 9. PCB Layout Guidelines

- Avoid sharing power traces with other chips
- Place capacitors (CP, CVREG, CSEN) near the chip
- Place series resistors (CX0–CX7) close to the chip
- Use a large ground plane
- Keep sensing traces isolated from power/signal lines

---

## 10. Absolute Ratings

| Parameter      | Value             |
| -------------- | ----------------- |
| Operating Temp | -40°C to +85°C    |
| Storage Temp   | -50°C to +150°C   |
| Supply Voltage | -0.3V to +5.5V    |
| Pin Current    | ±20mA             |
| Pin Voltage    | -0.3V to VCC+0.3V |

---

## 11. Electrical Characteristics (TA = 25°C)

| Parameter         | Symbol  | Min | Typ  | Max | Unit |
| ----------------- | ------- | --- | ---- | --- | ---- |
| Operating Voltage | VCC     | 2.5 | —    | 5.5 | V    |
| Supply Current    | IDD     | —   | 1.09 | —   | mA   |
|                   |         | —   | 570  | —   | µA   |
| Startup Time      | Tini    | —   | 600  | —   | ms   |
| Sampling Period   | Tsi     | —   | 4.8  | —   | ms   |
| Sink/Source Curr. | Isk/Ipl | —   | —    | 10  | mA   |
| Min Delta Cap     | —       | —   | 0.2  | —   | pF   |

---

## 12. ESD Protection

| Model | Polarity | Max Voltage |
| ----- | -------- | ----------- |
| HBM   | ±        | 8kV         |
| MM    | ±        | 600V        |

---

## 13. Package Dimensions (SO-16)

| Symbol | Min (mm) | Nom   | Max   | Min (in) | Nom   | Max   |
| ------ | -------- | ----- | ----- | -------- | ----- | ----- |
| A      | 1.30     | 1.50  | 1.70  | 0.051    | 0.059 | 0.067 |
| A1     | 0.06     | 0.16  | 0.26  | 0.002    | 0.006 | 0.010 |
| b      | 0.30     | 0.40  | 0.55  | 0.012    | 0.016 | 0.022 |
| C      | 0.15     | 0.25  | 0.35  | 0.006    | 0.010 | 0.014 |
| D      | 9.70     | 10.00 | 10.30 | 0.382    | 0.394 | 0.406 |
| E      | 3.75     | 3.95  | 4.15  | 0.148    | 0.156 | 0.163 |
| e      | —        | 1.27  | —     | —        | 0.050 | —     |
| H      | 5.70     | 6.00  | 6.30  | 0.224    | 0.236 | 0.248 |
| L      | 0.45     | 0.65  | 0.85  | 0.018    | 0.026 | 0.033 |
| θ      | 0°       | —     | 8°    | 0°       | —     | 8°    |
