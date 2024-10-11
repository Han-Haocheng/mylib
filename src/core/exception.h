//
// Created by HanHaocheng on 2024/6/15.
//

#ifndef MYLIB_SLN_EXCEPTION_H
#define MYLIB_SLN_EXCEPTION_H

#include "define.h"
#include "algorithm.h"

#include <vector>

//#include "../logger/log.h"

#define MYLIB_THROW(what) \
  throw MYLIB_SPACE::Exception { MYLIB_CURRENT_SOURCE_INFO, what }

MYLIB_BEGIN

struct ExceptFuncInfo {
  String name;
  uint64 addr;

  ExceptFuncInfo();
  explicit ExceptFuncInfo(String name = "", uint64 addr = 0);
};

class Exception : public std::exception {
public:

  /**
   * @brief 捕获堆栈信息
   * @param out 输出的堆栈信息
   * @param skip 跳过的步数
   * @param maxStackSize 堆栈最大大小
   * @return 是否成功
   */
  static bool CaptureStackBack(std::vector<ExceptFuncInfo> &out, size_t skip, size_t maxStackSize = 64);

  explicit Exception(SourceInfo info, String what);
  ~Exception() noexcept override;

  Exception(Exception &&e) noexcept = default;
  Exception(const Exception &e) noexcept = default;

  Exception &operator=(Exception &&e) noexcept = default;
  Exception &operator=(const Exception &e) noexcept = default;

  [[nodiscard]] const std::vector<ExceptFuncInfo> &stackBack() const { return m_stack_back; }
  [[nodiscard]] const SourceInfo &info() const { return m_info; }

  void show() const;

private:
  SourceInfo m_info;
  std::vector<ExceptFuncInfo> m_stack_back;
};

MYLIB_END
#endif//MYLIB_SLN_EXCEPTION_H
