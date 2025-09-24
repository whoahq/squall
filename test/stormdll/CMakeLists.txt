
add_library(storm SHARED stormstubs.cpp storm.def)
target_include_directories(storm PRIVATE "../..")
