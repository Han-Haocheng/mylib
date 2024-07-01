//
// Created by HanHaocheng on 2024/6/15.
//

#include "exception.h"

#include <utility>

MYLIB_SPACE_BEGIN

ExceptFuncInfo::ExceptFuncInfo(String name, uint64 addr) : name(std::move(name)), addr(addr) {}

bool Exception::CaptureStackBack(std::vector<ExceptFuncInfo> &out, size_t skip, size_t maxStackSize) {
  PVOID *pStackTrace;
  PSYMBOL_INFO pSymbol;
  USHORT frames;
  HANDLE hCurProcess;
  auto dMaxStackSize = (DWORD) maxStackSize;

  hCurProcess = GetCurrentProcess();
  SymInitialize(hCurProcess, nullptr, TRUE);

  pStackTrace = (PVOID *) malloc(dMaxStackSize * sizeof(PVOID));
  if (!pStackTrace) {
    SymCleanup(hCurProcess);
    return false;
  }

  pSymbol = (PSYMBOL_INFO) malloc(sizeof(SYMBOL_INFO) + MAX_SYM_NAME * sizeof(decltype(pSymbol->Name)));

  if (!pSymbol) {
    free(pStackTrace);
    SymCleanup(hCurProcess);
    return false;
  }

  frames = CaptureStackBackTrace(0, dMaxStackSize, pStackTrace, nullptr);
  pSymbol->MaxNameLen = MAX_SYM_NAME;
  pSymbol->SizeOfStruct = sizeof(SYMBOL_INFO);

  for (size_t i = skip; i < frames; i++) {
    DWORD64 displacement = 0;

    if (!SymFromAddr(hCurProcess, (DWORD64) pStackTrace[i], &displacement, pSymbol)) {
      out.emplace_back("unknown function", 0);
      continue;
    }

    String strFuncName = pSymbol->Name;
    out.emplace_back(strFuncName, pSymbol->Address);

    if (strFuncName == "main") break;
  }

  free(pStackTrace);
  free(pSymbol);
  SymCleanup(hCurProcess);
  return true;
}

Exception::Exception(SourceInfo info, String what)
    : std::exception(what.c_str()),
      m_info(std::move(info)),
      m_stack_back() {
  CaptureStackBack(m_stack_back, 1);
}
Exception::~Exception() noexcept = default;

MYLIB_SPACE_END
