//
// Created by HanHaocheng on 2024/4/21.
//

#ifndef MYPROJECT_LOCK_H
#define MYPROJECT_LOCK_H

#include "../core/mylib_def.h"
#if _WIN32 == 1
#include <Windows.h>
#elif linux

#endif// _WIN32

MYLIB_SPACE_BEGIN

#if _WIN32 == 1
using mutex_t = HANDLE;
using rwlock_t = SRWLOCK;
using spinlock_t = KSPIN_LOCK;
#elif linux
using mutex_t = pthread_mutex_t;
using rwlock_t = pthread_rwlock_t;
using spinlock_t = pthread_spinlock_t;
#endif// _WIN32 == 1

// 特化模板，当T有wr()函数时返回true
template<typename T>
struct check_is_rwlock {
  template<typename>
  using void_t = void;

  template<typename U, typename = decltype(&U::wlock)>
  constexpr static bool check_wrlock(int) { return true; }
  template<typename U, typename = decltype(&U::rlock)>
  constexpr static bool check_rdlock(int) { return true; }

  template<typename U>
  constexpr static bool check_wrlock(...) { return false; }
  template<typename U>
  constexpr static bool check_rdlock(...) { return false; }

  static constexpr bool value = check_wrlock<T>(0) && check_rdlock<T>(0);
};

template<class T, typename = void>
class ScopedGround {
public:
  inline explicit ScopedGround(T &lock) : m_lock(lock), m_is_locked() { _lock(); }
  inline ~ScopedGround() { _unlock(); }

  inline void lock() {
    if (m_is_locked) return;
    _lock();
  }

  inline void unlock() {
    if (!m_is_locked) return;
    _unlock();
  }

private:
  inline void _lock() {
    m_is_locked = true;
    m_lock.lock();
  }

  inline void _unlock() {
    m_is_locked = false;
    m_lock.unlock();
  }

private:
  bool m_is_locked;
  T m_lock;
};

template<class T>
class ScopedGround<T, typename std::enable_if<check_is_rwlock<T>::value>::type> {
public:
  inline explicit ScopedGround(T &lock) : m_lock(lock) { m_lock.rlock(); }
  inline ~ScopedGround() { m_lock.unlock(); }
  inline void wlock() { m_lock.wlock(); }
  inline void rlock() { m_lock.rlock(); }
  inline void unlock() { m_lock.unlock(); }

private:
  T m_lock;
};

class Lock {
public:
  virtual inline void lock() = 0;
  virtual inline void unlock() = 0;
};

class NullLock : public Lock {
public:
  using Ground = ScopedGround<NullLock>;
  NullLock() = default;
  ~NullLock() = default;
  inline void lock() final {}
  inline void unlock() final {}
};

/**
 * @brief 互斥锁
 */
class Mutex : Lock {
public:
  using Ground = ScopedGround<Mutex>;

  Mutex();
  ~Mutex();
  void lock() final;
  void unlock() final;

private:
  mutex_t m_mutex;
};

class Spinlock : Lock {
public:
  using Ground = ScopedGround<Spinlock>;

  Spinlock();
  ~Spinlock();
  void lock() final;
  void unlock() final;

private:
  spinlock_t m_spinlock;
};

/**
 * @brief 原子锁
 */
class CASLock : Lock {
public:
  using Ground = ScopedGround<CASLock>;

  CASLock();
  ~CASLock() = default;
  void lock() final;
  void unlock() final;

private:
  volatile std::atomic_flag m_atomic_flag;
};

class RWLockInf {
public:
  enum status_t
  {
    RWLS_IDLE = 0x01,
    RWLS_READ = 0x02,
    RWLS_WRITE = 0x04,
  };
  RWLockInf() : m_state(RWLS_IDLE), m_lock_count(0) {}
  virtual void rlock() = 0;
  virtual void wlock() = 0;
  virtual void unlock() = 0;
  uint32 lock_count() const { return m_lock_count; };

protected:
  status_t m_state;
  uint32 m_lock_count;
};

class RWNullLock : public RWLockInf {
public:
  using Ground = ScopedGround<RWNullLock>;
  RWNullLock() = default;
  ~RWNullLock() = default;
  inline void rlock() final { m_state = RWLS_READ; }
  inline void wlock() final { m_state = RWLS_WRITE; }
  inline void unlock() final { m_state = RWLS_IDLE; }
};

class RWLock : public RWLockInf {
public:
  using Ground = ScopedGround<RWLock>;

  RWLock();
  ~RWLock();
  void rlock() final;
  void wlock() final;
  void unlock() final;

private:
  rwlock_t m_rwlock;
};

MYLIB_SPACE_END

#endif//MYPROJECT_LOCK_H
