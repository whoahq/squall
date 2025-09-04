#define CATCH_CONFIG_MAIN
#include "test/Test.hpp"
#include "storm/Big.hpp"

BigDataTest::BigDataTest() { SBigNew(&num); }
BigDataTest::~BigDataTest() { SBigDel(num); }
