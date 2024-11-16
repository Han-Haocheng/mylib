#include "algorithm.h"

#include <ctime>


MYLIB_BEGIN

time_t GetCurrentTimestamp() {
  return time(nullptr); 
}

MYLIB_END
