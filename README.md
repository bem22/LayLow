![laylowbro png2](https://github.com/user-attachments/assets/0424781e-94b8-422b-a442-c3358c5af60c)

# Synapse Shot

**Synapse Shot** is a lightweight C application for converting raw camera files to 16-bit grayscale images and analyzing them to report the brightest pixels. It supports both processing existing files (static mode) and live capture on Raspberry Pi (dynamic mode).

---

## Features

- **Static Mode**: Process existing `.raw`, `.dng`, or supported raw files from `data/captures/`.  
- **Dynamic Mode** (Raspberry Pi Only): Capture a new raw image via `libcamera-still`, convert, and analyze in one command.  
- **16-bit Grayscale Conversion**: Uses weighted NTSC (0.299 R + 0.587 G + 0.114 B) or simple average.  
- **Brightest-Pixel Analysis**: Heap-based top-K finder that reports pixel coordinates and values.  
- **Cross-Platform Build**: Ubuntu 22.04 desktop (static only) and Raspberry Pi OS (full).

---

## Dependencies

### Ubuntu 22.04 Desktop

```bash
sudo apt update
sudo apt install build-essential cmake pkg-config libraw-dev libpng-dev
```

### Raspberry Pi OS (Zero 2 W)

```bash
sudo apt update
sudo apt install build-essential cmake pkg-config libraw-dev libpng-dev libcamera-tools
```

---

## Build Instructions

```bash
git clone https://github.com/yourorg/synapse-shot.git
cd synapse-shot

mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=RelWithDebInfo
make -j\$(nproc)
```

> Binaries will be in \`build/\`: \`synapse_shot\`

---

## Usage

> All commands assume you’re in the project root (where \`data/\` resides).

### Common Options

```bash
Usage: synapse_shot --mode (static|dynamic) [--input <file>] --outdir <dir>
       synapse_shot --help

  -m, --mode      Mode of operation: 'static' or 'dynamic'
  -i, --input     (static) Path to a raw/DNG file; random pick if omitted
  -o, --outdir    Directory for outputs (will be created if missing)
  -h, --help      Show this help message
```

#### 1. Static Mode

Process existing captures:

```bash
# Random file from data/captures/
./synapse_shot --mode static --outdir data/metadata

# Specific file
./synapse_shot --mode static     --input data/captures/sample.dng     --outdir data/metadata
```

**Outputs**  
- \`data/metadata/<base>_grayscale.png\` — 16-bit grayscale PNG  
- \`data/metadata/<base>_report.txt\` — Lines of \`x y value\` for the brightest pixels  

#### 2. Dynamic Mode (Pi Only)

Capture and process in one step:

```bash
./synapse_shot --mode dynamic --outdir data/metadata
```

**What happens:**  
1. Captures a raw DNG into  
   \`data/captures/capture_<timestamp>.dng\`  
2. Converts to grayscale.  
3. Finds top-50 brightest pixels.  
4. Writes grayscale PNG & report TXT into \`data/metadata/\`.

---

### Viewing Results

- **16-bit PNG**: Use an image viewer that supports 16-bit (e.g. ImageMagick’s \`display\`).  
- **Report**: Plain text; each line: \`x y value\`  

Convert to 8-bit for quick preview:

```bash
convert -depth 16     data/metadata/<base>_grayscale.png     -normalize out8.png
```

---

### Directory Layout

```bash
synapse-shot/
├── CMakeLists.txt
├── data/
│   ├── captures/    # input raw/DNG files
│   └── metadata/    # output PNGs & reports
├── include/         # public headers
├── src/             # source code
├── tests/           # unit & integration tests
├── build/           # out-of-source build
└── README.md
```

---

### Compile & Run Tests

```bash
# raw2gray + heap analysis
gcc -Iinclude tests/test_raw2gray.c src/image-processing/raw2gray.c     -lraw -lpng -lm -o test_raw2gray
./test_raw2gray

gcc -Iinclude tests/test_analysis.c     src/image-processing/analysis.c src/datastructures/heap.c     -o test_analysis
./test_analysis
```

Or run the provided script:

```bash
chmod +x scripts/run_tests.sh
./scripts/run_tests.sh
```

---

## The LayLow Hardware

“Laylow” is the custom carrier board and enclosure that transform a Raspberry Pi Zero 2 W into a self-contained camera unit.

### Bill of Materials

| Qty | Item                                            | Notes                                        |
|-----|-------------------------------------------------|----------------------------------------------|
| 1   | Raspberry Pi Zero 2 W                           | SBC core                                     |
| 1   | LiPo battery (IKEA VARMFRONT 5200 mAh)          | Power source                                 |
| 1   | BMS board (IKEA VARMFRONT 5200 mAh)             | Integrated charger & protection              |
| 1   | Battery-to-BMS cable                            | JST connector                                |
| 1   | Raspberry Pi Camera V2 (Sony IMX219, 8 MP)      | CSI-2 module                                 |
| 1   | Camera ribbon cable                             | 15-pin FFC                                   |
| 1   | Enclosure top piece                             | 3D-printed PLA                               |
| 1   | Enclosure bottom piece                          | 3D-printed PLA                               |
| 1   | Enclosure shell                                 | Snap-fit assembly                            |
| 1   | microSD card (≥ 16 GB)                          | OS & data storage                            |

### Key Interfaces

- **Power**: USB-C input → BMS → LiPo → boost converter → 5 V rail  
- **Camera**: CSI ribbon → Pi camera connector  
- **Status LED**: GPIO → transistor → LED for success/failure feedback  
- **User Button**: GPIO → tactile switch for manual capture trigger  
- **USB-OTG**: Pi USB-C port for serial debug or data access  

---

## Troubleshooting

- **“failed to load RAW data”** → Verify `--input` path and file extension.  
- **Dynamic mode errors** → Ensure `libcamera-tools` is installed and camera enabled (`/boot/config.txt` has `start_x=1`).  
- **No camera detected** → Re-seat ribbon cable; check `vcgencmd get_camera`.  

---

## License

MIT License. See [LICENSE](LICENSE) for details.
