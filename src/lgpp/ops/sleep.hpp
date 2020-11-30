#ifndef LGPP_OPS_SLEEP_HPP
#define LGPP_OPS_SLEEP_HPP

#include <chrono>
#include <thread>

#include "../val.hpp"
#include "../op.hpp"

namespace lgpp::ops {

  struct Sleep {};

  template <>
  inline const Op* eval(const Op& op, const Sleep& imp, lgpp::Thread& thread, lgpp::Stack& stack) {
    auto ms = pop(stack, lgpp::types::Int);
    this_thread::sleep_for(chrono::milliseconds(ms));
    return &op+1;
  }

}

#endif
