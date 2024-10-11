cmake_minimum_required(VERSION 3.22)

file(TO_CMAKE_PATH "$ENV{VCPKG_ROOT}" VCPKG_ROOT)

set(CMAKE_TOOLCHAIN_FILE "${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")

project(mylib VERSION 0.3.0)

set(CMAKE_SYSTEM_PROCESSOR x64)
set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR})
message(STATUS >>>> CMAKE_INSTALL_PREFIX:${CMAKE_INSTALL_PREFIX})

if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    set(CMAKE_BINARY_DIR "${CMAKE_SOURCE_DIR}/build/Debug")
elseif(${CMAKE_BUILD_TYPE} STREQUAL "Release")
    set(CMAKE_BINARY_DIR "${CMAKE_SOURCE_DIR}/build/Release")
else ()
    message(STATUS >>>> CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE})
endif ()

message(STATUS >>>> CMAKE_BINARY_DIR: ${CMAKE_BINARY_DIR})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib/${CMAKE_BUILD_TYPE})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE})

set(CMAKE_CXX_STANDARD 17)

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4819")
endif()

find_package(boost_lexical_cast CONFIG REQUIRED)
find_package(yaml-cpp CONFIG REQUIRED)
find_package(GTest CONFIG REQUIRED)
find_package(jsoncpp CONFIG REQUIRED)

# add_subdirectory(mylib)
add_subdirectory(tests)

add_subdirectory(core)
add_subdirectory(config)
add_subdirectory(log)
add_subdirectory(thread)
add_subdirectory(schedule)


