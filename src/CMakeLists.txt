set(MODEL_NAME ${PROJECT_NAME})


initialize_model(${MODEL_NAME} ".")
add_library(${MODEL_NAME} STATIC ${MODEL_SOURCES})
install_model(${MODEL_NAME})


target_link_libraries(${MODEL_NAME} PUBLIC JsonCpp::JsonCpp yaml-cpp::yaml-cpp DbgHelp Boost::lexical_cast)

