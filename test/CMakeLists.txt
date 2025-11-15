
if(WHOA_TEST_STORMDLL)
    set(TEST_SOURCES
        Big.cpp
        Core.cpp
        Event.cpp
        EventTest.cpp
        Memory.cpp
        Region.cpp
        String.cpp
        Test.cpp
    )

    if(WHOA_STORMDLL_VERSION GREATER_EQUAL 2003)
        list(APPEND TEST_SOURCES Unicode.cpp)
    endif()
else()
    file(GLOB TEST_SOURCES
        "*.cpp"
        "big/*.cpp"
    )
endif()

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

if(CMAKE_COMPILER_IS_GNUCXX AND CMAKE_BUILD_TYPE STREQUAL "Debug")
    include(CodeCoverage)
    target_compile_options(${PROJECT_NAME} PRIVATE -g -fprofile-arcs -ftest-coverage -O0)
endif()

install(TARGETS StormTest DESTINATION "bin")
