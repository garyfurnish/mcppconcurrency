#pragma once
#include "declarations.hpp"
#include <mcpputil/mcpputil/alignment.hpp>
#include <utility>
namespace mcppconcurrency
{
  template <typename Monoid_Type,
            size_t Alignment = mcpputil::cs_default_alignment,
            size_t Padding_Alignment = mcpputil::cs_default_cache_line_size>
  class alignas(Alignment) aligned_monoid_t
  {
  public:
    using monoid_type = Monoid_Type;
    using monoid_reference = monoid_type &;
    using const_monoid_reference = const monoid_type &;
    inline static constexpr const size_t cs_alignment = Alignment;
    inline static constexpr const size_t cs_padding_alignment = Padding_Alignment;
    inline static constexpr const size_t cs_padding_size = cs_padding_alignment - sizeof(monoid_type);

    aligned_monoid_t() = default;
    aligned_monoid_t(const monoid_type &monoid);
    aligned_monoid_t(monoid_type &&monoid);
    aligned_monoid_t(const aligned_monoid_t &) = default;
    aligned_monoid_t(aligned_monoid_t &&) = default;
    aligned_monoid_t &operator=(const aligned_monoid_t &) = default;
    aligned_monoid_t &operator=(aligned_monoid_t &&) = default;

    auto operator+(const monoid_type &monoid) const -> aligned_monoid_t;
    auto operator+(const aligned_monoid_t &monoid) const -> aligned_monoid_t;

    auto operator+=(const monoid_type &monoid) -> aligned_monoid_t &;
    auto operator+=(const aligned_monoid_t &monoid) -> aligned_monoid_t &;

    auto monoid() -> monoid_type;
    auto monoid_ref() -> monoid_reference;
    auto monoid_ref() const -> const_monoid_reference;
    auto monoid_cref() const -> const_monoid_reference;

  private:
    monoid_type m_monoid;
    char m_padding[cs_padding_size];
  };
  static_assert(sizeof(aligned_monoid_t<int, 8, 64>) == 64);
  template <typename Monoid_Type, size_t Alignment, size_t Padding_Alignment>
  aligned_monoid_t<Monoid_Type, Alignment, Padding_Alignment>::aligned_monoid_t(const monoid_type &monoid) : m_monoid(monoid)
  {
  }
  template <typename Monoid_Type, size_t Alignment, size_t Padding_Alignment>
  aligned_monoid_t<Monoid_Type, Alignment, Padding_Alignment>::aligned_monoid_t(monoid_type &&monoid)
      : m_monoid(::std::move(monoid))
  {
  }
  template <typename Monoid_Type, size_t Alignment, size_t Padding_Alignment>
  auto aligned_monoid_t<Monoid_Type, Alignment, Padding_Alignment>::operator+(const monoid_type &monoid) const -> aligned_monoid_t
  {
    return aligned_monoid_t(m_monoid + monoid);
  }
  template <typename Monoid_Type, size_t Alignment, size_t Padding_Alignment>
  auto aligned_monoid_t<Monoid_Type, Alignment, Padding_Alignment>::operator+(const aligned_monoid_t &monoid) const
      -> aligned_monoid_t
  {
    return aligned_monoid_t(m_monoid + monoid.monoid());
  }
  template <typename Monoid_Type, size_t Alignment, size_t Padding_Alignment>
  auto aligned_monoid_t<Monoid_Type, Alignment, Padding_Alignment>::operator+=(const monoid_type &monoid) -> aligned_monoid_t &
  {
    m_monoid = m_monoid + monoid;
    return *this;
  }
  template <typename Monoid_Type, size_t Alignment, size_t Padding_Alignment>
  auto aligned_monoid_t<Monoid_Type, Alignment, Padding_Alignment>::operator+=(const aligned_monoid_t &monoid)
      -> aligned_monoid_t &
  {
    m_monoid = m_monoid + monoid.monoid();
    return *this;
  }
  template <typename Monoid_Type, size_t Alignment, size_t Padding_Alignment>
  auto aligned_monoid_t<Monoid_Type, Alignment, Padding_Alignment>::monoid() -> monoid_type
  {
    return m_monoid;
  }
  template <typename Monoid_Type, size_t Alignment, size_t Padding_Alignment>
  auto aligned_monoid_t<Monoid_Type, Alignment, Padding_Alignment>::monoid_ref() -> monoid_reference
  {
    return m_monoid;
  }
  template <typename Monoid_Type, size_t Alignment, size_t Padding_Alignment>
  auto aligned_monoid_t<Monoid_Type, Alignment, Padding_Alignment>::monoid_ref() const -> const_monoid_reference
  {
    return m_monoid;
  }
  template <typename Monoid_Type, size_t Alignment, size_t Padding_Alignment>
  auto aligned_monoid_t<Monoid_Type, Alignment, Padding_Alignment>::monoid_cref() const -> const_monoid_reference
  {
    return m_monoid;
  }
}
