# iot-destovka
ESP8266 project for measuring water level in tank

Code for ESP8266 to calculate water level in a rainwater tank.
Water level is measured using JSN-SR04T sensor in the tank.
The level is shown on an onboard OLED display.
The level is also provided for a remote client over wifi using GET request.

# ESP8266 Rainwater tank measurement project

This project uses an ESP8266 microcontroller to measure water level with an ultrasonic sensor and display the result on an OLED display. It also provides a GET endpoint to retrieve this information over Wi-Fi.

## Hardware Requirements

- ESP8266 microcontroller (NodeMCU)
- Ultrasonic sensor (JSN-SR04T)
- OLED display (0.91" 128x32, I2C interface)

## Wiring Diagram

- Ultrasonic Sensor:
  - Trigger Pin: D6
  - Echo Pin: D5
  
- OLED Display (I2C):
  - SDA Pin: D1
  - SCL Pin: D2
  
- Power source for ESP8266

## Before compilation

The code uses `arduino_secrets.h` file to store Wi-Fi SSID and password. Use `arduino_secrets.h.example` template to create the file with your own SSID and password.

## Usage

1. Once the code is uploaded, the ESP8266 will connect to the configured Wi-Fi network.

2. The OLED display will show the measured distance, Wi-Fi status, and signal strength.

3. You can access the data remotely by making a GET request to the following endpoint: http://destovka.local/get_level
