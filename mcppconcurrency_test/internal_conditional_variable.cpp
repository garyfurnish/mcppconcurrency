#include <mcpputil/mcpputil/bandit.hpp>
#include <mcpputil/mcpputil/condition_variable.hpp>
#include <mcpputil/mcpputil/literals.hpp>
using namespace bandit;
using namespace ::mcpputil::literals;
void internal_conditional_variable_test()
{
  describe("internal_conditional_variable_tt", []() {
    it("test1 531745b3-65c4-4f17-b7d1-4fbb4f269fa4", [] {
      using allocator_type = ::std::allocator<void>;
      mcpputil::internal_condition_variable_t<allocator_type> conditional;
      ::std::atomic<bool> atom{false};
      ::std::mutex mutex;
      ::std::unique_lock<decltype(mutex)> lock(mutex);
      auto thread_func = [&atom, &conditional]() {
        atom = true;
        conditional.notify_all();
      };
      auto thread = ::std::thread(thread_func);
      conditional.wait(lock, [&atom]() { return atom.load(); });
      thread.join();

    });
  });
}
