# STM32 CAN-Based Automotive Dashboard & Fault Monitoring

An Embedded C project based on the **STM32F407-Discovery** that demonstrates CAN bus communication, automotive data monitoring, message decoding, and fault detection.

The system monitors vehicle-related parameters such as engine RPM, vehicle speed, engine temperature, and battery voltage. Received CAN messages are decoded and processed to display real-time information and identify communication or system faults.

## 1. Project Overview

The objective of this project is to develop an automotive-style monitoring system using the STM32F407 microcontroller and the CAN (Controller Area Network) communication protocol.

The system is organized around two logical CAN nodes:

- **Transmitting Node:** Generates and transmits vehicle-related information over the CAN bus.
- **Dashboard Node:** Receives CAN messages, extracts the monitored parameters, checks their validity, detects communication timeouts, and presents the information through a dashboard interface.

The transmitting node can be implemented using another CAN-enabled device or a simulated data source, depending on the test setup.

## 2. Main Features

- CAN 2.0 communication at a configured bitrate of 500 kbit/s.
- CAN message transmission and reception using STM32 CAN peripherals.
- Decoding of automotive-style CAN data frames.
- Monitoring of engine RPM.
- Monitoring of vehicle speed.
- Engine temperature monitoring.
- Battery voltage monitoring.
- CAN message timeout detection.
- Basic fault monitoring and status reporting.
- UART-based serial dashboard output.
- Modular Embedded C firmware architecture.

## 3. Hardware Requirements

| Component | Purpose |
|---|---|
| STM32F407-Discovery | Main embedded development board |
| CAN transceiver | Physical interface between MCU CAN signals and CANH/CANL |
| CAN transmitter / second CAN node | Generates CAN messages |
| USB-to-UART interface (if required) | Connects serial output to a computer |
| CAN bus wiring | Connects the CAN nodes |
| 120 Ω termination resistors | Bus termination at both physical ends |

**Important:** The STM32F407 contains a CAN controller, not a CAN physical-layer transceiver. An external CAN transceiver is required for connection to the CANH and CANL bus.

## 4. Software Requirements

- STM32CubeIDE
- STM32CubeMX (integrated into STM32CubeIDE)
- STM32 HAL Drivers
- Embedded C
- Serial terminal software for UART monitoring

## 5. System Architecture

The system follows this general communication flow:

```text
        TRANSMITTING NODE
                |
        Vehicle Data Generation
                |
          CAN Frame Creation
                |
                v
=================================
          CAN BUS
        500 kbit/s
=================================
                |
                v
          DASHBOARD NODE
                |
         CAN Reception
                |
         Message Filtering
                |
         Data Decoding
                |
       Fault / Timeout Checks
                |
                v
       Dashboard Data Update
                |
          UART Interface
                |
                v
        PC Serial Dashboard
```

The transmitting and receiving functions may be implemented on separate physical devices or adapted to a simulation setup.

## 6. Monitored Parameters

| Parameter | Description |
|---|---|
| Engine RPM | Engine rotational speed |
| Vehicle Speed | Vehicle speed information |
| Engine Temperature | Engine temperature monitoring |
| Battery Voltage | Vehicle electrical supply monitoring |
| CAN Communication Status | Monitors the reception of expected messages |

The actual data encoding, scaling factors, units, and CAN identifiers must be documented according to the implemented CAN protocol.

## 7. CAN Communication Configuration

| Configuration | Value |
|---|---|
| CAN Peripheral | CAN1 |
| Protocol | CAN 2.0 |
| Configured bitrate | 500 kbit/s |
| CAN RX | PD0 |
| CAN TX | PD1 |
| Frame format | Classical CAN data frames |
| Data length | According to the implemented message format |

An external CAN transceiver is required between the STM32 CAN pins and the physical bus.

### CAN Message Documentation

Each message should be documented with its identifier, data length, signal layout, scaling, and transmission period.

| Message | CAN ID | Data |
|---|---|---|
| Engine data | To be specified | RPM |
| Vehicle data | To be specified | Speed |
| Temperature data | To be specified | Engine temperature |
| Battery data | To be specified | Battery voltage |

*Replace the identifiers and message definitions with the exact values used in your firmware.*

## 8. Fault Monitoring

The dashboard monitors incoming information and checks for abnormal or missing data.

Potential monitoring functions include:

- **CAN Timeout Detection:** Identifies when an expected message has not been received within its configured timeout period.
- **Engine Overtemperature:** Detects engine temperature exceeding the configured threshold.
- **Battery Voltage Monitoring:** Identifies voltage values outside the configured acceptable range.
- **Communication Status:** Indicates whether expected CAN messages are being received.

Fault thresholds, timeout periods, and recovery behavior must be configured and documented according to the implementation.

## 9. Firmware Architecture

The firmware is organized into separate modules to improve readability, maintainability, and code reuse.

| Module | Responsibility |
|---|---|
| `main.c` | Application initialization and main execution flow |
| `can_bus.c/.h` | CAN peripheral initialization, transmission, and reception |
| `can_protocol.c/.h` | CAN identifiers, message encoding, and decoding |
| `can_dashboard.c/.h` | Storage and presentation of monitored vehicle parameters |
| `fault_monitor.c/.h` | Fault conditions and communication timeout monitoring |

The final module names should match the files included in the repository.

## 10. Dashboard Interface

The dashboard presents the received automotive information, including:

- Engine RPM
- Vehicle speed
- Engine temperature
- Battery voltage
- CAN communication status
- Detected fault conditions

The current implementation uses UART serial output for monitoring. A graphical PC dashboard can be added as an extension if implemented.

## 11. Project Structure

```text
STM32-CAN-Automotive-Dashboard/
│
├── Core/
│   ├── Inc/
│   └── Src/
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
└── Linker scripts
```

## 12. How to Build and Run

1. Clone or download this repository.
2. Open STM32CubeIDE.
3. Import the project using **File → Import → Existing Projects into Workspace**.
4. Verify the selected STM32F407 device and project configuration.
5. Check the CAN peripheral configuration in the `.ioc` file.
6. Verify the CAN bitrate and the associated timing parameters.
7. Connect the external CAN transceiver and the CAN bus.
8. Connect the transmitting CAN node.
9. Build the firmware.
10. Flash the STM32F407-Discovery board.
11. Open a serial terminal to monitor dashboard information and fault messages.
12. Test message reception, parameter updates, and timeout behavior.

## 13. Testing and Validation

The following tests can be used to validate the system:

| Test | Expected behavior |
|---|---|
| Valid CAN message reception | Dashboard updates the corresponding parameter |
| RPM message update | Engine RPM value is refreshed |
| Speed message update | Vehicle speed value is refreshed |
| Temperature update | Engine temperature is displayed |
| Battery voltage update | Battery voltage is displayed |
| Missing CAN message | Communication timeout is detected |
| Overtemperature input | Temperature fault is reported when the configured threshold is exceeded |
| Invalid or unexpected message | Message is handled according to the implemented validation rules |

Actual test results and screenshots should be added after testing the firmware.

## 14. Future Improvements

- Develop a graphical PC dashboard with real-time gauges.
- Add additional CAN message identifiers and vehicle parameters.
- Implement more comprehensive fault classification.
- Add CAN bus error-state monitoring.
- Introduce data logging and diagnostic history.
- Extend the application using FreeRTOS for multitasking.

## 15. Skills Demonstrated

- Embedded C programming
- STM32F407 microcontroller development
- STM32CubeIDE and STM32CubeMX
- CAN 2.0 communication
- CAN message encoding and decoding
- STM32 HAL peripheral configuration
- UART serial communication
- Embedded firmware modularization
- Communication timeout detection
- Basic automotive fault monitoring

---

**Project:** STM32 CAN-Based Automotive Dashboard & Fault Monitoring  
**Platform:** STM32F407-Discovery  
**Programming Language:** Embedded C  
**Development Environment:** STM32CubeIDE
