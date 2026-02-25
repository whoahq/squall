# For WoW storm wrapper
add_library(storm STATIC
    addressmap_335a.cpp
    loader.cpp
)
target_include_directories(storm PRIVATE "../..")
