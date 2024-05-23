//
// Created by HanHaocheng on 2024/4/21.
//

#include "lock.h"

MYLIB_SPACE_BEGIN

Mutex::Mutex()
    : m_mutex() {
#if _WIN32 == 1
  if ((m_mutex = CreateMutex(NULL, FALSE, NULL)) == NULL) {
    //todo error
  }
#elif linux
  pthread_mutex_init(&m_mutex, nullptr);
#endif// _WIN32==1
}

Mutex::~Mutex() {
#if _WIN32 == 1
  if (CloseHandle(m_mutex)) {
    //todo error
  }
#elif linux
  pthread_mutex_destroy(&m_mutex);
#endif// _WIN32==1
}

void Mutex::lock() {
#if _WIN32 == 1
  WaitForSingleObject(m_mutex, INFINITE);
#elif linux
  pthread_mutex_lock(&m_mutex);
#endif// _WIN32==1
}
void Mutex::unlock() {
#if _WIN32 == 1
  if (ReleaseMutex(m_mutex)) {
    //todo error
  }
#elif linux
  pthread_mutex_unlock(&m_mutex);
#endif// _WIN32==1
}

RWLock::RWLock() : m_rwlock() {
#if _WIN32 == 1

  InitializeSRWLock(&m_rwlock);
#elif linux
  pthread_rwlock_init(&m_rwlock, nullptr);
#endif// _WIN32==1
}

RWLock::~RWLock() {

#if _WIN32 == 1
#elif linux
  pthread_rwlock_destroy(&m_rwlock);
#endif// _WIN32==1
}
void RWLock::rlock() {
#if _WIN32 == 1
  while (!TryAcquireSRWLockShared(&m_rwlock))
#elif linux
  pthread_rwlock_rdlock(&m_rwlock)
#endif// _WIN32==1
    ;
}
void RWLock::wlock() {
#if _WIN32 == 1
  while (!TryAcquireSRWLockExclusive(&m_rwlock))
    ;
#elif linux
  pthread_rwlock_wrlock(&m_rwlock);
#endif// _WIN32==1
}
void RWLock::unlock() {
#if _WIN32 == 1
  ReleaseSRWLockExclusive(&m_rwlock);
  ReleaseSRWLockShared(&m_rwlock);
#elif linux
  pthread_rwlock_unlock(&m_rwlock);
#endif// _WIN32==1
}

CASLock::CASLock() : m_atomic_flag() { m_atomic_flag.clear(); }

void CASLock::lock() {
  while (std::atomic_flag_test_and_set_explicit(&m_atomic_flag, std::memory_order_acquire))
    ;
}
void CASLock::unlock() {
  std::atomic_flag_clear_explicit(&m_atomic_flag, std::memory_order_release);
}
Spinlock::Spinlock() : m_spinlock(0) {

  pthread_spin_init(&m_spinlock, 0);
}
Spinlock::~Spinlock() { pthread_spin_destroy(&m_spinlock); }
void Spinlock::lock() { pthread_spin_lock(&m_spinlock); }
void Spinlock::unlock() { pthread_spin_unlock(&m_spinlock); }

MYLIB_SPACE_END

//读者与写者问题继 读写锁SRWLock
#include <process.h>
#include <stdio.h>
#include <windows.h>
//设置控制台输出颜色
BOOL SetConsoleColor(WORD wAttributes) {
  HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
  if (hConsole == INVALID_HANDLE_VALUE)
    return FALSE;
  return SetConsoleTextAttribute(hConsole, wAttributes);
}

const int READER_NUM = 5;//读者个数

//关键段和事件

CRITICAL_SECTION g_cs;
SRWLOCK g_srwLock;

//读者线程输出函数(变参函数的实现)
void ReaderPrintf(char *pszFormat, ...) {
  va_list pArgList;
  va_start(pArgList, pszFormat);
  EnterCriticalSection(&g_cs);
  vfprintf(stdout, pszFormat, pArgList);
  LeaveCriticalSection(&g_cs);
  va_end(pArgList);
}

//读者线程函数
unsigned int __stdcall ReaderThreadFun(PVOID pM) {
  ReaderPrintf("     编号为%d的读者进入等待中...n", GetCurrentThreadId());
  //读者申请读取文件
  AcquireSRWLockShared(&g_srwLock);
  //读取文件
  ReaderPrintf("编号为%d的读者开始读取文件...n", GetCurrentThreadId());
  Sleep(rand() % 100);
  ReaderPrintf(" 编号为%d的读者结束读取文件n", GetCurrentThreadId());
  //读者结束读取文件
  ReleaseSRWLockShared(&g_srwLock);
  return 0;
}

//写者线程输出函数
void WriterPrintf(char *pszStr) {
  EnterCriticalSection(&g_cs);
  SetConsoleColor(FOREGROUND_GREEN);
  printf("     %sn", pszStr);
  SetConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
  LeaveCriticalSection(&g_cs);
}

//写者线程函数
unsigned int __stdcall WriterThreadFun(PVOID pM) {
  WriterPrintf("写者线程进入等待中...");
  //写者申请写文件
  AcquireSRWLockExclusive(&g_srwLock);
  //写文件
  WriterPrintf("  写者开始写文件.....");
  Sleep(rand() % 100);
  WriterPrintf("  写者结束写文件");
  //标记写者结束写文件
  ReleaseSRWLockExclusive(&g_srwLock);
  return 0;
}

int main() {
  printf("  读者写者问题继 读写锁SRWLockn");
  printf(" -- by MoreWindows( http://blog.csdn.net/MoreWindows ) --nn");
  //初始化读写锁和关键段
  InitializeCriticalSection(&g_cs);
  InitializeSRWLock(&g_srwLock);
  HANDLE hThread[READER_NUM + 1];
  int i;
  //先启动二个读者线程
  for (i = 1; i <= 2; i++)
    hThread[i] = (HANDLE) _beginthreadex(NULL, 0, ReaderThreadFun, NULL, 0, NULL);
  //启动写者线程
  hThread[0] = (HANDLE) _beginthreadex(NULL, 0, WriterThreadFun, NULL, 0, NULL);
  Sleep(50);
  //最后启动其它读者结程
  for (; i <= READER_NUM; i++)
    hThread[i] = (HANDLE) _beginthreadex(NULL, 0, ReaderThreadFun, NULL, 0, NULL);
  WaitForMultipleObjects(READER_NUM + 1, hThread, TRUE, INFINITE);
  for (i = 0; i < READER_NUM + 1; i++)
    CloseHandle(hThread[i]);
  //销毁关键段
  DeleteCriticalSection(&g_cs);
  return 0;
}
