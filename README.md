# ESP32-Based Environmental Control and Security Automation System

## Abstract
This project outlines the development of a cooperative multitasking embedded system utilizing the ESP32 microcontroller. The system governs four independent subsystems: environmental cooling, automated irrigation, ambient light regulation, and motion-activated security. By utilizing non-blocking time-tracking (`millis()`) instead of standard delay functions, the microcontroller successfully processes continuous sensor data and actuates multiple relays simultaneously without execution suspension.

## 1. System Architecture
The system integrates analog and digital sensors to monitor environmental parameters, processing the data to control 220V AC and 5V DC loads via relay modules. 

### Subsystems Overview
1.  **Thermal Management:** A DHT11 sensor monitors ambient temperature. If the threshold (≥ 28.0°C) is exceeded, a DC cooling fan is actuated.
2.  **Automated Irrigation:** An analog soil moisture sensor evaluates soil conductivity. The system categorizes moisture into four discrete levels (indicated by LEDs) and triggers a water pump when critical dryness is detected.
3.  **Illumination Control:** A BPW34 photodiode measures ambient light intensity. A drop below the defined analog threshold actuates a relay connected to a 220V AC lighting fixture.
4.  **Security Monitoring:** A Passive Infrared (PIR) sensor detects thermal motion, triggering an audible alarm sequence via a piezoelectric buzzer.

## 2. Hardware Specifications

### Microcontroller
* ESP32 Development Board

### Sensors & Inputs
* **DHT11:** Temperature and Humidity Sensor (Digital)
* **HC-SR501 (PIR):** Motion Sensor (Digital)
* **Analog Soil Moisture Sensor:** (Analog)
* **BPW34 Photodiode:** Light Intensity Sensor (Analog, configured with a pull-down/pull-up resistor network)

### Actuators & Outputs
* 4x 5V Relay Modules (Active High)
* 1x Piezoelectric Buzzer
* 4x Standard LEDs (Red, Yellow, Green, Blue) with 220Ω current-limiting resistors
* 1x 5V DC Mini Water Pump
* 1x 5V DC Cooling Fan
* 1x 220V AC Light Bulb and Fixture

## 3. Crucial Safety and Hardware Protocols

### AC Voltage Warning (220V)
This project involves the switching of mains electricity (220V AC) via a relay module. 
* **Galvanic Isolation:** The relay provides physical isolation between the low-voltage ESP32 logic and the high-voltage mains. 
* **Protocol:** Absolute disconnection from the main power grid is mandatory prior to any physical alteration, wiring modification, or inspection of the relay terminals.

### ESP32 Boot Strapping Constraints
To ensure stable boot sequences, specific GPIO pins (e.g., GPIO 5, 12, and 15) must remain floating or pulled to specific states during startup. Connecting peripheral loads (like relays) to these strapping pins will cause a boot failure or brownout reset. Therefore, safe pins (such as GPIO 22 and 27) were specifically selected for this architecture.

## 4. Pin Mapping and Configuration

| Component | ESP32 GPIO | Type | Power (VCC) | Ground (GND) |
| :--- | :--- | :--- | :--- | :--- |
| DHT11 Sensor | GPIO 4 | Digital Input | 3.3V | GND |
| PIR Sensor | GPIO 25 | Digital Input | 5V (VIN) | GND |
| Soil Moisture | GPIO 32 | Analog Input | 3.3V | GND |
| BPW34 Photodiode | GPIO 34 | Analog Input | - | GND |
| Buzzer | GPIO 33 | Digital Output| - | GND |
| Fan Relay | GPIO 26 | Digital Output| 5V (VIN) | GND |
| Light Relay (220V) | GPIO 27 | Digital Output| 5V (VIN) | GND |
| Pump Relay | GPIO 14 | Digital Output| 5V (VIN) | GND |
| LED 1 (Critically Dry)| GPIO 21 | Digital Output| - | GND |
| LED 2 (Dry) | GPIO 19 | Digital Output| - | GND |
| LED 3 (Moist) | GPIO 22 | Digital Output| - | GND |
| LED 4 (Wet) | GPIO 18 | Digital Output| - | GND |

## 5. Software Implementation (C++)
