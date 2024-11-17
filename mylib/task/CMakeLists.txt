set(MODEL_NAME ${PROJECT_NAME}_task)
initialize_module(MODEL_NAME lib)

target_link_libraries(${MODEL_NAME} PUBLIC mylib_core mylib_log)
target_link_libraries(${MODEL_NAME} PRIVATE JsonCpp::JsonCpp yaml-cpp::yaml-cpp)




message(STATUS ">>>> [${MODEL_NAME}] create finish.\n\n")
