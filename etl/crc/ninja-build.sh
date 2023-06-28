#!/bin/bash -e

export LOCAL_DIR=$(pwd)
export ARM_BUILD_DIR=build-arm
export X86_64_BUILD_DIR=build-x86_64
export CLANG_TIDY_WORKING_DIR=build-clang
export CMAKE_ARM_TOOLCHAIN_FILE=../toolchains/arm.cmake
export CMAKE_X86_64_TOOLCHAIN_FILE=../toolchains/x86_64.cmake

# Parameters:
# $1 : build directory
# $2 : toolchain file
# $3 : extra cmake flags (optional)
# $4 : Release | Debug
# $5 : ninja flags
function make_and_build {
	RETURN_DIR=$(pwd)
	mkdir -p $1 && cd $1
	cmake .. -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_TOOLCHAIN_FILE=$2 -GNinja $3 -DCMAKE_BUILD_TYPE=$4
	ninja $5
	cd $RETURN_DIR
}

if [ "$1" == "clang" ]; then
	RETURN_DIR=$(pwd)
	mkdir -p $CLANG_TIDY_WORKING_DIR && cd $CLANG_TIDY_WORKING_DIR
	cmake .. -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_TOOLCHAIN_FILE=$CMAKE_X86_64_TOOLCHAIN_FILE -DCMAKE_CXX_CLANG_TIDY="clang-tidy" -GNinja
	ninja "-j$(expr $(nproc) - 4)"
	cd $RETURN_DIR
	exit 0
fi

if [ "$1" == "arm" ]; then
    make_and_build $ARM_BUILD_DIR $CMAKE_ARM_TOOLCHAIN_FILE " " Release "-j$(expr $(nproc) - 4)"
    exit 0
fi

if [ "$1" == "x86" ]; then
    make_and_build $X86_64_BUILD_DIR $CMAKE_X86_64_TOOLCHAIN_FILE "-DENABLE_TESTS:bool=True" Debug "-j$(expr $(nproc) - 4)"
    exit 0
fi

if [ "$1" == "coverage" ]; then
    # gcovr/gcov does not play nicely with ninja, so revert to traditional cmake/make build
    RETURN_DIR=$(pwd)
    mkdir $X86_64_BUILD_DIR && cd $X86_64_BUILD_DIR
    cmake .. -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_TOOLCHAIN_FILE=$CMAKE_X86_64_TOOLCHAIN_FILE -DENABLE_TESTS:bool=True -DCMAKE_BUILD_TYPE=Debug
	make "-j$(expr $(nproc) - 4)"
    make test --directory=./subprojects/Build/havoc
    cd $RETURN_DIR
    exit 0
fi

# No flags given, perform x86 release build
make_and_build $X86_64_BUILD_DIR $CMAKE_X86_64_TOOLCHAIN_FILE " " Release "-j8"