//
// Created by HanHaocheng on 2024/4/21.
//

#ifndef MYPROJECT_LOCK_H
#define MYPROJECT_LOCK_H

#include "../core/mylib_def.h"

MYLIB_SPACE_BEGIN

// 特化模板，当T有wr()函数时返回true
template<typename T>
struct check_is_rwlock {
  template<typename>
  using void_t = void;

  template<typename U, typename = decltype(&U::wrlock)>
  constexpr static bool check_wrlock(int) { return true; }
  template<typename U, typename = decltype(&U::rdlock)>
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
  inline explicit ScopedGround(T &lock) : m_lock(lock) { m_lock.rdlock(); }
  inline ~ScopedGround() { m_lock.unlock(); }
  inline void wrlock() { m_lock.wrlock(); }
  inline void rdlock() { m_lock.rdlock(); }
  inline void unlock() { m_lock.unlock(); }

private:
  T m_lock;
};

class Nulllock {
public:
  using Ground = ScopedGround<Nulllock>;

  Nulllock() = default;
  ~Nulllock() = default;
  virtual inline void lock() {}
  virtual inline void unlock() {}
};

/**
 * @brief 互斥锁
 */
class Mutex : Nulllock {
public:
  using Ground = ScopedGround<Mutex>;

  Mutex();
  ~Mutex();
  void lock() final;
  void unlock() final;

private:
  pthread_mutex_t m_mutex;
};

class Spinlock : Nulllock {
public:
  using Ground = ScopedGround<Spinlock>;

  Spinlock();
  ~Spinlock();
  void lock() final;
  void unlock() final;

private:
  pthread_spinlock_t m_spinlock;
};

/**
 * @brief 原子锁
 */
class CASLock : Nulllock {
public:
  using Ground = ScopedGround<CASLock>;

  CASLock();
  ~CASLock() = default;
  void lock() final;
  void unlock() final;

private:
  volatile std::atomic_flag m_atomic_flag;
};

class RWNulllock {
public:
  using Ground = ScopedGround<RWNulllock>;

  RWNulllock() = default;

  ~RWNulllock() = default;

  virtual inline void rdlock() {}
  virtual inline void wrlock() {}
  virtual inline void unlock() {}
};

class RWLock : RWNulllock {
public:
  using Ground = ScopedGround<RWLock>;

  RWLock();

  ~RWLock();

  void rdlock() final;

  void wrlock() final;

  void unlock() final;

private:
  pthread_rwlock_t m_rwlock;
};

MYLIB_SPACE_END

#endif//MYPROJECT_LOCK_H
