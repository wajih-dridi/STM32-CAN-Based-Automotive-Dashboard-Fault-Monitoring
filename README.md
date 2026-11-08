# STM32 CAN-Based Automotive Dashboard & Fault Monitoring

![STM32](https://img.shields.io/badge/Platform-STM32F407--Discovery-blue)
![Language](https://img.shields.io/badge/Language-Embedded%20C-green)
![IDE](https://img.shields.io/badge/IDE-STM32CubeIDE-orange)
![Protocol](https://img.shields.io/badge/Protocol-CAN%202.0-red)
![Bitrate](https://img.shields.io/badge/CAN-500%20kbit%2Fs-blueviolet)
![Status](https://img.shields.io/badge/Project-Development-yellow)

## 1. Project Overview

This project implements a CAN-based automotive dashboard and fault monitoring system using an **STM32F407-Discovery development board** and the CAN (Controller Area Network) communication protocol.

The system is designed around two logical CAN nodes. The transmitting node generates automotive-related information, including engine RPM, vehicle speed, engine temperature, and battery voltage, and transmits these parameters over the CAN bus.

The dashboard node receives the CAN messages, extracts and processes the transmitted information, monitors communication status, and detects abnormal conditions such as engine overheating and CAN message timeouts.

USART2 provides a serial communication interface for displaying the monitored vehicle parameters and detected fault conditions through a computer terminal.

The main objective is to develop a modular embedded firmware architecture combining CAN communication, message filtering, data decoding, fault monitoring, UART communication, and real-time automotive parameter visualization.

## 2. Main Features

- CAN 2.0 communication using the STM32F407 CAN1 peripheral.
- CAN communication configured for a bitrate of 500 kbit/s.
- Automotive data transmission and reception between two logical CAN nodes.
- Engine RPM monitoring.
- Vehicle speed monitoring.
- Engine temperature monitoring.
- Battery voltage monitoring.
- CAN message filtering and data decoding.
- CAN communication timeout detection.
- Engine overtemperature detection.
- Fault status monitoring and reporting.
- UART-based dashboard output through USART2.
- Modular Embedded C firmware architecture.
- Hardware abstraction using STM32 HAL libraries.

## 3. Hardware Requirements

| Component | Description |
|---|---|
| Microcontroller | STM32F407VGT6 |
| Development Board | STM32F407-Discovery |
| CAN Transceiver | External CAN physical-layer transceiver |
| CAN Transmitting Node | Second CAN-enabled device or simulated CAN transmitter |
| Communication Bus | CANH and CANL differential bus |
| USB-to-UART Interface | Serial communication with a computer, if required |
| Termination Resistors | 120 Ω CAN bus termination resistors |
| Power Supply | Appropriate regulated power supply |

**Important:** The STM32F407 includes an integrated CAN controller but does not include a CAN physical-layer transceiver. An external CAN transceiver is required to connect the microcontroller to the CANH and CANL bus.

For a two-node physical CAN network, both nodes must be connected through compatible CAN transceivers, with proper bus wiring and termination.

## 4. Software Requirements

- STM32CubeIDE
- STM32CubeMX (integrated into STM32CubeIDE)
- STM32F4 HAL Driver Library
- ST-LINK debugger/programmer
- Serial terminal (PuTTY, Tera Term, or similar)
- CAN monitoring software or a CAN-capable test device, if required

Programming language: **Embedded C**

## 5. System Architecture

The firmware is organized around two main communication nodes and separate software modules for CAN communication, data processing, and fault monitoring.

### A. CAN Transmitting Node

Responsible for generating and transmitting automotive-related information over the CAN bus.

Monitored parameters transmitted by the node include:

- Engine RPM.
- Vehicle speed.
- Engine temperature.
- Battery voltage.

The transmitting node creates CAN data frames according to the configured message identifiers and data format.

Depending on the test setup, this node can be implemented using a separate CAN-enabled device or a simulated CAN transmitter.

### B. CAN Communication Module (`can_bus.c` / `can_bus.h`)

Responsible for managing CAN peripheral initialization and communication.

Main responsibilities:

- CAN peripheral initialization.
- CAN transmission.
- CAN message reception.
- CAN message filtering.
- CAN message identification.
- CAN communication status monitoring.

The module provides the interface between the STM32 application and the CAN communication peripheral.

### C. CAN Protocol Module (`can_protocol.c` / `can_protocol.h`)

Responsible for interpreting the received CAN data frames.

Main responsibilities:

- CAN message identifier management.
- Message identification and decoding.
- Extraction of automotive parameters.
- Conversion of received data into meaningful engineering values.
- Validation of received message information.

The decoded parameters are forwarded to the dashboard application for further processing.

### D. Dashboard Module (`can_dashboard.c` / `can_dashboard.h`)

Responsible for managing the monitored automotive parameters.

Main responsibilities:

- Updating engine RPM.
- Updating vehicle speed.
- Updating engine temperature.
- Updating battery voltage.
- Maintaining the latest received vehicle information.
- Preparing dashboard information for UART transmission.

### E. Fault Monitoring Module (`fault_monitor.c` / `fault_monitor.h`)

Responsible for monitoring communication and vehicle-related conditions.

Main responsibilities:

- CAN message timeout detection.
- Engine overtemperature detection.
- Battery voltage monitoring.
- Communication status monitoring.
- Fault status generation and reporting.

Fault thresholds and timeout periods are configurable according to the implemented application.

### F. Main Application (`main.c`)

The main application handles:

- STM32 peripheral initialization.
- System clock configuration.
- CAN1 initialization.
- UART initialization through USART2.
- CAN message reception and processing.
- Dashboard parameter updates.
- Fault monitoring execution.
- UART dashboard output.

## 6. STM32 Peripheral Configuration

The project uses the following peripheral configuration:

| Peripheral | Function | Configuration |
|---|---|---|
| CAN1 | Automotive communication | 500 kbit/s |
| USART2 | Serial dashboard | 115200 baud |
| GPIO | Peripheral interfacing | Alternate Function / Output |
| SysTick / Timer | Application timing | According to firmware implementation |

### CAN1 Pin Configuration

The STM32F407 CAN1 peripheral uses the following pin assignments:

| STM32 Pin | Peripheral Function | Description |
|---|---|---|
| PD0 | CAN1_RX | CAN data reception |
| PD1 | CAN1_TX | CAN data transmission |

The CAN1 peripheral communicates with the physical CAN bus through an external CAN transceiver.

### USART2 Pin Configuration

USART2 provides the serial interface between the STM32F407 and the computer.

| STM32 Pin | Peripheral Function | Description |
|---|---|---|
| PA2 | USART2_TX | UART transmission |
| PA3 | USART2_RX | UART reception |

### CAN Communication Configuration

The CAN peripheral is configured for Classical CAN communication.

| Parameter | Configuration |
|---|---|
| Peripheral | CAN1 |
| Protocol | CAN 2.0 |
| Bitrate | 500 kbit/s |
| RX Pin | PD0 |
| TX Pin | PD1 |
| Frame Type | Data Frame |
| Data Length | According to message definition |
| Identifier Format | According to configured CAN identifiers |

The CAN bit timing parameters, including prescaler, synchronization jump width, time segment 1, and time segment 2, must be configured according to the actual CAN peripheral clock.

### CAN Message Structure

The dashboard processes received CAN frames according to the application-defined message protocol.

The following table describes the intended automotive information:

| Automotive Parameter | Description | Unit |
|---|---|---|
| Engine RPM | Engine rotational speed | RPM |
| Vehicle Speed | Vehicle movement speed | km/h |
| Engine Temperature | Engine temperature | °C |
| Battery Voltage | Electrical system voltage | V |

The exact CAN identifiers, data-byte positions, scaling factors, and message transmission periods must match the implemented firmware.

## 7. CAN Message Processing

The dashboard node follows a sequence of operations when receiving automotive information:

1. Initialize the CAN1 peripheral.
2. Configure the CAN acceptance filters.
3. Start CAN communication.
4. Wait for incoming CAN messages.
5. Retrieve the received CAN frame.
6. Identify the message using its CAN identifier.
7. Decode the corresponding data bytes.
8. Convert the raw data into meaningful engineering values.
9. Update the corresponding dashboard parameter.
10. Update the communication monitoring information.
11. Forward the processed information to the dashboard and fault monitoring modules.

CAN reception can be implemented using polling or interrupt-driven reception, depending on the firmware configuration.

## 8. Fault Monitoring

The fault monitoring module evaluates the received information and detects abnormal operating conditions.

### A. CAN Communication Timeout

The system monitors the reception of expected CAN messages.

If a message is not received within its configured timeout period, the dashboard identifies a communication timeout condition.

This allows the application to detect missing or interrupted CAN communication.

### B. Engine Overtemperature Detection

The engine temperature is monitored against a configurable threshold.

When the received temperature exceeds the configured limit, the system reports an overtemperature condition.

### C. Battery Voltage Monitoring

The battery voltage parameter can be monitored against configurable minimum and maximum voltage limits.

This allows the dashboard to identify voltage conditions outside the configured operating range.

### Fault Monitoring Summary

| Fault Condition | Detection Method |
|---|---|
| CAN Communication Timeout | Expected message not received within the configured period |
| Engine Overtemperature | Received temperature exceeds the configured threshold |
| Battery Voltage Fault | Voltage outside configured limits |
| CAN Communication Status | Monitoring expected message reception |

The actual thresholds, timeout values, and fault recovery behavior must be documented according to the firmware implementation.

## 9. UART Dashboard Interface

USART2 provides a serial communication interface for displaying the automotive parameters received through the CAN bus.

The dashboard can display:

- Engine RPM.
- Vehicle speed.
- Engine temperature.
- Battery voltage.
- CAN communication status.
- Detected fault conditions.

Example output format:

```text
================================
       AUTOMOTIVE DASHBOARD
================================

Engine RPM       : 2500 RPM
Vehicle Speed    : 80 km/h
Engine Temp      : 92 C
Battery Voltage  : 12.4 V

CAN Status       : ACTIVE
Fault Status     : NORMAL

================================
```

*The values above are illustrative examples of the dashboard format, not measured experimental results.*

The serial interface allows the user to monitor the received automotive parameters and fault conditions through a computer terminal.

A graphical PC dashboard can also be developed as an extension to provide visual gauges, indicators, and real-time parameter updates.

## 10. Repository Structure

```text
STM32-CAN-Automotive-Dashboard/
│
├── Core/
│   ├── Inc/
│   │   ├── main.h
│   │   ├── can_bus.h
│   │   ├── can_protocol.h
│   │   ├── can_dashboard.h
│   │   ├── fault_monitor.h
│   │   └── stm32f4xx_it.h
│   │
│   └── Src/
│       ├── main.c
│       ├── can_bus.c
│       ├── can_protocol.c
│       ├── can_dashboard.c
│       ├── fault_monitor.c
│       ├── stm32f4xx_it.c
│       └── stm32f4xx_hal_msp.c
│
├── Drivers/
│   ├── CMSIS/
│   └── STM32F4xx_HAL_Driver/
│
├── Documentation/
│   ├── Images/
│   ├── CAN_Message_Table.md
│   ├── System_Architecture.png
│   └── Wiring_Diagram.png
│
├── README.md
├── ProjectX2.ioc
├── .project
├── .cproject
└── LICENSE
```

*The structure above is a suggested organization. Retain all STM32CubeIDE-generated files and folders required to build the firmware.*

## 11. How to Build and Run

1. Clone or download this repository.
2. Open STM32CubeIDE.
3. Import the STM32CubeIDE project.
4. Connect the STM32F407-Discovery board using ST-LINK.
5. Open the `.ioc` file and verify the CAN1 peripheral configuration.
6. Verify CAN1 pin assignments: PD0 (RX) and PD1 (TX).
7. Verify the CAN bitrate and bit timing configuration.
8. Connect the external CAN transceiver to the STM32 CAN pins.
9. Connect the CAN transceiver to the CAN bus.
10. Connect the transmitting CAN node or configured CAN simulation source.
11. Configure the serial terminal for USART2 at 115200 baud, 8N1.
12. Build the firmware.
13. Flash the firmware onto the STM32F407-Discovery board.
14. Monitor incoming CAN messages and dashboard information through the serial terminal.
15. Test the communication timeout and fault monitoring functions.

Before powering the CAN network, verify the transceiver pinout, common ground, bus polarity, and termination.

## 12. Important Configuration Notes

Before using the firmware with a real CAN network, verify the following:

- **CAN Bitrate:** Ensure that all CAN nodes use the same bitrate.
- **CAN Transceiver:** Use a compatible external CAN transceiver.
- **CAN Pin Configuration:** Verify PD0 for CAN1_RX and PD1 for CAN1_TX.
- **CAN Filters:** Configure acceptance filters according to the identifiers used by the transmitting node.
- **Message Identifiers:** Ensure that the transmitter and receiver use matching CAN identifiers.
- **Data Encoding:** Verify byte ordering, signal positions, scaling factors, and data lengths.
- **Timeout Monitoring:** Configure appropriate timeout periods for the expected message transmission rates.
- **Fault Thresholds:** Set temperature and voltage thresholds according to the application requirements.
- **Bus Termination:** Use appropriate CAN bus termination at the physical network endpoints.

## 13. Future Improvements

Potential extensions for future versions include:

- Development of a graphical PC dashboard with real-time gauges.
- Real-time vehicle parameter visualization.
- CAN message logging and diagnostic history.
- Additional automotive parameters and CAN identifiers.
- Improved fault classification and diagnostic reporting.
- CAN bus error-state monitoring.
- CAN communication using interrupt-driven reception or DMA where appropriate.
- Integration with FreeRTOS for multitasking applications.
- Expansion into a more complete automotive electronic control unit monitoring system.

## 14. Skills Demonstrated

This project demonstrates practical knowledge of:

- Embedded C programming.
- STM32F407 microcontroller architecture.
- STM32CubeIDE and CubeMX configuration.
- STM32 HAL libraries.
- CAN 2.0 communication protocol.
- CAN message transmission and reception.
- CAN message filtering and decoding.
- Automotive data monitoring.
- UART serial communication.
- Embedded fault detection.
- Communication timeout handling.
- Modular firmware development.
- Real-time embedded applications.

## 15. Author

**Wajih Dridi**
**Embedded Systems / Firmware Development Portfolio**

Developed using STM32F407-Discovery and Embedded C.

---

**Project:** STM32 CAN-Based Automotive Dashboard & Fault Monitoring  
**Microcontroller:** STM32F407VGT6  
**Communication Protocol:** CAN 2.0  
**CAN Bitrate:** 500 kbit/s  
**Development Environment:** STM32CubeIDE  
**Programming Language:** Embedded C
