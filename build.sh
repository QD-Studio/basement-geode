#!/bin/bash

# remember to actually set these..
export SPLAT_DIR=/mnt/C/Dev/GD/linux/splat
export TOOLCHAIN=/mnt/C/Dev/GD/linux/toolchain/clang-cl-msvc.cmake
# this is supposed to default to x86 but it doesnt for some reason
export HOST_ARCH=x86
export GEODE_BINDINGS_REPO_PATH=/mnt/C/Dev/GD/Geode/bindings

cmake --build build_linux --config Release
