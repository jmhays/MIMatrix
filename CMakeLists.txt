cmake_minimum_required(VERSION 2.8)
project(MIMatrix)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fopenmp -O3")

set(SOURCE_FILES main.cpp MI.h MIUtils.h csvparser.c csvparser.h)
add_executable(MIMatrix ${SOURCE_FILES})