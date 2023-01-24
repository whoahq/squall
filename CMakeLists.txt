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

include(lib/system/cmake/system.cmake)

# OS defines
if(WHOA_SYSTEM_WIN)
    # Avoid win32 header hell
    add_compile_definitions(
        NOMINMAX
        WIN32_LEAN_AND_MEAN
    )
endif()

add_subdirectory(lib)
add_subdirectory(storm)
add_subdirectory(test)
