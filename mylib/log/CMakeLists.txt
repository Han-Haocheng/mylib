set(MODEL_NAME log)

file(GLOB_RECURSE MODEL_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
file(GLOB_RECURSE MODEL_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
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

add_library(${MODEL_NAME} STATIC ${MODEL_SOURCES})
target_link_libraries(${MODEL_NAME} PUBLIC core config)


target_link_libraries(${MODEL_NAME} PRIVATE JsonCpp::JsonCpp yaml-cpp::yaml-cpp)

message(STATUS ">>>> [${MODEL_NAME}] create finish.\n\n")
install(FILES ${MODEL_HEADERS} DESTINATION include/${MODEL_NAME})