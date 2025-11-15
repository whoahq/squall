## Running Tests

### Coverage
Only runnable with GCC.

1. `sudo apt install lcov`
2. `mkdir build`
3. `cd build`
4. `cmake -DCMAKE_BUILD_TYPE=Debug ..`
5. `make StormCoverage`

Open the resulting `html` file for the report.


### Against an original copy of Storm.dll
Only runnable on Windows. Allows running tests against the original `storm.dll` rather than against the recreated code, to test accuracy.

1. `mkdir build`
2. `cd build`
2. `cmake -DWHOA_TEST_STORMDLL=1 -DWHOA_STORM_FLAVOR=SC1 -A Win32 ..`.
3. `cmake --build .`
4. Copy `storm.dll` from your Starcraft 1.17 installation folder to the same directory as `StormTest.exe`.
5. Run `StormTest.exe` from the command line.

To use a different version of Storm.dll, use the following arguments in the cmake command:

| Arguments | Games |
|-----------|-------|
|`-DWHOA_STORMDLL_VERSION=2016`| Starcraft 1.17 |
|`-DWHOA_STORMDLL_VERSION=2007`| Starcraft 1.15.0 ~ 1.16.1 |
|`-DWHOA_STORMDLL_VERSION=2004`| Starcraft 1.11b ~ 1.14 |
|`-DWHOA_STORMDLL_VERSION=2003`| Starcraft 1.10 ~ 1.11, Diablo II 1.10 ~ 1.13d |
|`-DWHOA_STORMDLL_VERSION=2000`| Diablo 1.09, Starcraft 1.08 ~ Starcraft 1.09b, Diablo II 1.06 ~ Diablo II 1.09d, Warcraft II: BNE 2.02.1 |
|`-DWHOA_STORMDLL_VERSION=1999`| Diablo 1.08, Diablo II 1.0 ~ Diablo II 1.05b |
