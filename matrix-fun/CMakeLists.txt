cmake_minimum_required(VERSION 3.28)
project(matrix-fun)

set(CMAKE_CXX_STANDARD 26)

set (CMAKE_CXX_FLAGS  "-march=native -O3")
add_executable(matrix-fun matrix.cpp
        main.cpp
        matrix.h
        Tensor4.cpp
        Tensor4.h
        Utils.h

)

