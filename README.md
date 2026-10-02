# Temperature-Humidity-Nano
A handy temperature and humidity device that uses a Temperature and Humidity sensor (DHT11) to read and display the appropriate values onto a 16x2 lcd screen.

Hardware: 

- Arduino Nano
- breadboard
- Jumper Wires
- power supply module 
- 10k Potentiometer
- DHT11 Temperature & Humidity Module
- 9v Battery 
- 16x2 LCD screen

---

# ESP32 Version: Automatic Fan Control

`ESP32FanControl/` is an upgraded version of this project that also **turns a fan on when the room gets hot** and off once it cools down. It does this wirelessly through a Wi-Fi smart plug.

An ESP32 replaces the Nano because the Nano has no Wi-Fi.

## How it works

```
DHT11 ──> ESP32 ──Wi-Fi──> smart plug ──> fan
```

- Temp reaches **78°F or higher** → fan turns **ON**
- Temp drops to **75°F or lower** → fan turns **OFF**

The 3°F gap stops the fan from rapidly flipping on and off when the temperature hovers near one value. Change `FAN_ON_TEMP` / `FAN_OFF_TEMP` at the top of the sketch to adjust it.

The LCD shows the temperature on line 1 and the humidity and fan state on line 2 (`Hum:45% Fan:ON`).

## Hardware

- ESP32 dev board (e.g. ESP32-DevKitC / ESP32-WROOM-32)
- DHT11 Temperature & Humidity Module
- 16x2 LCD screen + 10k Potentiometer (same as the Nano version)
- Wi-Fi smart plug: **Shelly** (Gen1, Gen2, or Gen3) or any plug running **Tasmota** firmware
- A regular fan that stays on when you plug it in (fans with only a push-button / remote usually go back to "off" after losing power, so they won't work)
- USB cable / 5V supply for the ESP32

> The smart plug handles the wall power, so you never touch mains wiring. Don't swap it for a bare relay module unless you know how to wire mains power safely.

## Wiring

| Part | Pin | ESP32 pin |
|---|---|---|
| DHT11 | VCC | 3V3 |
| DHT11 | GND | GND |
| DHT11 | DATA | GPIO 4 |
| LCD | RS | GPIO 13 |
| LCD | E | GPIO 14 |
| LCD | D4 / D5 / D6 / D7 | GPIO 25 / 26 / 27 / 33 |
| LCD | VSS, RW, K (backlight −) | GND |
| LCD | VDD, A (backlight +) | 5V / VIN |
| LCD | V0 (contrast) | Potentiometer middle pin (outer pins to 5V and GND) |

RW **must** go to GND so the LCD never sends 5V back into the ESP32 (its pins are 3.3V only).

## Setup

1. In the Arduino IDE, install ESP32 board support: **File → Preferences → Additional boards manager URLs**, add `https://espressif.github.io/arduino-esp32/package_esp32_index.json`, then install **esp32** in the Boards Manager.
2. Install the libraries **DHT sensor library** (Adafruit) and **Adafruit Unified Sensor**. **LiquidCrystal** comes with the IDE.
3. Set up your smart plug with its app and connect it to your Wi-Fi. Find its IP address in the app or your router's device list. Setting your router to always give the plug the same IP keeps it from changing.
4. Copy `ESP32FanControl/secrets.example.h` to `ESP32FanControl/secrets.h` and fill in your Wi-Fi name and password and your plug's IP address. Uncomment the URL lines that match your plug. (`secrets.h` is git-ignored, so your password stays off GitHub.)
5. Select your ESP32 board and port, then upload.
6. Open the Serial Monitor at **115200 baud** to check the readings and see whether the plug responds (`Plug ON -> HTTP 200`).

**Quick test:** set `FAN_ON_TEMP` a degree or two below the current room temperature and upload again. The fan should switch on within a few seconds.
