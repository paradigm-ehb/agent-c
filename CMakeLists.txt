CMAKE_MINIMUM_REQUIRED(VERSION 3.12)

# Set the project name
PROJECT(synf)

# Find curl package
find_package(CURL REQUIRED)

# add the executable
ADD_EXECUTABLE(main

        source/main.c
        source/lib.c
)

TARGET_LINK_LIBRARIES(main PRIVATE CURL::libcurl)
TARGET_INCLUDE_DIRECTORIES(main PRIVATE source/lib/osx source/lib/linux)
    source/main.c
    source/lib.c
)

target_include_directories(main PRIVATE source/lib/osx)
