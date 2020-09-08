file(GLOB_RECURSE STORM_SOURCES "*.cpp")

add_library(storm STATIC
    ${STORM_SOURCES}
)

target_include_directories(storm
    PRIVATE
        ${CMAKE_SOURCE_DIR}/storm
)
