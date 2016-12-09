#pragma once
#include "declarations.hpp"
#include <mcpputil/mcpputil/container.hpp>
#include <memory>
namespace mcppconcurrency
{
  template <typename Monoid_Type, typename Allocator = ::std::allocator<Monoid_Type>>
  class accumulator_monoid_t
  {
  public:
    using monoid_type = Monoid_Type;
    using monoid_reference = Monoid_Type &;
    using const_monoid_reference = const Monoid_Type &;
    using allocator = Allocator;
    accumulator_monoid_t() noexcept = default;
    accumulator_monoid_t(const accumulator_monoid_t &) = default;
    accumulator_monoid_t(accumulator_monoid_t &&) = default;
    accumulator_monoid_t &operator=(const accumulator_monoid_t &) = default;
    accumulator_monoid_t &operator=(accumulator_monoid_t &&) noexcept = default;

    void set_max_threads(size_t num_threads);
    auto max_threads() -> size_t;

    void accumulate(const monoid_type &);
    void accumulate_all();
    void clear();

    auto monoid() -> monoid_type;
    auto monoid_ref() -> monoid_reference;
    auto monoid_ref() const -> const_monoid_reference;
    auto monoid_cref() const -> const_monoid_reference;

  private:
    monoid_type m_accumulator;
    ::mcpputil::rebind_vector_t<monoid_type, allocator> m_thread_monoids;
  };
  template <typename Monoid_Type, typename Allocator>
  void accumulator_monoid_t<Monoid_Type, Allocator>::set_max_threads(size_t num_threads)
  {
    if (m_thread_monoids.empty()) {
      m_thread_monoids.resize(num_threads);
    } else {
      throw ::std::runtime_error("Max threads for accumulator already set. 5069e93d-aa8f-4729-8c16-f71eb0832d59");
    }
  }
  template <typename Monoid_Type, typename Allocator>
  auto accumulator_monoid_t<Monoid_Type, Allocator>::max_threads() -> size_t
  {
    return m_thread_monoids.size();
  }
  template <typename Monoid_Type, typename Allocator>
  void accumulator_monoid_t<Monoid_Type, Allocator>::accumulate(const monoid_type &monoid)
  {
    m_accumulator = m_accumulator + monoid;
  }
  template <typename Monoid_Type, typename Allocator>
  void accumulator_monoid_t<Monoid_Type, Allocator>::accumulate_all()
  {
    for (auto &&monoid : m_thread_monoids) {
      accumulate(monoid);
      monoid = monoid_type();
    }
  }
  template <typename Monoid_Type, typename Allocator>
  void accumulator_monoid_t<Monoid_Type, Allocator>::clear()
  {
    m_accumulator = monoid_type();
  }
  template <typename Monoid_Type, typename Allocator>
  auto accumulator_monoid_t<Monoid_Type, Allocator>::monoid() -> monoid_type
  {
    return m_accumulator;
  }
  template <typename Monoid_Type, typename Allocator>
  auto accumulator_monoid_t<Monoid_Type, Allocator>::monoid_ref() -> monoid_reference
  {
    return m_accumulator;
  }
  template <typename Monoid_Type, typename Allocator>
  auto accumulator_monoid_t<Monoid_Type, Allocator>::monoid_ref() const -> const_monoid_reference
  {
    return m_accumulator;
  }
  template <typename Monoid_Type, typename Allocator>
  auto accumulator_monoid_t<Monoid_Type, Allocator>::monoid_cref() const -> const_monoid_reference
  {
    return m_accumulator;
  }
}
