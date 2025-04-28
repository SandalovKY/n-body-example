
file(GLOB SOURCE *.cpp)
add_executable(nbody ${SOURCE})
message(STATUS ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(nbody PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}\\..\\include)
