set(MODEL_NAME ${PROJECT_NAME}_tests)
initialize_module(MODEL_NAME exe)

target_link_libraries(${MODEL_NAME} PUBLIC mylib_core mylib_task mylib_config mylib_log)


enable_testing()
find_package(GTest CONFIG REQUIRED)
target_link_libraries(${MODEL_NAME} PRIVATE GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main)
add_test(mylib-tests ${MODEL_NAME})



message(STATUS ">>>> [${MODEL_NAME}] create finish.\n\n")
