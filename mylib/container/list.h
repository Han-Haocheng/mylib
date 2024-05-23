#pragma once
#include "../core/mylib_def.h"
#include "iterator.h"
MYLIB_SPACE_BEGIN
class List;

class ListIterator : public RandomAccessIterator<ListIterator, List, int> {
public:
  using super = RandomAccessIterator<ListIterator, List, int>;
  ListIterator() : super() {

  }

protected:
  pointer m_data;
};

class List {
public:
  using value_type = int;
  using iterator = ListIterator;

  List() {
    iterator();
  }
  ~List();

protected:
  size_t m_size;

private:
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

  SequentialList();
  SequentialList(size_t n);

  iterator begin() const { return m_begin; }
  iterator end() const { return m_end; }
  const_iterator cbegin() const { return m_begin; }
  const_iterator cend() const { return m_end; }
  size_type size() const { return m_end - m_begin; }
  bool empty() const { return m_begin == m_end; }

private:
  pointer m_begin;
  pointer m_end;
  pointer m_storage_end;
};
MYLIB_SPACE_END