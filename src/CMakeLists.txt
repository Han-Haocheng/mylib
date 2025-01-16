
set(MODEL_NAME ${PROJECT_NAME})
initialize_module(${MODEL_NAME} lib)


target_link_libraries(${MODEL_NAME} PRIVATE
        JsonCpp::JsonCpp yaml-cpp::yaml-cpp DbgHelp Boost::lexical_cast)

message(STATUS ">>>> [${MODEL_NAME}] create finish.\n\n")
