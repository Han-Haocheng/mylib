#include "include.h"


TEST(TestCaseName, TestName) {
  MYLIB_LOG_ERROR(MYLIB_ROOT_LOGGER) << "error test";
  for (size_t i = 0; i < 1; i++) {
    MYLIB_LOG_WARN(MYLIB_ROOT_LOGGER) << i;
  }
  auto test_logger = MYLIB_LOG_NAME("test");
  MYLIB_LOG_INFO(test_logger) << "ttttt";
}
