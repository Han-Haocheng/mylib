cmake_minimum_required(VERSION 3.22)

project(mylib)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)

set(CMAKE_CXX_STANDARD 17)

find_package(Boost REQUIRED COMPONENTS unit_test_framework)
find_package(yaml-cpp REQUIRED)
find_package(GTest CONFIG REQUIRED)
find_package(jsoncpp CONFIG REQUIRED)

add_subdirectory(mylib-static)
add_subdirectory(mylib-tests)
