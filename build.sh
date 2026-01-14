#!/bin/bash
set -eu
pushd "$(dirname "$0")" >/dev/null

# Compiler (fixed)
compiler="clang"

# gRPC flags
grpc_c_flags="$(pkg-config --cflags grpc++)"
grpc_lib_flags="$(pkg-config --libs grpc++)"

# Flags
common_flags="-I../src -Wall $grpc_c_flags"
link_flags="-lm $grpc_lib_flags"

# Prep build dir
mkdir -p build
pushd build >/dev/null

# Build
echo "[build] agent"
$compiler $common_flags ../agent/agent_core.cc $link_flags -o agent

# Run
echo "[run] ./agent"
./agent

# Cleanup stack
popd >/dev/null
popd >/dev/null

