#include "stream.h"

#include "../base/mylib_def.h"

#include <iostream>
MYLIB_BEGIN

class ConsoleStream {
public:
  ConsoleStream &operator<<(const std::ios &s){};

private:
  std::iostream m_ios;
};

class FileStream {
};

MYLIB_END