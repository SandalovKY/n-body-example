cmake_minimum_required(VERSION 3.14)

set(CMAKE_CXX_COMPILER "icx.exe")
set(CMAKE_C_COMPILER "icx.exe")

set(CMAKE_CXX_FLAGS "-Qopt-report:max")

project(Lab1)

add_subdirectory(ver0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)