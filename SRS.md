# Camera System Requirements Specification (v10)  
**Date:** May 8, 2025

*(Updated to include remote battery reporting and DFU transport guidance)*

## 1. Introduction
A self‐contained, battery-powered camera system on Raspberry Pi 2 W (Ubuntu 22.04) paired with an nRF52840-based Bluetooth remote (Zephyr RTOS), that:  
- **Camera (“Synapse-Shot”)**  
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

- **DFU-OTA (Over-The-Air Device Firmware Update)**  
  A mechanism, implemented over BLE, that allows new firmware images to be transferred to the remote and applied without a physical connection.

- **RTOS (Real-Time Operating System)**  
  A lightweight operating system (in this project, Zephyr) that guarantees predictable task scheduling and low-power operation on the nRF52840.

- **GATT (Generic Attribute Profile)**  
  The BLE framework that defines how data is organized into services and characteristics; used here for “take-photo” commands, battery reporting, and DFU transport.

- **LAN (Local Area Network)**  
  The Wi-Fi network the camera joins; enables any browser on the same subnet to access the Synapse-Shot UI.

- **UI (User Interface)**  
  The web pages served by the camera’s Node.js server, providing capture controls, status indicators, and battery information.

- **DHCP (Dynamic Host Configuration Protocol)**  
  The network protocol by which the camera automatically obtains its IPv4 address, gateway, and DNS settings from a LAN router.

- **JSON (JavaScript Object Notation)**  
  A lightweight, human-readable data interchange format; used to store and exchange exposure metadata (`shutter_speed`, `aperture`, `scan_duration`, `scan_direction`).

- **JPEG (Joint Photographic Experts Group)**  
  A lossy image format; baseline JPEG here must be ≥1280×720 px at quality ≥90 for easy inspection and sharing.

- **DNG (Digital Negative)**  
  An open RAW image format that preserves full sensor data for advanced post-processing; output alongside each JPEG.

- **MTU (Maximum Transmission Unit)**  
  The largest size of a single BLE ATT packet; negotiating up to 247 bytes improves throughput during OTA-DFU transfers.

- **SMP (Simple Management Protocol)**  
  Zephyr’s management protocol over BLE (via the `mcumgr` tool) for block-based DFU image transfers.

- **LED Feedback**  
  The onboard indicator that blinks at defined rates (1 Hz for success, 10 Hz for failure) to signal system status without a UI.

- **Pi / Raspberry Pi 2 W**  
  The single-board computer hosting the camera, Node.js server, and storage; runs Ubuntu 22.04 and the Synapse-Shot application.

- **nRF52840 Dongle**  
  A USB-powered development board with Nordic’s nRF52840 SoC; runs Zephyr RTOS for the remote unit, providing BLE connectivity, button input, LED output, and OTA-DFU capability.

---

## 3. Hardware Requirements (Camera)

### HW-REQ-001  
- **Category**: Quality  
- **Description**: Device shall be fully portable and rechargeable via USB-C.  
- **Acceptance Criteria**: Charges 0 → 100 % in ≤ 3 h with a 5 V/2 A USB-C adapter.

### HW-REQ-006  
- **Category**: Quality  
- **Description**: Internal 5 000 mAh battery shall support ≥ 250 captures per charge.  
- **Acceptance Criteria**: 250 full-power cycles (capture + UI serve + idle) before battery ≤ 10 %.

---

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

---

## 5. Addressing Guidance

- **DHCP (Default)**: Easiest—auto-configures on most routers.  
- **Static IP (Optional)**: Reserve a DHCP lease on your LAN’s DHCP server for a fixed address (no Pi-side changes needed).

**Recommendation:** Use DHCP by default; manage fixed addressing via your router.

---

## 6. Implementation Notes (Camera)

### 6.1 Folder Structure  
/home/pi/synapse-shot/
├── server.js # Node.js entrypoint
├── package.json
├── public/ # static assets (dark-theme CSS via CDN, client JS, logo.png)
│ └── css/
├── views/ # EJS templates
├── data/
│ ├── captures/ # JPEG & DNG output files
│ └── metadata/ # JSON metadata per shot
└── logs/ # HTTP and error logs (timestamped .log files)


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

---

## 7. LED & Button Hardware (Remote)

- Uses the **onboard LED** and **user button** on the 949-nRF52840-dongle  
- No external wiring required; firmware shall reference the dongle’s default GPIO mappings

---

## 8. Remote Unit Software Requirements

### SW-REM-PLAT-001  
- **Category**: Platform  
- **Description**: Remote shall run on the nRF52840 USB-dongle with Zephyr RTOS and Nordic’s toolchain (via USB/JTAG).  
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
- **Rationale**: Allows the Synapse-Shot UI to display remote battery in future releases.  
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

---

## 9. DFU Transport & MTU Guidance

- **Recommended ATT MTU**: 247 bytes (max for BLE 4.2/5) to speed block transfers.  
- **DFU Packet Size**: 128–240 bytes per Write Without Response is a good balance of throughput and reliability.  
- **Rationale**: Larger MTU and packet sizes reduce total DFU time; Zephyr’s default DFU over SMP supports MTU scaling.  
- **Implementation Note**:  
  - Negotiate MTU at connection start.  
  - Use Zephyr’s `mcumgr` SMP target with configurable block size.

---

*End of Specification v10*  