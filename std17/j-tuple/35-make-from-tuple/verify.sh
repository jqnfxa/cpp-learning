#!/usr/bin/env bash
set -euo pipefail
cd "$(dirname "${BASH_SOURCE[0]}")"
BUILD_DIR="build"
cmake -S . -B "${BUILD_DIR}" -G Ninja >/dev/null
cmake --build "${BUILD_DIR}"
ctest --test-dir "${BUILD_DIR}" --output-on-failure
