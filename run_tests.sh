#!/bin/bash
CC=clang++
OUT=temp
FLAGS="-I."

if [ ! -d "./$OUT" ]; then
  echo "building out directory for tests"
  mkdir ./$OUT
else
  echo "$OUT does exist."
fi

for file in ./tests/*.cc; do
  if [ ! -e "$file" ]; then
    echo "no .cc files found"
    break
  fi

  BASE=$(basename "$file" .cc)

  $CC "$file" $FLAGS -o "./$OUT/$BASE"

  if [ $? -eq 0 ]; then
    echo "compiled successfully and running... $BASE"
    "./$OUT/$BASE"
  else
    echo "failed to compile $BASE"
  fi
done
