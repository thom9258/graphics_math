#!/bin/bash

[[ ! -d build ]] && mkdir build
conan install . --build=missing
cd build
cmake -DCMAKE_TOOLCHAIN_FILE=Release/generators/conan_toolchain.cmake \
	  -DCMAKE_BUILD_TYPE=Release \
	  .. 
