#pragma once
namespace mcppconcurrency
{
  template <typename Lock_Functor, typename Unlock_Functor>
  class functor_partial_lockable
  {
    using lock_functor_type = Lock_Functor;
    using unlock_functor_type = Unlock_Functor;
  };
}
