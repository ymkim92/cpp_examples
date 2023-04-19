```
$ mkdir build
$ cd build
$ cmake ..
$ cmake "-DCMAKE_BUILD_TYPE=Debug" ..
$ make 
$ ctest -T Test --no-compress-output
$ ctest -T Test --verbose
$ ctest -R EepromTests
```