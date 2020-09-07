file(GLOB STORM_SOURCES "*.cpp")

add_library(storm STATIC
    ${STORM_SOURCES}
)
