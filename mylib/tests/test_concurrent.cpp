
#include"include.h"

#include <string>
#include <string_view>

struct a {
  enum Enum
  {
    aaa,
    sss,
    ccc
  };
  static constexpr const char *a_str = "aaa, sss, ccc";

  constexpr static std::string_view c_str(int t) {
    std::string_view res{a_str};
    size_t i = 0;
    size_t sub_beg = 0;
    size_t split_i = 0;

    while (i < res.length()) {
      if (res[i] == ',') {
        if (split_i == t - 1) {
          sub_beg = i + 1;
        }
        split_i++;
      }
      if (split_i > t) {
        break;
      }
      i++;
    }

    size_t sub_size = i - sub_beg;
    if (sub_beg < res.length() && res[sub_beg] == ' ') {
      sub_beg++;
      sub_size--;
    }

    while (sub_size > 0 && res[sub_beg + sub_size - 1] == ' ') {
      sub_size--;
    }

    return res.substr(sub_beg, sub_size);
  }
};

TEST(TestBase, EnumToCStr) {
  try {
    std::stringstream ss;
    ss<< a::c_str(1);
    std::string rt = ss.str();
    
    std::cout << "========================================" << ss.str() << std::endl;
    size_t a = 0;
  } catch (const std::exception &e) {
    std::cout << e.what() << std::endl;
  }  catch (...) {
    std::cout << "error" << std::endl;

  }
}

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
