#!/usr/bin/env bash
#
# verify.sh — configure, build, and run tests for this one task.
#
# Usage: ./verify.sh
#
# Exits non-zero if the build fails or any test fails. Re-runs are fast:
# cmake/Ninja only rebuild what changed.

set -euo pipefail

cd "$(dirname "${BASH_SOURCE[0]}")"

BUILD_DIR="build"

cmake -S . -B "${BUILD_DIR}" -G Ninja >/dev/null
cmake --build "${BUILD_DIR}"
ctest --test-dir "${BUILD_DIR}" --output-on-failure
