include(CheckCXXCompilerFlag)
check_cxx_compiler_flag(-Wno-invalid-offsetof HAS_NO_INVALID_OFFSETOF)

file(GLOB STORM_SOURCES
    "*.cpp"
    "big/*.cpp"
    "crypto/*.cpp"
    "error/*.cpp"
    "hash/*.cpp"
    "queue/*.cpp"
    "string/*.cpp"
    "thread/*.cpp"
)

if(WHOA_SYSTEM_WIN)
    file(GLOB STORM_WIN_SOURCES
        "win/*.cpp"
        "error/win/*.cpp"
        "thread/win/*.cpp"
    )
    list(APPEND STORM_SOURCES ${STORM_WIN_SOURCES})
endif()

if(WHOA_SYSTEM_MAC)
    file(GLOB STORM_MAC_SOURCES
        "mac/*.cpp"
        "mac/*.mm"
        "thread/mac/*.cpp"
        "thread/mac/*.mm"
    )
    list(APPEND STORM_SOURCES ${STORM_MAC_SOURCES})
endif()

if(WHOA_SYSTEM_LINUX)
    file(GLOB STORM_LINUX_SOURCES
        "linux/*.cpp"
        "thread/linux/*.cpp"
    )
    list(APPEND STORM_SOURCES ${STORM_LINUX_SOURCES})
endif()

if(WHOA_SFILE_MODE STREQUAL "STORMLIB")
    list(APPEND STORM_SOURCES "file/SFile.cpp")
else()
    list(APPEND STORM_SOURCES "file/SFileNative.cpp")
endif()

add_library(storm STATIC
    ${STORM_SOURCES}
)

target_compile_definitions(storm PRIVATE _CRT_SECURE_NO_WARNINGS)
target_compile_definitions(storm PUBLIC WHOA_SFILE_MODE_${WHOA_SFILE_MODE})

target_include_directories(storm
    PUBLIC
        ${PROJECT_SOURCE_DIR}
)

target_link_libraries(storm
    PRIVATE
        system
)

if(HAS_NO_INVALID_OFFSETOF)
    target_compile_options(storm
        PUBLIC
            -Wno-invalid-offsetof
    )
endif()

# Standalone builds
if(WHOA_STANDALONE)
    # Debug build options
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        # GCC debug build options
        if(CMAKE_COMPILER_IS_GNUCXX)
            # Enable coverage reporting
            include(CodeCoverage)
            target_compile_options(${PROJECT_NAME} PRIVATE -g -fprofile-arcs -ftest-coverage -O0)

            # Enable ASan
            target_compile_options(storm PRIVATE -fsanitize=address -fno-omit-frame-pointer)
        endif()
    endif()
endif()
