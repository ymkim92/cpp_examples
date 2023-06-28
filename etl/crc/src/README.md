# libhavoc

libhavoc is the collection of software components that manage the chamber servo and ESAF/RCB operation.

libhavoc has been integrated into the [Cerberus Flight Computer](http://git.skybornetech.com:8929/firmware/cv3) and [Standalone HAVOC](http://git.skybornetech.com:8929/firmware/stm32/-/tree/master/standalone-havoc) projects.

## Design
[libhavoc Design Documentation](./design/README.md)

## Building

### Building native debug
```
./ninja-build.sh x86
```

### Building native release
```
./ninja-build.sh
```

### Building nx
```
./yocto-build.sh
```

## Running unit tests
```
./ninja-build.sh x86
cd build-x86_64/subprojects/Build/havoc
ctest --verbose
```

## Generating coverage report
```
./ninja-build.sh coverage
./code_coverage.sh
```