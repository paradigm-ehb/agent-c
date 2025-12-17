#!/bin/sh

CC=clang
SOURCE=main.c
OUT=main

set -xe

$CC --std=c99 -g "$SOURCE" -lprocps -o "$OUT"
