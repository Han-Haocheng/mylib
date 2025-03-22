set(MODEL_NAME ${PROJECT_NAME}_tests)

initialize_model(${MODEL_NAME} ".")
add_executable(${MODEL_NAME} ${MODEL_SOURCES} ${MODEL_HEADERS})
install_model(${MODEL_NAME})

target_link_libraries(${MODEL_NAME} PRIVATE mylib)

enable_testing()
target_link_libraries(${MODEL_NAME} PUBLIC GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main)
add_test(mylib-tests ${MODEL_NAME})

