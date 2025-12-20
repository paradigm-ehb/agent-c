#!/bin/sh
set -eu

CC=cc

SRC_DIR=source
INC_DIR=include
OUT_DIR=build

SRC_FILES="
$SRC_DIR/agent_resources.c
"

OUT="$OUT_DIR/agent_resources"

CFLAGS="
-std=c99
-xc

-I$INC_DIR

-Wall
-Wextra
-Wpedantic
-Wshadow
-Wconversion
-Wsign-conversion
-Wundef
-Wpointer-arith
-Wcast-align
-Wcast-qual
-Wwrite-strings
-Wswitch-enum
-Wformat=2
-Wstrict-aliasing=2

-Werror=implicit-int
-Werror=incompatible-pointer-types
-Werror=return-type

-D_POSIX_C_SOURCE=200809L
"

mkdir -p "$OUT_DIR"

$CC $CFLAGS $SRC_FILES -o "$OUT"

$OUT
