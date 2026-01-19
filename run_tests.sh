#!/bin/bash
CC=clang++
OUT=temp
FLAGS="-I."

if [ ! -d "./$OUT" ]; then
  echo "building out directory for tests"
  mkdir ./$OUT
else
  echo "$OUT found."
fi

for file in ./tests/*.cc; do
  if [ ! -e "$file" ]; then
    echo -e "\e[31mno .cc files fount\e[0m"
    break
  fi

  BASE=$(basename "$file" .cc)

  $CC "$file" $FLAGS -o "./$OUT/$BASE"

  if [ $? -eq 0 ]; then
    echo -e "Compiled successfully and running \e[36m[$BASE]"
    "./$OUT/$BASE"
    if [ $? -eq 0 ]; then
      echo -e "\e[0m\e[42mTests passed successfully!\e[0m"
    fi
  else
   echo -e "\e[31mFailed to compile\e[0m"
  fi
done
