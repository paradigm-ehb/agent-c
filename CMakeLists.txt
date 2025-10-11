CMAKE_MINIMUM_REQUIRED(VERSION 3.12)

PROJECT(synf-core)

ADD_EXECUTABLE(main

        source/main.c
        source/lib.c
)

ADD_EXECUTABLE(source/main.c source/lib.c)
TARGET_INCLUDE_DIRECTORIES(main PRIVATE source/lib/osx source/lib/linux)
