#pragma once

#include <memory>

#include "control_block.h"

template <typename T>
struct shared_ptr
{
  shared_ptr();

  // nullptr_t
  shared_ptr(std::nullptr_t) noexcept; // produces empty shared_ptr

  explicit shared_ptr(T *ptr);

  // бывает пустой и нулевой
  // empty -> cblock == nullptr
  // shared_ptr<T>(nullptr)

  // nullified -> ptr == nullptr
  // shared_ptr<int>(static_cast<int*>(nullptr))

  template <typename U, typename D>
  shared_ptr(U *ptr, D deleter);

  template <typename U>
  explicit shared_ptr(U *ptr); /* : shared_ptr(ptr, std::default_delete<U>()) {}*/

  template <typename U>
  explicit shared_ptr(shared_ptr<U> const &other, T *ptr) noexcept; // alias constructor (make other's cblock shared)

  template <typename Y>
  shared_ptr(shared_ptr<Y> const &other) noexcept; // copy constructor

  // operator = for rvalue, lvalue
  // ~
  // reset()
  // reset(Y*)
  // get()
  // *
  // ->
  // oeprator bool
  // use_count

private:
  control_block *cblock;
  T *ptr;

  template <typename... Args>
  friend shared_ptr make_shared(Args &&... args);
};

template <typename T>
shared_ptr<T>::shared_ptr(T *ptr) : shared_ptr(ptr, std::default_delete<T>())
{
}

template <typename T>
template <typename Y>
shared_ptr<T>::shared_ptr(Y *ptr) : shared_ptr(ptr, std::default_delete<Y>())
{
}

template <typename T>
template <typename Y, typename D>
shared_ptr<T>::shared_ptr(Y *ptr, D deleter) : ptr(ptr)
{
  try
  {
    cblock = new regular_control_block<Y, D>(ptr, deleter);
  }
  catch (/*...*/)
  {
    // if Y is not a child of T
    deleter(ptr);
    throw;
  }
}

template <typename T, typename... Args>
shared_ptr<T> make_shared(Args &&... args)
{
  inplace_control_block<T> *cblock = new inplace_control_block<T>(std::forward<Args>(args)...);
}
