set(MODEL_NAME ${PROJECT_NAME}_tests)
initialize_module(${MODEL_NAME} exe)

target_link_libraries(${MODEL_NAME} PRIVATE mylib)


enable_testing()
target_link_libraries(${MODEL_NAME} PUBLIC GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main)
add_test(mylib-tests ${MODEL_NAME})



message(STATUS ">>>> [${MODEL_NAME}] create finish.\n\n")
