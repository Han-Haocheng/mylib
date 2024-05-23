#pragma once
#include "../core/mylib_def.h"

MYLIB_SPACE_BEGIN
using ptrdiff_t = long long;

template<class DerivedTy, class ConverterTy, class Ty,
         class DiffTy = ptrdiff_t, class SizeTy = size_t,
         class PointerTy = Ty *, class RefTy = Ty &, class ConstRefTy = const Ty &>
class OutputIterator {
public:
  using converter_type = ConverterTy;
  using derived_type = DerivedTy;
  using size_type = SizeTy;
  using different_type = DiffTy;
  using value_type = Ty;
  using pointer = PointerTy;
  using reference = RefTy;
  using const_reference = ConstRefTy;

  OutputIterator(OutputIterator &&) = delete;
  OutputIterator(const OutputIterator &) = default;
  ~OutputIterator() {}

  OutputIterator &operator=(OutputIterator &&) = delete;
  OutputIterator &operator=(const OutputIterator &) = default;

  reference operator*() const { return static_cast<derived_type *>(this)->reference(); }
  pointer operator->() const { return static_cast<derived_type *>(this)->pointer(); }
  derived_type &operator++() { return static_cast<derived_type *>(this)->self_increase(); }
  derived_type &operator++(int) { return static_cast<derived_type *>(this)->increase_self(); }

  template<class iter>
  void operator==(const iter &rhs) const { return *(*this) == &*rhs; }

  template<class iter>
  bool operator!=(const iter &rhs) const { return !(*this == rhs); }
};

template<class DerivedTy, class ConverterTy, class Ty,
         class DiffTy = ptrdiff_t, class SizeTy = size_t,
         class PointerTy = Ty *, class RefTy = Ty &, class ConstRefTy = const Ty &>
class InputIterator {
public:
  using converter_type = ConverterTy;
  using derived_type = DerivedTy;
  using size_type = SizeTy;
  using different_type = DiffTy;
  using value_type = Ty;
  using pointer = PointerTy;
  using reference = RefTy;

  InputIterator(InputIterator &&) = delete;
  InputIterator(const InputIterator &) = default;
  ~InputIterator() {}

  InputIterator &operator=(InputIterator &&) = delete;
  InputIterator &operator=(const InputIterator &) = default;

  reference operator*() { return static_cast<derived_type *>(this)->reference(); }
  pointer operator->() { return static_cast<derived_type *>(this)->pointer(); }
};

template<class DerivedTy, class ConverterTy, class Ty,
         class DiffTy = ptrdiff_t, class SizeTy = size_t,
         class PointerTy = Ty *, class RefTy = Ty &, class ConstRefTy = const Ty &>
class ForwardIterator : public OutputIterator<DerivedTy, ConverterTy, Ty, DiffTy, SizeTy, PointerTy, RefTy, ConstRefTy>,
                        public InputIterator<DerivedTy, ConverterTy, Ty, DiffTy, SizeTy, PointerTy, RefTy, ConstRefTy> {
public:
  ForwardIterator() = default;
};
template<class DerivedTy, class ConverterTy, class Ty,
         class DiffTy = ptrdiff_t, class SizeTy = size_t,
         class PointerTy = Ty *, class RefTy = Ty &, class ConstRefTy = const Ty &>
class BidirectionalIterator : public ForwardIterator<DerivedTy, ConverterTy, Ty, DiffTy, SizeTy, PointerTy, RefTy, ConstRefTy> {
public:
  using converter_type = ConverterTy;
  using derived_type = DerivedTy;
  using size_type = SizeTy;
  using different_type = DiffTy;
  using value_type = Ty;
  using pointer = PointerTy;
  using reference = RefTy;

  BidirectionalIterator() = default;

  derived_type &operator--() { return static_cast<derived_type *>(this)->self_reduction(); }
  derived_type &operator--(int) { return static_cast<derived_type *>(this)->self_reduction(); }
};

template<class DerivedTy, class ConverterTy, class Ty,
         class DiffTy = ptrdiff_t, class SizeTy = size_t,
         class PointerTy = Ty *, class RefTy = Ty &, class ConstRefTy = const Ty &>
class RandomAccessIterator : public BidirectionalIterator<DerivedTy, ConverterTy, Ty, DiffTy, SizeTy, PointerTy, RefTy, ConstRefTy> {
public:
  using converter_type = ConverterTy;
  using derived_type = DerivedTy;
  using size_type = SizeTy;
  using different_type = DiffTy;
  using value_type = Ty;
  using pointer = PointerTy;
  using reference = RefTy;

  RandomAccessIterator() {}
  RandomAccessIterator(RandomAccessIterator &&) = default;
  RandomAccessIterator(const RandomAccessIterator &) = default;
  ~RandomAccessIterator() {}

  bool operator<(const derived_type &rhs) const { return static_cast<derived_type *>(this)->less(rhs); }
  bool operator>=(const derived_type &rhs) const { return !(*this < rhs); }
  bool operator>(const derived_type &rhs) const { return rhs < *this; }
  bool operator<=(const derived_type &rhs) const { return !(rhs < *this); }

  friend derived_type operator+(const derived_type &lhs, const different_type &rhs) { return lhs->offset(rhs); }
  friend derived_type operator-(const derived_type &lhs, const different_type &rhs) { return lhs->offset(-rhs); }
  friend derived_type operator+(const different_type &lhs, const derived_type &rhs) { return rhs->offset(lhs); }
  friend derived_type operator-(const different_type &lhs, const derived_type &rhs) { return rhs->offset(-lhs); }

  reference operator[](size_type n) { return static_cast<derived_type *>(this)->at(n); }
};

MYLIB_SPACE_END