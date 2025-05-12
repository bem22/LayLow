#!/usr/bin/env bash
set -euo pipefail

ROOT="$(cd "$(dirname "$0")/.." && pwd)"
INCL="-I${ROOT}/include"

# ensure data dirs exist so tests that write metadata won't error
mkdir -p "${ROOT}/data/captures" "${ROOT}/data/metadata"

# now make and cd into build/tests
BUILD="${ROOT}/build/tests"
mkdir -p "$BUILD"
cd "$BUILD"

# Sources
SRC_RAW2GRAY="${ROOT}/src/image-processing/raw2gray.c"
SRC_ANALYSIS="${ROOT}/src/image-processing/analysis.c"
SRC_HEAP="${ROOT}/src/datastructures/heap.c"
SRC_CAPTURE="${ROOT}/src/camera/capture.c"
SRC_COMMON="${ROOT}/src/common/common.c"

TESTDIR="${ROOT}/tests"
BUILD="${ROOT}/build/tests"
mkdir -p "$BUILD"
cd "$BUILD"

echo "=== Building test_raw2gray ==="
gcc ${INCL} \
    "${TESTDIR}/test_raw2gray.c" "${SRC_RAW2GRAY}" \
    -lraw -lpng -lm \
    -o test_raw2gray
echo ""

echo "=== Building test_analysis ==="
gcc ${INCL} \
    "${TESTDIR}/test_analysis.c" "${SRC_ANALYSIS}" "${SRC_RAW2GRAY}" "${SRC_HEAP}" \
    -lraw -lpng -lm \
    -o test_analysis
echo ""

echo "=== Building test_common ==="
gcc ${INCL} \
    "${TESTDIR}/test_common.c" "${SRC_COMMON}" \
    -o test_common
echo ""

echo "=== Building test_heap ==="
gcc ${INCL} \
    "${TESTDIR}/test_heap.c" "${SRC_HEAP}" \
    -o test_heap
echo ""

echo "=== Building test_png_write ==="
gcc ${INCL} \
    "${TESTDIR}/test_png_write.c" "${SRC_RAW2GRAY}" \
    -lraw -lpng -lm \
    -o test_png_write
echo ""

echo "=== Building test_ubuntu_dynamic_fail ==="
gcc ${INCL} \
    "${TESTDIR}/test_ubuntu_dynamic_fail.c" \
    "${SRC_CAPTURE}" "${SRC_COMMON}" "${SRC_RAW2GRAY}" \
    -lraw -lpng -lm \
    -o test_ubuntu_dynamic_fail
echo ""

echo "=== Running test_raw2gray ==="
./test_raw2gray
echo ""

echo "=== Running test_analysis ==="
./test_analysis
echo ""

echo "=== Running test_common ==="
./test_common
echo ""

echo "=== Running test_heap ==="
./test_heap
echo ""

echo "=== Running test_png_write ==="
./test_png_write
echo ""

echo "=== Running test_ubuntu_dynamic_fail ==="
./test_ubuntu_dynamic_fail
echo ""

# Pi-specific capture test
if [[ "$(uname -m)" =~ ^(arm|aarch64) ]] || which libcamera-still >/dev/null 2>&1; then
  echo "=== Building test_capture ==="
  gcc ${INCL} \
      "${TESTDIR}/test_capture.c" "${SRC_CAPTURE}" "${SRC_COMMON}" \
      -o test_capture
  echo ""

  echo "=== Running test_capture ==="
  ./test_capture
  echo ""
else
  echo "--- Skipping test_capture (no libcamera) ---"
  echo ""
fi

echo "=== All tests done ==="
