cmake_minimum_required(VERSION 3.1)

if (${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(FATAL_ERROR
        "In-source builds not allowed.
        Please make a new directory (called a build directory) and run CMake from there.
        You may need to remove CMakeCache.txt."
    )
endif()

if(TARGET storm)
    # Guard for use as transitive dependency
    return()
endif()

# Project
project(storm)

set(CMAKE_CXX_STANDARD 11)

# Arch defines
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(WHOA_ARCH_64 1)
elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
    set(WHOA_ARCH_32 1)
endif()

# OS defines
if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    set(WHOA_PLATFORM_WIN 1)
    add_definitions(-DWHOA_PLATFORM_WIN)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set(WHOA_PLATFORM_LINUX 1)
    add_definitions(-DWHOA_PLATFORM_LINUX)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(WHOA_PLATFORM_MAC 1)
    add_definitions(-DWHOA_PLATFORM_MAC)
endif()

add_subdirectory(storm)
add_subdirectory(test)
