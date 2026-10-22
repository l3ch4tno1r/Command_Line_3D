cmake_minimum_required(VERSION 3.26.4)
project(Console3D)

set(CMAKE_CXX_STANDARD 20)

add_compile_definitions(UNICODE)

add_subdirectory(Console3D)
add_subdirectory(Console3DSandbox)
add_subdirectory(LCN_Math)
add_subdirectory(LCN_Collisions)
add_subdirectory(Utilities)