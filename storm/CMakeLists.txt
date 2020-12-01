file(GLOB STORM_SOURCES
    "*.cpp"
    "hash/*.cpp"
    "string/*.cpp"
    "thread/*.cpp"
)

if(WHOA_PLATFORM_WIN)
    file(GLOB STORM_WIN_SOURCES
        "win/*.cpp"
        "thread/win/*.cpp"
        )
    list(APPEND STORM_SOURCES ${STORM_WIN_SOURCES})
endif()

if(WHOA_PLATFORM_MAC)
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
