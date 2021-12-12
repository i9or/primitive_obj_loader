cmake_minimum_required(VERSION 3.22.0 FATAL_ERROR)

project(primitive_obj_loader VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

find_package(GLUT REQUIRED)
find_package(OpenGL REQUIRED)

message(STATUS "GLUT: ${GLUT_INCLUDE_DIR}")

include_directories({GLUT_INCLUDE_DIR} {OPENGL_INCLUDE_DIR})

add_executable(primitive_obj_loader main.cpp)

target_link_libraries(primitive_obj_loader GLUT::GLUT OpenGL::GL)
