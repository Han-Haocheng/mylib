//
// Created by HanHaocheng on 2024/6/15.
//

#include "include.h"


TEST(EXCEPTION, capture_stack_back_trace) {
  std::vector<MYLIB_SPACE::ExceptFuncInfo> res;
  MYLIB_SPACE::Exception::CaptureStackBack(res, 0);
  for (auto &info: res) {
    std::cout << info.name << std::endl;
  }
}

static void test_throw() {
  MYLIB_THROW("test_throw");
}

TEST(EXCEPTION, throw_except) {
  try {
    test_throw();
  } catch (const MYLIB_SPACE::Exception &e) {
    std::cout << e.what() << std::endl;
  } catch (...) {
  }
}