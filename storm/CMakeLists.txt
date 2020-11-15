file(GLOB STORM_SOURCES
    "*.cpp"
    "hash/*.cpp"
    "string/*.cpp"
    "thread/*.cpp"
)

if(PLATFORM_MAC)
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
