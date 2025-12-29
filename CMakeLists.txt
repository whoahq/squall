cmake_minimum_required(VERSION 3.1...3.5)

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

set(WHOA_SFILE_MODE "NATIVE" CACHE STRING "SFile backend (NATIVE or STORMLIB)")
set_property(CACHE WHOA_SFILE_MODE PROPERTY STRINGS NATIVE STORMLIB)

list(APPEND CMAKE_MODULE_PATH
    "${CMAKE_CURRENT_SOURCE_DIR}/lib/system/cmake"
    "${CMAKE_CURRENT_SOURCE_DIR}/vendor/cmake-modules"
)

include(system)

# Standalone builds
if(CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
    set(WHOA_STANDALONE 1)
    message(STATUS "Building Storm as a standalone project")
endif()

if(WHOA_ASSERTIONS_ENABLED)
    add_definitions(-DWHOA_ASSERTIONS_ENABLED)
endif()

# Storm flavors
if(WHOA_STORM_FLAVOR STREQUAL "SC1")
    message(STATUS "Building Storm with StarCraft flavoring")

    if (WHOA_SYSTEM_MAC OR WHOA_SYSTEM_LINUX)
        add_definitions(-DWHOA_STORM_C_CRIT_SECT_RECURSIVE)
    endif()

    add_definitions(-DWHOA_RECT_USES_SCREEN_COORDINATES)
    add_definitions(-DWHOA_SSTRHASH64_SUBTRACTS)
    add_definitions(-DWHOA_SUPPORTS_KOREAN_CODEPAGE)
elseif(WHOA_STORM_FLAVOR STREQUAL "WOW")
    message(STATUS "Building Storm with World of Warcraft flavoring")
else()
    message(STATUS "Building Storm with default flavoring")
endif()

# OS defines
if(WHOA_SYSTEM_WIN)
    # Implicit behavior of CriticalSection
    add_definitions(-DWHOA_STORM_C_CRIT_SECT_RECURSIVE)

    # Avoid win32 header hell
    add_compile_definitions(
        NOMINMAX
        WIN32_LEAN_AND_MEAN
    )
endif()

add_subdirectory(lib)
add_subdirectory(vendor)

# Code coverage reporting
if(WHOA_STANDALONE AND CMAKE_BUILD_TYPE STREQUAL "Debug")
    # GCC coverage configuration
    if(CMAKE_COMPILER_IS_GNUCXX)
        # Running coverage on linux+gcc:
        #
        #   sudo apt install lcov
        #   cmake -DCMAKE_BUILD_TYPE=Debug ..
        #   make
        #   make StormCoverage
        #
        # Results in `StormCoverage/index.html`
        include(CodeCoverage)

        setup_target_for_coverage_lcov(
                NAME StormCoverage
                EXECUTABLE StormTest
                DEPENDENCIES StormTest storm
                BASE_DIRECTORY "${PROJECT_SOURCE_DIR}/storm"
                LCOV_ARGS --rc branch_coverage=1
                GENHTML_ARGS --rc genhtml_branch_coverage=1
        )

        message(STATUS "Code coverage reporting enabled")
    endif()
endif()

if(WHOA_TEST_STORMDLL)
    # Doesn't build squall, but still builds the tests to run against a real Storm.dll
    add_definitions(-DWHOA_TEST_STORMDLL)
    if(WHOA_STORMDLL_VERSION)
        add_definitions(-DWHOA_STORMDLL_VERSION=${WHOA_STORMDLL_VERSION})
    else()
        add_definitions(-DWHOA_STORMDLL_VERSION=2016)
    endif()
    add_subdirectory(test/stormdll)
else()
    add_subdirectory(storm)
endif()

if(WHOA_STANDALONE)
    # Add tests when standalone
    add_subdirectory(test)
endif()
