file(GLOB_RECURSE TEST_SOURCES
    "*.cpp"
    "big/*.cpp"
)

if(WHOA_SYSTEM_MAC)
    set_source_files_properties(${TEST_SOURCES}
        PROPERTIES COMPILE_FLAGS "-x objective-c++"
    )

    add_executable(StormTest ${TEST_SOURCES})

    target_link_libraries(StormTest
        PRIVATE
            storm
            "-framework AppKit"
            "-framework Carbon"
            "-framework IOKit"
    )
endif()

if(WHOA_SYSTEM_LINUX OR WHOA_SYSTEM_WIN)
    add_executable(StormTest ${TEST_SOURCES})

    target_link_libraries(StormTest
        PRIVATE
            storm
    )
endif()

target_include_directories(StormTest
    PRIVATE
        ${PROJECT_SOURCE_DIR}
)

install(TARGETS StormTest DESTINATION "bin")
