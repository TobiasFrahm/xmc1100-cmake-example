rm -rf build
cmake -B build -GNinja -DCMAKE_VERBOSE_MAKEFILE=ON | tee cmake.log
cmake --build build -v | tee build.log
