# StormLib 9.31 - CMake build for integration with squall

set(STORMLIB_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

set(STORMLIB_SOURCES
    ${STORMLIB_SRC_DIR}/FileStream.cpp
    ${STORMLIB_SRC_DIR}/SBaseCommon.cpp
    ${STORMLIB_SRC_DIR}/SBaseDumpData.cpp
    ${STORMLIB_SRC_DIR}/SBaseFileTable.cpp
    ${STORMLIB_SRC_DIR}/SBaseSubTypes.cpp
    ${STORMLIB_SRC_DIR}/SCompression.cpp
    ${STORMLIB_SRC_DIR}/SFileAddFile.cpp
    ${STORMLIB_SRC_DIR}/SFileAttributes.cpp
    ${STORMLIB_SRC_DIR}/SFileCompactArchive.cpp
    ${STORMLIB_SRC_DIR}/SFileCreateArchive.cpp
    ${STORMLIB_SRC_DIR}/SFileExtractFile.cpp
    ${STORMLIB_SRC_DIR}/SFileFindFile.cpp
    ${STORMLIB_SRC_DIR}/SFileGetFileInfo.cpp
    ${STORMLIB_SRC_DIR}/SFileListFile.cpp
    ${STORMLIB_SRC_DIR}/SFileOpenArchive.cpp
    ${STORMLIB_SRC_DIR}/SFileOpenFileEx.cpp
    ${STORMLIB_SRC_DIR}/SFilePatchArchives.cpp
    ${STORMLIB_SRC_DIR}/SFileReadFile.cpp
    ${STORMLIB_SRC_DIR}/SFileVerify.cpp
    ${STORMLIB_SRC_DIR}/SMemUtf8.cpp
)

# Bundled compression libraries (always included)
file(GLOB STORMLIB_ADPCM   ${STORMLIB_SRC_DIR}/adpcm/*.c ${STORMLIB_SRC_DIR}/adpcm/*.cpp)
file(GLOB STORMLIB_HUFFMAN ${STORMLIB_SRC_DIR}/huffman/*.cpp)
file(GLOB STORMLIB_JENKINS ${STORMLIB_SRC_DIR}/jenkins/*.c)
file(GLOB STORMLIB_LZMA    ${STORMLIB_SRC_DIR}/lzma/*.c)
file(GLOB STORMLIB_PKLIB   ${STORMLIB_SRC_DIR}/pklib/*.c)
file(GLOB STORMLIB_SPARSE  ${STORMLIB_SRC_DIR}/sparse/*.cpp)

# Crypto libraries (libtomcrypt/libtommath) - using the wrapper files
set(STORMLIB_CRYPTO
    ${STORMLIB_SRC_DIR}/LibTomCrypt.c
    ${STORMLIB_SRC_DIR}/LibTomMath.c
    ${STORMLIB_SRC_DIR}/LibTomMathDesc.c
)

list(APPEND STORMLIB_SOURCES
    ${STORMLIB_ADPCM}
    ${STORMLIB_HUFFMAN}
    ${STORMLIB_JENKINS}
    ${STORMLIB_LZMA}
    ${STORMLIB_PKLIB}
    ${STORMLIB_SPARSE}
    ${STORMLIB_CRYPTO}
)

# On macOS, StormPort.h defines __SYS_ZLIB and __SYS_BZLIB to use system libraries.
# On other platforms, use the bundled versions.
if(NOT WHOA_SYSTEM_MAC)
    file(GLOB STORMLIB_BZIP2 ${STORMLIB_SRC_DIR}/bzip2/*.c)
    file(GLOB STORMLIB_ZLIB  ${STORMLIB_SRC_DIR}/zlib/*.c)
    list(APPEND STORMLIB_SOURCES ${STORMLIB_BZIP2} ${STORMLIB_ZLIB})
endif()

add_library(stormlib-9.31 STATIC ${STORMLIB_SOURCES})

# define to avoid auto-linking pragmas in StormLib.h
target_compile_definitions(stormlib-9.31
    PRIVATE
        __STORMLIB_SELF__
        _CRT_SECURE_NO_WARNINGS
)

target_include_directories(stormlib-9.31
    PUBLIC
        ${STORMLIB_SRC_DIR}
)

# Platform-specific settings
if(WHOA_SYSTEM_WIN)
    target_link_libraries(stormlib-9.31 PRIVATE wininet)
elseif(WHOA_SYSTEM_MAC)
    # macOS uses system zlib and bzip2 (per StormPort.h __SYS_ZLIB, __SYS_BZLIB)
    find_library(COREFOUNDATION_LIBRARY CoreFoundation)
    find_package(ZLIB REQUIRED)
    find_package(BZip2 REQUIRED)
    target_link_libraries(stormlib-9.31
        PRIVATE
            ${COREFOUNDATION_LIBRARY}
            ZLIB::ZLIB
            BZip2::BZip2
    )
elseif(WHOA_SYSTEM_LINUX)
    # Linux - typically link against system zlib
    target_link_libraries(stormlib-9.31 PRIVATE z)
endif()

# Suppress warnings from third-party code
if(CMAKE_C_COMPILER_ID MATCHES "Clang|GNU")
    target_compile_options(stormlib-9.31 PRIVATE
        -Wno-implicit-function-declaration
        -Wno-deprecated-declarations
        -Wno-unused-parameter
        -Wno-sign-compare
    )
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    target_compile_options(stormlib-9.31 PRIVATE
        -Wno-deprecated-declarations
    )
endif()
