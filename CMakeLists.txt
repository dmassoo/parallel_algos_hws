cmake_minimum_required(VERSION 3.17)
project(parallel_algos_hws)

#find_package(MPI REQUIRED) #make it REQUIRED, if you want
#include_directories(SYSTEM ${MPI_INCLUDE_PATH})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fopenmp")


add_executable(Task0 Task0.cpp)
add_executable(Task1 Task1.cpp)
add_executable(Task2 Task2.cpp)