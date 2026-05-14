# Sony IR Intervalometer

Arduino-based IR intervalometer for Sony Alpha/NEX cameras.

## Compatible Cameras

All cameras compatible with the **RMT-DSLR2** remote:
- NEX-5T, NEX-6, NEX-7
- a6000, a6300, a6500
- a7, a7R, a7S (and Mark II/III variants)

## Hardware

| Component | Details |
|---|---|
| Arduino | UNO or Nano |
| IR Transmitter | Any 3-pin IR transmitter module (widely available on Amazon) |
| Wiring | VCC → 5V, GND → GND, Signal → **Pin 9** |

<img width="3024" height="2268" alt="Arduino_IR_Intervalometer" src="https://github.com/user-attachments/assets/b919900a-93d3-47a2-a59f-b2f604b6611f" />
<img width="732" height="870" alt="IR_Transmitter" src="https://github.com/user-attachments/assets/f38314b9-d246-41f9-9753-afb856c9bcf7" />

## Dependencies

Install via Arduino IDE Library Manager:
> **IRremote** by shirriff, z3t0, ArminJo (v3+)

## Configuration

Edit the settings block at the top of the sketch:

```cpp
Mode     shootMode    = BULB;   // BULB or SINGLE
uint32_t bulbTime     = 11;     // Exposure time (s) — set 1s above target ~ not necessary
uint32_t intervalTime = 5;      // Wait after shutter close (s)
int      totalShots   = 0;      // 0 = infinite
```

**Older cameras** (e.g. A200, A350) have slower image processing and may need `intervalTime` of 10s or more to avoid missed shots.

## Usage

1. Set camera to **BULB** mode (if using BULB)
2. Enable **Remote Ctrl** in camera menu
3. Point IR transmitter at the **front** of the camera
4. Upload sketch and open Serial Monitor (9600 baud)

## Serial Output Example

```
=== Sony IR Intervalometer ===
Mode     : BULB
Exposure : 11s
Interval : 5s
Shots    : inf
------------------------------
[Shot 1] Shutter open
  Exposing: 11s
  ...
  Shutter close
  Next shot in: 5s
  ...

[Shot 2] Shutter open
  ...
```
