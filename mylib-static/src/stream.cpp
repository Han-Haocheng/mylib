#include "stream.h"

#include "core/mylib_def.h"

#include <iostream>
MYLIB_SPACE_BEGIN

class ConsoleStream {
public:
  ConsoleStream &operator<<(const std::ios &s){};

private:
  std::iostream m_ios;
};

class FileStream {
};

MYLIB_SPACE_END