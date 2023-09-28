cmake_minimum_required(VERSION 3.14)

set(CMAKE_CXX_COMPILER "icx-cl.exe")
set(CMAKE_C_COMPILER "icx-cl.exe")

# set(CMAKE_LIBRARY_PATH "D:/Intel/oneAPI/compiler/latest/windows/compiler/lib/intel64_win")

project(Lab1)

add_subdirectory(ver0)
# add_subdirectory(ver1)
# add_subdirectory(ver2)
# add_subdirectory(ver3)
# add_subdirectory(ver4)
# add_subdirectory(ver5)
# add_subdirectory(ver6)
# add_subdirectory(ver7)
# add_subdirectory(ver8)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)