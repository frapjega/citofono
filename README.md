# Citofono Smart (ESP32-S3 + Telegram)

A smart intercom solution built with **ESP32-S3** that allows you to control your home's door or gate remotely via a **Telegram Bot**. It bridges traditional hardware with modern connectivity, providing a secure and silent way to manage access.

## Features
- **Remote Access:** Open your door from anywhere using a simple Telegram command.
- **Solid State Relay (SSR):** Uses an SSR for silent, fast, and high-durability switching (no mechanical wear).
- **Role-Based Access Control:** Separate permissions for `admin` (extended menus) and `user`.
- **Selective Authorization:** The bot interacts with everyone to provide their Chat ID, but **only authorized users** in the whitelist can trigger the relay.
- **Reliability:** Integrated **Watchdog Timer (WDT)** and auto-reconnection logic to ensure 24/7 availability.
- **NTP Time Sync:** Automatic management of Italian Summer/Winter time (CET/CEST) for accurate logging.

---

## Version History

| Version | Status | Key Features Added |
|:---|:---|:---|
| **v1.0.0** | Stable | Initial release with SSR support, User/Admin roles, and Telegram integration. |
| **v1.0.1** | Stable | Unknown. |
| **v1.0.2** | Stable | Added README.md file. |

---

## 🛠 Hardware Specifications
- **Microcontroller:** [ESP32-S3 N16R8](https://www.espressif.com/en/products/socs/esp32-s3) (16MB Flash / 8MB PSRAM).
- **Actuator:** Solid State Relay (SSR) connected to **GPIO 18**.
- **Status Signal:** Built-in LED on **GPIO 2**.

---

## Setup & Installation

### 1. Software Requirements
Ensure you have the following installed in your Arduino IDE:
- **ESP32 Board Manager** (v2.0.x or higher).
- `UniversalTelegramBot` Library.
- `ArduinoJson` Library.

### 2. Configuration (Secrets)
This project uses a separate file for sensitive data. Create a file named `arduino_secrets.h` in your project folder:

```cpp
#define SECRET_SSID "YOUR_WIFI_SSID"
#define SECRET_PASS "YOUR_WIFI_PASSWORD"
#define SECRET_BOT_TOKEN "123456789:ABCDefghIJKL..." // From @BotFather