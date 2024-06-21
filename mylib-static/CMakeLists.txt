set(MODEL_NAME mylib)

file(GLOB_RECURSE MODEL_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*/*.h mylib.h)
file(GLOB_RECURSE MODEL_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*/*.cpp main.cpp)
source_group("header files" FILES ${MODEL_HEADERS})
source_group("source files" FILES ${MODEL_SOURCES})

message(STATUS ">>>> Header files:")
foreach (SOURCE_FILE ${MODEL_HEADERS})
    message(STATUS "\t\t[${SOURCE_FILE}]")
endforeach ()

message(STATUS ">>>> Source files:")
foreach (SOURCE_FILE ${MODEL_SOURCES})
    message(STATUS "\t\t[${SOURCE_FILE}]")
endforeach ()

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_library(${MODEL_NAME}d-static STATIC ${MODEL_SOURCES})
    add_library(${MODEL_NAME}d-dynamic SHARED ${MODEL_SOURCES})
    target_link_libraries(${MODEL_NAME}d-static PUBLIC DbgHelp JsonCpp::JsonCpp yaml-cpp::yaml-cpp)
    target_link_libraries(${MODEL_NAME}d-dynamic PUBLIC DbgHelp JsonCpp::JsonCpp yaml-cpp::yaml-cpp)
elseif (CMAKE_BUILD_TYPE STREQUAL "Release")
    add_library(${MODEL_NAME}-static STATIC ${MODEL_SOURCES})
    add_library(${MODEL_NAME}-dynamic SHARED ${MODEL_SOURCES})
    target_link_libraries(${MODEL_NAME}-static PUBLIC DbgHelp JsonCpp::JsonCpp yaml-cpp::yaml-cpp)
    target_link_libraries(${MODEL_NAME}-dynamic PUBLIC DbgHelp JsonCpp::JsonCpp yaml-cpp::yaml-cpp)
endif ()


message(STATUS ">>>> [${MODEL_NAME}] create finish.\n\n")

