//
// Created by HanHaocheng on 2024/4/21.
//

#include <boost/test/unit_test.hpp>

#include "../mylib/concurrent/thread.h"
#include "../mylib/log/log.h"

BOOST_AUTO_TEST_SUITE(test_concurrent_suite)

BOOST_AUTO_TEST_CASE(test_thread_base) {
  //  MYLIB_SPACE_NAME::Thread::ptr t = std::make_shared<MYLIB_SPACE_NAME::Thread>("test", [=]() {
  //    MYLIB_LOG_DEBUG(MYLIB_LOG_ROOT_LOGGER)
  //        << MYLIB_SPACE_NAME::GetCurrentThreadName();
  //    sleep(1);
  //    MYLIB_LOG_DEBUG(MYLIB_LOG_ROOT_LOGGER) << "test thread end : ";
  //  });
  MYLIB_LOG_DEBUG(MYLIB_LOG_ROOT_LOGGER) << "begin";

  static const size_t s = 50;
  MYLIB_SPACE_NAME::Thread::ptr thread[s];
  int a = 0;
  for (auto &th: thread) {
    th = std::make_shared<MYLIB_SPACE_NAME::Thread>("TestThread", [=]() {
      MYLIB_LOG_DEBUG(MYLIB_LOG_ROOT_LOGGER) << "test thread beg_" << a;
      MYLIB_LOG_DEBUG(MYLIB_LOG_ROOT_LOGGER) << "test thread end_" << a;
    });
    a++;
  }

  MYLIB_LOG_DEBUG(MYLIB_LOG_ROOT_LOGGER) << "end";
}

BOOST_AUTO_TEST_SUITE_END()
