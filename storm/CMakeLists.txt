include(CheckCXXCompilerFlag)
check_cxx_compiler_flag(-Wno-invalid-offsetof HAS_NO_INVALID_OFFSETOF)

file(GLOB STORM_SOURCES
    "*.cpp"
    "hash/*.cpp"
    "string/*.cpp"
    "thread/*.cpp"
)

if(WHOA_SYSTEM_WIN)
    file(GLOB STORM_WIN_SOURCES
        "win/*.cpp"
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

add_library(storm STATIC
    ${STORM_SOURCES}
)

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
