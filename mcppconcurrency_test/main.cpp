#include <iostream>
#include <mcppconcurrency/mcppconcurrency/monoid_accumulator.hpp>
#include <mcpputil/mcpputil/bandit.hpp>
#include <mcpputil/mcpputil/literals.hpp>
using namespace bandit;
using namespace ::mcpputil::literals;
go_bandit([]() {
  describe("monoid_accumulator_t", []() {
    it("3fd12db5-14b6-42cb-87a7-7ad5b66cf2a7", []() {
      mcppconcurrency::monoid_accumulator_t<ptrdiff_t> accumulator;
      accumulator.set_max_threads(10);
      accumulator.set_thread_values(0);
      accumulator.set_accumulator_value(0);
      AssertThat(accumulator.max_threads(), Equals(10_sz));
      ptrdiff_t total = 0;
      for (ptrdiff_t i = 0; i < 10; ++i) {
        accumulator.accumulate(i, i);
        total += i;
      }
      accumulator.accumulate_all();
      AssertThat(accumulator.monoid(), Equals(total));
      accumulator.clear();
      AssertThat(accumulator.monoid(), Equals(0));

    });
  });
});
int main(int argc, char *argv[])
{
  auto ret = bandit::run(argc, argv);
#ifdef _WIN32
  ::std::cin.get();
#endif
  return ret;
}
