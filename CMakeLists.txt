cmake_minimum_required(VERSION 2.8)

if (${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(FATAL_ERROR
        "In-source builds not allowed.
        Please make a new directory (called a build directory) and run CMake from there.
        You may need to remove CMakeCache.txt."
    )
endif()

# Project
project(storm)

set(CMAKE_CXX_STANDARD 11)

# Arch defines
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(ARCH_64 1)
elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
    set(ARCH_32 1)
endif()

# OS defines
if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    set(PLATFORM_WIN 1)
    add_definitions(-DPLATFORM_WIN)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set(PLATFORM_LINUX 1)
    add_definitions(-DPLATFORM_LINUX)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(PLATFORM_MAC 1)
    add_definitions(-DPLATFORM_MAC)
endif()

add_subdirectory(storm)
add_subdirectory(test)
