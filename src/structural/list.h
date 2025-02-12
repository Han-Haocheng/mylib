#pragma once

#include "iterator.h"
#include <functional>

MYLIB_BEGIN

template<class Ty>
using function = std::function<Ty>;

class List {
public:
  using value_type = int;
  using pointer = value_type *;
  using reference = value_type &;
  using const_reference = const value_type &;

  using size_type = size_t;
  using difference_type = ptrdiff_t;

  using iterator = value_type *;
  using const_iterator = const value_type *;

  virtual ~List() = default;

  virtual reference operator[](size_type pos) = 0;
  virtual const_reference operator[](size_type pos) const = 0;
  virtual reference at(size_type pos) = 0;
  virtual reference front() = 0;
  virtual reference back() = 0;

  virtual bool empty() const = 0;
  virtual size_type size() const = 0;
  virtual void clear() = 0;

  virtual iterator begin() = 0;
  virtual iterator end() = 0;
  virtual const_iterator cbegin() = 0;
  virtual const_iterator cend() = 0;

  virtual void push_back(const_reference) = 0;
  virtual void pop_back() = 0;
};

class SequentialList : public List {
public:
  using value_type = int;
  using pointer = value_type *;
  using reference = value_type &;
  using const_reference = const value_type &;

  using size_type = size_t;
  using difference_type = ptrdiff_t;

  using iterator = value_type *;
  using const_iterator = const value_type *;

  explicit SequentialList(size_t n = 0)
      : m_begin(nullptr),
        m_end(nullptr),
        m_storage_end(nullptr) {
    if (m_begin == m_end) return;
    m_begin = new value_type[2 * n + 1];
    m_end = m_begin + n;
    m_storage_end = m_begin + (2 * n);
  }

  explicit SequentialList(size_t n, const value_type &initVal) : SequentialList(n) {
    if (m_begin == m_end)
      return;
    for (auto beg = m_begin; beg != m_end; ++beg) {
      *beg = initVal;
    }
  }

  ~SequentialList() {
    delete[] m_begin;
    m_begin = nullptr;
    m_storage_end = nullptr;
    m_end = nullptr;
  }

  reference operator[](size_type index) { return m_begin[index]; }
  const_reference operator[](size_type index) const { return m_begin[index]; }

  [[nodiscard]] iterator begin() const { return m_begin; }
  [[nodiscard]] iterator end() const { return m_end; }
  [[nodiscard]] const_iterator cbegin() const { return m_begin; }
  [[nodiscard]] const_iterator cend() const { return m_end; }
  [[nodiscard]] size_type size() const { return m_end - m_begin; }
  [[nodiscard]] bool empty() const { return m_begin == m_end; }

  iterator find(const value_type &val) {
    for (auto beg = m_begin; beg != m_end; ++beg) {
      if (*beg != val) continue;
      return beg;
    }
    return m_end;
  }

  void _traverse(const function<bool(iterator &iter)> &cb) {
    for (auto beg = m_begin; beg != m_end; ++beg) {
      cb(beg);
    }
  }

  void push_back(const value_type &val) {
    // 判断空间是否将要满
    //// 如果将要满，则重新分配空间
    // 更新

    if (m_end == m_storage_end) {
      size_type size = m_end - m_begin;
      auto new_begin = new value_type[size * 2 + 1];
      for (size_t i = 0; i < size; ++i)
        new_begin[i] = m_begin[i];
      delete[] m_begin;
      m_begin = new_begin;
      m_storage_end = &m_begin[2 * size];
    }

    *m_end = val;
    m_end++;
  }

private:
private:
  pointer m_begin;
  pointer m_storage_end;
  pointer m_end;
};
MYLIB_END
