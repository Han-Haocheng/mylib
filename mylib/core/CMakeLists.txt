set(MODEL_NAME ${PROJECT_NAME}_core)
initialize_module(MODEL_NAME lib)

target_link_libraries(${MODEL_NAME} PRIVATE DbgHelp Boost::lexical_cast JsonCpp::JsonCpp yaml-cpp::yaml-cpp)


message(STATUS ">>>> [${MODEL_NAME}] create finish.\n\n")

