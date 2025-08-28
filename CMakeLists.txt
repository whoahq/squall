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

list(APPEND CMAKE_MODULE_PATH
    "${CMAKE_CURRENT_SOURCE_DIR}/lib/system/cmake"
    "${CMAKE_CURRENT_SOURCE_DIR}/vendor/cmake-modules"
)

include(system)

# OS defines
if(WHOA_SYSTEM_WIN)
    # Avoid win32 header hell
    add_compile_definitions(
        NOMINMAX
        WIN32_LEAN_AND_MEAN
    )
endif()

add_subdirectory(lib)

if(CMAKE_COMPILER_IS_GNUCXX AND CMAKE_BUILD_TYPE STREQUAL "Debug")
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
endif()

add_subdirectory(storm)
add_subdirectory(test)
