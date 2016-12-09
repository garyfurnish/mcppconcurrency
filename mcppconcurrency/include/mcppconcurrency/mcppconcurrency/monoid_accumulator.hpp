#pragma once
#include "aligned_monoid.hpp"
#include "declarations.hpp"
#include <gsl/gsl>
#include <mcpputil/mcpputil/container.hpp>
#include <memory>
namespace mcppconcurrency
{
  /**
   * \brief A per thread accumulator.
   *
   * This is used as follows: accumulate individual threads using accumulate.
   * Then call accumulate all threads.
   * The accumulated value is then accessible.
   * This separates the different thread monoids onto different cache lines.
   * This prevents excessive cross node traffic.
   * @tparam Monoid_Type Type of monoid to accumulate.
   * @tparam Allocator Allocator to use internally.
   **/
  template <typename Monoid_Type, typename Allocator = ::std::allocator<Monoid_Type>>
  class monoid_accumulator_t
  {
  public:
    using monoid_type = Monoid_Type;
    using monoid_reference = Monoid_Type &;
    using const_monoid_reference = const Monoid_Type &;
    using allocator = Allocator;
    using id_type = ptrdiff_t;
    monoid_accumulator_t() noexcept = default;
    monoid_accumulator_t(const monoid_accumulator_t &) = default;
    monoid_accumulator_t(monoid_accumulator_t &&) = default;
    monoid_accumulator_t &operator=(const monoid_accumulator_t &) = default;
    monoid_accumulator_t &operator=(monoid_accumulator_t &&) noexcept = default;
    /**
     * \brief Set maximum number of threads.
     *
     * May only be called once.
     **/
    void set_max_threads(size_t num_threads);
    /**
     * \brief Return maximum number of threads.
     **/
    auto max_threads() -> size_t;
    /**
     * \brief Set per thread values to a common value.
     **/
    void set_thread_values(const monoid_type &monoid);
    /**
     * \brief Set accumulator monoid value.
     **/
    void set_accumulator_value(const monoid_type &monoid);
    /**
     * \brief Accumulate for given id.
     *
     * This is safety checked on id range.
     * @param id Id to accumulate on.
     * @param monoid Monoid to accumulate.
     **/
    void accumulate(id_type id, const monoid_type &monoid);
    /**
     * \brief Accumulate for given id.
     *
     * This is not safety checked on id range.
     * @param id Id to accumulate on.
     * @param monoid Monoid to accumulate.
     **/
    void accumulate_unsafe(id_type id, const monoid_type &monoid);
    /**
     * \brief Accumulate per thread values.
     **/
    void accumulate_all();
    /**
     * \brief Clear all values by default construction.
     **/
    void clear();
    /**
     * \brief Return accumulated values.
     **/
    auto monoid() -> monoid_type;
    /**
     * \brief Return reference to accumulated values.
     **/
    auto monoid_ref() -> monoid_reference;
    /**
     * \brief Return const reference to accumulated values.
     **/
    auto monoid_ref() const -> const_monoid_reference;
    /**
     * \brief Return const reference to accumulated values.
     **/
    auto monoid_cref() const -> const_monoid_reference;

  private:
    /**
     * \brief Add a monoid value to accumulated values.
     **/
    void accumulate(const monoid_type &monoid);
    /**
     * \brief Accumulated values.
     **/
    monoid_type m_accumulator;
    /**
     * \brief Vector of per thread monoids.
     **/
    ::mcpputil::rebind_vector_t<
        aligned_monoid_t<monoid_type, mcpputil::cs_default_alignment, mcpputil::cs_default_cache_line_size>,
        allocator>
        m_thread_monoids;
  };
  template <typename Monoid_Type, typename Allocator>
  void monoid_accumulator_t<Monoid_Type, Allocator>::set_max_threads(size_t num_threads)
  {
    if (m_thread_monoids.empty()) {
      m_thread_monoids.resize(num_threads);
    } else {
      throw ::std::runtime_error("Max threads for accumulator already set. 5069e93d-aa8f-4729-8c16-f71eb0832d59");
    }
  }
  template <typename Monoid_Type, typename Allocator>
  auto monoid_accumulator_t<Monoid_Type, Allocator>::max_threads() -> size_t
  {
    return m_thread_monoids.size();
  }
  template <typename Monoid_Type, typename Allocator>
  void monoid_accumulator_t<Monoid_Type, Allocator>::set_thread_values(const monoid_type &monoid)
  {
    for (auto &&lmonoid : m_thread_monoids) {
      lmonoid = monoid;
    }
  }
  template <typename Monoid_Type, typename Allocator>
  void monoid_accumulator_t<Monoid_Type, Allocator>::set_accumulator_value(const monoid_type &monoid)
  {
    m_accumulator = monoid;
  }

  template <typename Monoid_Type, typename Allocator>
  void monoid_accumulator_t<Monoid_Type, Allocator>::accumulate(const monoid_type &monoid)
  {
    m_accumulator = m_accumulator + monoid;
  }
  template <typename Monoid_Type, typename Allocator>
  void monoid_accumulator_t<Monoid_Type, Allocator>::accumulate(id_type id, const monoid_type &monoid)
  {
    if (id >= ::gsl::narrow_cast<ptrdiff_t>(m_thread_monoids.size()))
      throw ::std::runtime_error("Monoid accumulator id out of bounds. 161115cd-3604-457c-800f-d172c8776f62");
    accumulate_unsafe(id, monoid);
  }
  template <typename Monoid_Type, typename Allocator>
  void monoid_accumulator_t<Monoid_Type, Allocator>::accumulate_unsafe(id_type id, const monoid_type &monoid)
  {
    auto &ref = m_thread_monoids[::gsl::narrow_cast<size_t>(id)];
    ref = ref + monoid;
  }
  template <typename Monoid_Type, typename Allocator>
  void monoid_accumulator_t<Monoid_Type, Allocator>::accumulate_all()
  {
    for (auto &&monoid : m_thread_monoids) {
      accumulate(monoid.monoid());
      monoid = monoid_type();
    }
  }
  template <typename Monoid_Type, typename Allocator>
  void monoid_accumulator_t<Monoid_Type, Allocator>::clear()
  {
    m_accumulator = monoid_type();
    for (auto &&monoid : m_thread_monoids) {
      monoid = monoid_type();
    }
  }
  template <typename Monoid_Type, typename Allocator>
  auto monoid_accumulator_t<Monoid_Type, Allocator>::monoid() -> monoid_type
  {
    return m_accumulator;
  }
  template <typename Monoid_Type, typename Allocator>
  auto monoid_accumulator_t<Monoid_Type, Allocator>::monoid_ref() -> monoid_reference
  {
    return m_accumulator;
  }
  template <typename Monoid_Type, typename Allocator>
  auto monoid_accumulator_t<Monoid_Type, Allocator>::monoid_ref() const -> const_monoid_reference
  {
    return m_accumulator;
  }
  template <typename Monoid_Type, typename Allocator>
  auto monoid_accumulator_t<Monoid_Type, Allocator>::monoid_cref() const -> const_monoid_reference
  {
    return m_accumulator;
  }
}
