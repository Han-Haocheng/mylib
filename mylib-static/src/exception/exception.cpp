//
// Created by HanHaocheng on 2024/6/15.
//

#include "exception.h"

MYLIB_SPACE_BEGIN
FunctionInfo::FunctionInfo(const String &name, const uint64 &addr)
    : name(name), addr(addr) {
}

bool Exception::CaptureStackBack(std::vector<FunctionInfo> &out, size_t skip, size_t maxStackSize) {
  PVOID *pStackTrace;
  PSYMBOL_INFO pSymbol;
  USHORT frames;
  HANDLE hCurProcess;
  DWORD dMaxStackSize = (DWORD) maxStackSize;

  hCurProcess = GetCurrentProcess();
  SymInitialize(hCurProcess, nullptr, TRUE);

  if (!(pStackTrace = (PVOID *) malloc(dMaxStackSize * sizeof(PVOID)))) {
    SymCleanup(hCurProcess);
    return false;
  }
  if (!(pSymbol = (PSYMBOL_INFO) malloc(sizeof(SYMBOL_INFO) + MAX_SYM_NAME * sizeof(decltype(pSymbol->Name))))) {
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
    if (strFuncName == "main") {
      break;
    }
  }

  free(pStackTrace);
  free(pSymbol);
  SymCleanup(hCurProcess);
  return true;
}

Exception::Exception(Logger::ptr logger, LogEvent::value_type level, const SourceInfo &info, String what)
    : m_logger(logger ? std::move(logger) : MYLIB_ROOT_LOGGER),
      m_level(level),
      m_info(info),
      m_what(std::move(what)),
      m_stack_back() {
  CaptureStackBack(m_stack_back, 1);
  show_stack();
}

void Exception::show_stack() {
  SString m;
  m << "back:";
  for (auto &info: m_stack_back) {
    m << "\t" << info.addr
      << "\tat " << info.name
      << "\n";
  }
  MYLIB_LOG_BASE(m_level, m_logger, m_info)
      << m_what << "\n"
      << m.str();
}

MYLIB_SPACE_END
