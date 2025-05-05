
file(GLOB SOURCE_V0 *.cpp ../src/GSimulation.cpp)

set(WINDOWS_GUI ON)

project(nbody_gui LANGUAGES CXX)

add_executable(nbody_gui WIN32 "${SOURCE_V0}")

target_include_directories(nbody_gui PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}\\..\\include)

target_compile_options(nbody_gui PRIVATE /ZI /DEBUG /Qopt-report:max)
target_link_libraries(nbody_gui user32 gdi32 opengl32 glu32)

message(STATUS ${CMAKE_CURRENT_SOURCE_DIR})
