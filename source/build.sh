#!/bin/sh

set -xe

CC=clang
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"

SOURCE="$SCRIPT_DIR/agent_resources.c"
OUT="$SCRIPT_DIR/agent_resouces"

$CC --std=c99 -g "$SOURCE"  -o "$OUT"

./source/agent_resources
