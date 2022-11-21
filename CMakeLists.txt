cmake_minimum_required(VERSION 3.16)
project(make-it-stand)
list(PREPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

include(libigl)

igl_include(glfw)

# Add your project files
include_directories("include/")

file(GLOB SRCFILES src/*.cpp)
add_executable(${PROJECT_NAME} "main.cpp")
target_link_libraries(${PROJECT_NAME} PUBLIC igl::glfw)
