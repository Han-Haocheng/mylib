
#include"include.h"


TEST(TestThread, base) {
  MYLIB_SPACE::Thread::ptr thteads[5];
  size_t i = 0;
  for (auto &t: thteads) {
    MYLIB_LOG_DEBUG(MYLIB_ROOT_LOGGER)  << i;

    t = std::make_shared<MYLIB_SPACE::Thread>(MYLIB_SPACE::String("thread_") + std::to_string(i),
      []()->void {
      for (size_t j = 0; j < 20; j++) {
        MYLIB_LOG_DEBUG(MYLIB_ROOT_LOGGER) <<"--"<<j;
      }
    });
    i++;
  }

  i++;
}
