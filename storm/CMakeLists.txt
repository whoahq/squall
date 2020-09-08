file(GLOB_RECURSE STORM_SOURCES "*.cpp")

add_library(storm STATIC
    ${STORM_SOURCES}
)
