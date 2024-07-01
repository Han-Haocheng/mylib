#include "include.h"


TEST(TestCaseName, TestName) {
  MYLIB_LOG_ERROR(MYLIB_ROOT_LOGGER) << "error test";
  MYLIB_LOG_WARN(MYLIB_ROOT_LOGGER) << "warn test";

  auto test_logger = MYLIB_LOG_NAME("test");
  MYLIB_LOG_INFO(test_logger) << "ttttt";
}
