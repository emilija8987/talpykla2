cmake_minimum_required(VERSION 3.25)
project(Work_with_Files)

include_directories(Include)
file(GLOB SOURCES "*.cpp")
add_executable(Work_with_Files ${SOURCES})