# Camera System Requirements Specification (v12)
**Date:** May 8, 2025

## Requirements Traceability Matrix (v1.2)

| Req ID           | Category     | Description                                                                                                       | Implemented | Implementation / Tests                                  |
|------------------|--------------|-------------------------------------------------------------------------------------------------------------------|:-----------:|---------------------------------------------------------|
| **HW-CAM-001**   | Hardware     | USB-C rechargeable (5 V/2 A), portable                                                                            | ✅          | Physical hardware validated                              |
| **HW-CPU-001**   | Hardware     | Raspberry Pi Zero 2 W or x86_64 desktop                                                                           | ✅          | Platform smoke test                                      |
| **HW-MEM-001**   | Hardware     | ≥ 512 MB RAM on Pi; ≥ 2 GB on desktop                                                                              | ✅          | Verified in CI                                           |
| **HW-STOR-001**  | Hardware     | ≥ 16 GB storage; ≥ 200 MB free for data                                                                            | ✅          | Verified in CI                                           |
| **HW-POW-001**   | Hardware     | USB-C power, 0 → 100 % in ≤ 3 h                                                                                   | ✅          | Measured charge time                                     |
| **HW-ENV-001**   | Hardware     | Operate 0–50 °C, ≤ 80 % humidity                                                                                   | ✅          | Verified in smoke test                                   |
| **HW-REQ-006**   | Quality      | Internal 5 000 mAh battery shall support ≥ 250 captures per charge                                                 | ❌          | –                                                        |
| **SW-FUNC-001**  | Software     | Static mode: RAW/DNG/TIFF → 16-bit grayscale + bright-pixel report                                                 | ✅          | `synapse_shot --mode static`, `test_raw2gray`, `test_analysis` |
| **SW-FUNC-002**  | Software     | Dynamic mode (Pi only): `libcamera-still` capture → same pipeline                                                   | ✅ (Pi)      | `test_ubuntu_dynamic_fail` (desktop), `test_capture` (Pi) |
| **SW-FUNC-003**  | Software     | Heap-based top-K brightest-pixel analysis                                                                         | ✅          | `find_top_k()` module, `test_heap`                       |
| **SW-FUNC-004**  | Software     | CLI: `--mode`, `--input`, `--outdir`, clear errors                                                                 | ✅          | `main.c` parsing, general tests                          |
| **SW-FUNC-005**  | Software     | Join LAN via Wi-Fi and host Node.js web interface                                                                  | ❌          | –                                                        |
| **SW-FUNC-006**  | Software     | Capture 720p JPEG + DNG RAW with rolling-shutter metadata                                                           | ❌          | –                                                        |
| **SW-FUNC-007**  | Software     | Store images + metadata under `/home/pi/synapse-shot/data/`                                                        | ✅          | Filesystem verified                                      |
| **SW-FUNC-008**  | Software     | Onboard LED feedback (1 Hz success, 10 Hz failure)                                                                  | ❌          | –                                                        |
| **SW-FUNC-009**  | Software     | High-speed image capture via libcamera (≥ 30 FPS, RAW DNG burst mode)                                               | ❌          | –                                                        |
| **SW-FUNC-010**  | Software     | RAW→16-bit grayscale conversion using LibRaw + OpenCV                                                              | ❌          | –                                                        |
| **SW-FUNC-011**  | Software     | Thresholding & edge detection (Canny) post-processing                                                              | ❌          | –                                                        |
| **SW-FUNC-012**  | Software     | Bluetooth communication with remote unit (RFCOMM/GATT)                                                             | ❌          | –                                                        |
| **SW-FUNC-013**  | Software     | Background service & multithreading (daemon with worker threads)                                                    | ❌          | –                                                        |
| **SW-FUNC-014**  | Software     | Single-Take mode (BT-triggered snapshot → capture, convert, analyze, metadata, notification)                        | ❌          | –                                                        |
| **SW-FUNC-015**  | Software     | Timelapse mode (configurable interval & RAW sampling ratio via BT)                                                  | ❌          | –                                                        |
| **SW-SEC-001**   | Security     | No auth/encryption for local web UI                                                                                 | ❌          | HTTP-only UI                                             |
| **SW-PERF-001**  | Performance  | RAW→gray conversion ≤ 2 s on Pi Zero 2 W                                                                            | ✅          | Manual benchmarking                                     |
| **SW-PERF-002**  | Performance  | Analysis ≤ 500 ms for 10 MP images                                                                                  | ✅          | Manual benchmarking                                     |
| **SW-ERR-001**   | Reliability  | Graceful error on missing/invalid inputs                                                                            | ✅          | Error paths tested                                       |
| **SW-PLAT-002**  | Reliability  | Run as daemon & recover from transient I/O/camera errors                                                             | ❌          | Daemonization TODO                                       |
| **SW-MNT-001**   | Maintainable | Modular code + unit tests ≥ 90 % coverage                                                                           | ✅          | `scripts/run_tests.sh` covers core modules               |
| **SW-MNT-002**   | Maintainable | Build works on Ubuntu 22.04 & Pi OS without changes                                                                 | ✅          | Verified in both environments                            |
| **REM-PLAT-001** | Remote SW    | nRF52840 dongle with Zephyr RTOS                                                                                     | ❌          | –                                                        |
| **REM-FUNC-001** | Remote SW    | Deep sleep until button press                                                                                       | ❌          | –                                                        |
| **REM-FUNC-002** | Remote SW    | Short vs. long press logic (single vs. burst captures)                                                              | ❌          | –                                                        |
| **REM-FUNC-003** | Remote SW    | Report battery level over BLE                                                                                        | ❌          | –                                                        |
| **REM-FUNC-004** | Remote SW    | Match camera’s LED patterns on dongle’s LED                                                                          | ❌          | –                                                        |
| **REM-FUNC-005** | Remote SW    | OTA firmware updates via BLE (DFU-OTA)                                                                               | ❌          | –                                                        |
| **DFU-MTU-001**  | Guidance     | BLE MTU 247, packet size 128–240 B for DFU                                                                            | ❌          | Documented in spec                                      |
| **SW-FUT-001**   | Future SW    | Thresholding, edge detection, post-processing (Canny)                                                                | –           | Deferred                                                |
| **SW-FUT-002**   | Future SW    | Multithreaded background service (BT, capture, coord threads)                                                        | –           | Deferred                                                |
| **HW-FUT-001**   | Future HW    | Battery monitor HAT for real-time capacity reporting                                                                  | –           | Deferred                                                |
| **HW-FUT-002**   | Future HW    | Onboard LCD/OLED display for status and preview                                                                       | –           | Deferred                                                |
| **HW-FUT-003**   | Future HW    | Custom BLE remote (nRF52840-based) with buttons, LED, OTA                                                            | –           | Deferred                                                |

## Raw Requirements

## 1. Introduction
A self‐contained, battery-powered camera system on Raspberry Pi 2 W (Ubuntu 22.04) paired with an nRF52840-based Bluetooth remote (Zephyr RTOS), that:
- **Camera (“Synapse-Shot”) + "LayLow"**
  - Joins an existing LAN via Wi-Fi
  - Hosts a Node.js web interface for capture control and status
  - Captures 720p images (JPEG + DNG RAW) with rolling-shutter metadata
  - Stores all images and metadata locally under `/home/pi/synapse-shot/data/`
  - Provides visual feedback via an onboard LED
- **Remote (v1.0.0)**
  - Runs on an nRF52840 USB-dongle with Zephyr RTOS and Nordic’s toolchain
  - Ultra-low-power deep sleep until button press
  - Short-press vs. long-press logic for single vs. burst captures
  - Reports its battery level over BLE
  - Matches camera’s LED feedback patterns (using the dongle’s onboard LED)
  - Supports OTA firmware updates via BLE

## 2. Glossary

- **BLE (Bluetooth Low Energy)**  
  A low-power wireless protocol for short-range communication, used for remote control commands, battery reporting, and OTA firmware updates.

- **CLI (Command-Line Interface)**  
  A text-based interface for users to interact with software by typing commands.

- **Canny**  
  An algorithm for edge detection in images that uses gradients and non-maximum suppression.

- **Daemon**  
  A background service process that runs without direct user interaction.

- **DFU-OTA (Over-The-Air Device Firmware Update)**  
  A mechanism, implemented over BLE, that allows new firmware images to be transferred to a device and applied without a physical connection.

- **DHCP (Dynamic Host Configuration Protocol)**  
  The network protocol by which a device automatically obtains its IPv4 address, gateway, and DNS settings from a router.

- **DNG (Digital Negative)**  
  An open RAW image format that preserves full sensor data for advanced post-processing.

- **EJS (Embedded JavaScript)**  
  A templating language that generates HTML markup with plain JavaScript.

- **Express.js**  
  A fast, minimalist web framework for Node.js.

- **GATT (Generic Attribute Profile)**  
  The BLE framework that defines how data is organized into services and characteristics.

- **JSON (JavaScript Object Notation)**  
  A lightweight, human-readable data interchange format.

- **JPEG (Joint Photographic Experts Group)**  
  A lossy image format; baseline JPEG here must be ≥1280×720 px at quality ≥90.

- **LAN (Local Area Network)**  
  The Wi-Fi network the camera joins; enables any browser on the same subnet to access the UI.

- **LibRaw**  
  A C/C++ library for decoding RAW image formats like DNG.

- **libcamera**  
  A Linux camera support library that provides control over camera pipelines and sensors.

- **Milligram**  
  A minimal CSS framework used for styling the web UI.

- **MTU (Maximum Transmission Unit)**  
  The largest size of a single BLE ATT packet.

- **Node.js**  
  A JavaScript runtime built on Chrome’s V8 engine, used to run the server-side web application.

- **OpenCV**  
  An open source computer vision library for image processing tasks like thresholding and edge detection.

- **OS (Operating System)**  
  System software such as Ubuntu 22.04 that manages hardware and software resources.

- **OTA-DFU**  
  See DFU-OTA.

- **Pi / Raspberry Pi 2 W**  
  The single-board computer hosting the camera application.

- **POSIX threads (pthreads)**  
  The standard API for creating and managing threads in Unix-like systems.

- **RFCOMM**  
  A Bluetooth Classic protocol emulating serial port communication.

- **RTOS (Real-Time Operating System)**  
  A lightweight OS that guarantees predictable task scheduling and low-power operation (e.g., Zephyr).

- **SMP (Simple Management Protocol)**  
  Zephyr’s management protocol over BLE for block-based DFU image transfers.

- **UI (User Interface)**  
  The web pages served by the camera’s server, providing capture controls and status information.

- **Wi-Fi**  
  A family of wireless networking protocols based on the IEEE 802.11 standards.

- **Zephyr RTOS**  
  A scalable real-time operating system for connected resource-constrained devices.

## 3. Hardware Requirements (Camera)

### HW-REQ-001
- **Category**: Quality
- **Description**: Device shall be fully portable and rechargeable via USB-C.
- **Acceptance Criteria**: Charges 0 → 100 % in ≤ 3 h with a 5 V/2 A USB-C adapter.

### HW-REQ-006
- **Category**: Quality
- **Description**: Internal 5 000 mAh battery shall support ≥ 250 captures per charge.
- **Acceptance Criteria**: 250 full-power cycles (capture + UI serve + idle) before battery ≤ 10 %.

## 4. Software Requirements (Camera)

### SW-FUNC-001
- **Category**: Functional
- **Description**: On “Capture” command from the Synapse-Shot UI, camera shall capture a rolling-shutter image.
- **Acceptance Criteria**: Capture begins ≤ 100 ms after button click; image shows expected rolling-shutter timing.

### SW-FUNC-002
- **Category**: Functional
- **Description**: Device shall join an existing LAN via Wi-Fi and host the Synapse-Shot Node.js web server.
- **Acceptance Criteria**:
  1. Obtains IPv4 via DHCP by default.
  2. HTTP GET `/` returns page with “Capture” button, status, and battery level within ≤ 200 ms.

### SW-FUNC-003
- **Category**: Functional
- **Description**: Camera shall record for each capture:
  1. Shutter speed (exposure time)
  2. Fixed aperture value
  3. Total scan duration + direction (rolling-shutter timing)
- **Acceptance Criteria**: Metadata JSON includes `shutter_speed`, `aperture`, `scan_duration`, `scan_direction`.

### SW-FUNC-004
- **Category**: Functional
- **Description**: Camera shall output two files per shot:
  - **JPEG** (≥ 1280×720 px, quality ≥ 90)
  - **DNG RAW** (same resolution)
- **Acceptance Criteria**: Both files created and timestamp-paired in ≤ 50 ms post-capture, saved under `/home/pi/synapse-shot/data/captures/`.

### SW-FUNC-005
- **Category**: Functional
- **Description**: All image + metadata storage shall occur locally on the Pi’s filesystem.
- **Acceptance Criteria**: Files persist across reboots; no outbound uploads; metadata JSON saved under `/home/pi/synapse-shot/data/metadata/`.

### SW-FUNC-006
- **Category**: Functional
- **Description**: Synapse-Shot UI shall display:
  - Total captures taken
  - Current battery percentage
- **Acceptance Criteria**: Values update on page reload within ≤ 200 ms; UI uses a dark theme (black background, #ffd1dc accent) and is mobile-first responsive.

### SW-FUNC-007
- **Category**: Functional
- **Description**: On successful capture, device shall blink onboard LED once at 1 Hz; on any failure (capture, write, or serve), blink 10 times at 10 Hz.
- **Acceptance Criteria**: LED follows specified blink rate within 100 ms of the event.

### SW-SEC-001
- **Category**: Security
- **Description**: No encryption or authentication required for local web UI.
- **Acceptance Criteria**: UI serves over HTTP without credentials.

### SW-PLAT-001
- **Category**: Platform
- **Description**: Host OS shall be Ubuntu 22.04 on Raspberry Pi 2 W.
- **Acceptance Criteria**: System boots and runs Node.js, `picamera`, and any C/C++ BLE code without errors.

## 5. Addressing Guidance

- **DHCP (Default)**: Easiest—auto-configures on most routers.
- **Static IP (Optional)**: Reserve a DHCP lease on your LAN’s DHCP server for a fixed address (no Pi-side changes needed).

**Recommendation:** Use DHCP by default; manage fixed addressing via your router.

## 6. Implementation Notes (Camera)

### 6.1 Folder Structure
```
├── build /
├── CMakeLists.txt
├── data
│   ├── captures
│   │   ├── dual.dng
│   │   ├── raw1.dng
│   │   └── tag @ryanbreitkreutz - free raws from @signatureeditsco - DSC07073.dng
│   └── metadata
│       ├── dual_grayscale.png
│       ├── dual_report.txt
│       ├── tag @ryanbreitkreutz - free raws from @signatureeditsco - DSC07073_grayscale.png
│       └── tag @ryanbreitkreutz - free raws from @signatureeditsco - DSC07073_report.txt
├── README.md
├── src
│   ├── camera
│   │   ├── capture.c
│   │   └── capture.h
│   ├── common
│   │   ├── common.c
│   │   └── common.h
│   ├── datastructures
│   │   ├── heap.c
│   │   └── heap.h
│   ├── image-processing
│   │   ├── analysis.c
│   │   ├── analysis.h
│   │   ├── raw2gray.c
│   │   └── raw2gray.h
│   └── main.c
└── tests
    ├── build_run_all_tests.sh
    ├── test_analysis.c
    ├── test_capture.c
    ├── test_common.c
    ├── test_heap.c
    ├── test_png_write.c
    ├── test_raw2gray.c
    └── test_ubuntu_dynamic_fail.c
```

### 6.2 Web UI Stack
- **Server**: Express.js
- **Templating**: EJS
- **CSS Framework**: Milligram (override for black background & #ffd1dc accents)

### 6.3 Network & UI
- **Join existing LAN** via Wi-Fi, obtain IPv4 via DHCP.
- **UI page (`/`)** shows:
  - “Capture” button (POST `/capture`)
  - Status (“Idle” / “Capturing”)
  - Battery level (%)
- **Responsive design**: mobile-first, dark theme.

### 6.4 LED Feedback Patterns
- **Success**: 1 × slow blink at 1 Hz
- **Failure**: 10 × rapid blinks at 10 Hz

### 6.5 Error Logging
- All server-side errors append to timestamped `.log` files in `/home/pi/synapse-shot/logs/`.

## 7. LED & Button Hardware (Remote)

- Uses the **onboard LED** and **user button** on the 949-nRF52840-dongle.

## 8. Remote Unit Software Requirements

### SW-REM-PLAT-001
- **Category**: Platform
- **Description**: Remote shall run on the nRF52840 USB-dongle with Zephyr RTOS and Nordic’s toolchain.
- **Acceptance Criteria**:
  1. Zephyr RTOS image builds and boots on the dongle.
  2. BLE controller initializes without errors.

### SW-REM-FUNC-001
- **Category**: Functional
- **Description**: Short press (< 600 ms) wakes the device, debounces input for ≥ 650 ms, establishes BLE, sends one “capture” command.
- **Acceptance Criteria**:
  1. Debounce window of 650 ms.
  2. BLE connection + write complete in ≤ 200 ms after wake.

### SW-REM-FUNC-002
- **Category**: Functional
- **Description**: Long press (≥ 2 s) wakes the device, debounces, then sends three “capture” commands spaced 1 s apart.
- **Acceptance Criteria**: Three BLE writes at 1 s intervals, all ACK’ed.

### SW-REM-FUNC-003
- **Category**: Functional
- **Description**: Remote shall blink its onboard LED using the same patterns defined for the camera (1 Hz success, 10 Hz failure).
- **Acceptance Criteria**: LED follows pattern within 100 ms of each event.

### SW-REM-FUNC-004
- **Category**: Functional
- **Description**: Remote shall expose its battery voltage (and percentage) via a BLE GATT characteristic.
- **Acceptance Criteria**:
  1. Battery characteristic readable; returns voltage in mV and percentage (0–100%).
  2. Supported starting in firmware v1.0.0.

### SW-REM-FUNC-005
- **Category**: Functional
- **Description**: Support OTA-DFU via BLE using Zephyr’s DFU subsystem.
- **Acceptance Criteria**:
  1. DFU GATT service advertises on request.
  2. New firmware image (≤ 1 MB) can be transferred, applied, and rebooted.

### SW-REM-Q-001
- **Category**: Quality
- **Description**: Remote firmware + Zephyr image shall fit within nRF52840’s flash (≤ 1 MB) and use ≤ 128 KB RAM at runtime; idle current ≤ 10 µA.
- **Acceptance Criteria**:
  1. Image ≤ 1 MB; RAM ≤ 128 KB.
  2. Idle current ≤ 10 µA.

### SW-REM-SEC-001
- **Category**: Security
- **Description**: No BLE pairing or encryption required for now.
- **Acceptance Criteria**: BLE connections, GATT reads/writes, and DFU sessions proceed without security layers.

## 9. DFU Transport & MTU Guidance

- **Recommended ATT MTU**: 247 bytes (max for BLE 4.2/5).
- **DFU Packet Size**: 128–240 bytes per Write Without Response.
- **Rationale**: Larger MTU reduces DFU time.
- **Implementation Note**:
  - Negotiate MTU at connection start.
  - Use Zephyr’s `mcumgr` SMP target.

## 10. Additional Software Requirements

### SW-FUNC-008 – High-Speed Image Capture via libcamera
**Category:** Functional  
**Description:**  
The Raspberry Pi camera system shall support high-speed image capture using the libcamera library (C/C++ API)...  
**Acceptance Criteria:**
1. ≥ 30 FPS at full resolution...
2. Burst of 100 images...
3. Resource cleanup after session.

### SW-FUNC-009 – RAW Image Conversion to 16-bit Grayscale
**Category:** Functional  
**Description:**  
Convert RAW DNG to 16-bit grayscale via LibRaw + OpenCV...  
**Acceptance Criteria:** Under 2 s per image...

### SW-FUNC-010 – Basic Image Post-Processing and Analysis
**Category:** Functional  
**Description:**  
Thresholding and edge detection (Canny) via OpenCV...  
**Acceptance Criteria:** 640×480 in ≤ 100 ms...

### SW-FUNC-011 – Bluetooth Communication with Remote Unit
**Category:** Functional  
**Description:**  
Manage BLE (GATT/RFCOMM) link using BlueZ...  
**Acceptance Criteria:** Pair/connect, command latency ≤ 1 s...

### SW-FUNC-012 – Background Service & Multithreading
**Category:** Functional  
**Description:**  
Daemon with Bluetooth, Capture, and Coordinator threads...  
**Acceptance Criteria:** Threads health checks, auto-restart on failure.

### SW-FUNC-013 – Single-Take Mode (BT-Triggered Snapshot)
**Category:** Functional  
**Description:**  
On “single” BT command: capture → convert → analyze → notify...  
**Acceptance Criteria:** ≤ 3 s end-to-end, error handling.

### SW-FUNC-014 – Timelapse Mode (Configurable Interval & Sampling)
**Category:** Functional  
**Description:**  
Configurable interval `T_full` and RAW every N captures via BT...  
**Acceptance Criteria:** Interval ± 100 ms, RAW ratio exactly N−1 to 1, stop/summary command.
