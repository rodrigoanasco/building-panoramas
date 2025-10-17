cmake_minimum_required(VERSION 3.10)
project(OpenCVExample CXX)

find_package(OpenCV REQUIRED)

add_executable(OpenCVExample src/main.cpp)
target_include_directories(OpenCVExample PRIVATE ${OpenCV_INCLUDE_DIRS})
target_link_libraries(OpenCVExample PRIVATE ${OpenCV_LIBS})
