#pragma once

#include <cstddef>
#include <utility>

struct control_block
{

  control_block() noexcept;

  void add_ref() noexcept;
  void del_ref() noexcept;

  std::size_t ref_count() const noexcept;

protected:
  virtual ~control_block() = default;
  virtual void delete_object() noexcept = 0;

private:
  std::size_t n_refs;
};

template <typename T, typename D>
struct regular_control_block final : control_block
{
  explicit regular_control_block(T *ptr, D deleter);

  void delete_object() noexcept override;

private:
  T *ptr;
  D deleter;
};

template <typename T>
struct inplace_control_block final : control_block
{
  template <typename... Args>
  explicit inplace_control_block(Args &&... args);

  // TODO: virtual functions
private:
  typename std::aligned_storage<sizeof(T), alignof(T)>::type stg;
};

template <typename T>
template <typename... Args>
inplace_control_block<T>::inplace_control_block(Args &&... args)
{
  new (&stg) T(std
               : forward<Args>(args)...)
}

// TODO:
// 1. inplace control block
// 2.