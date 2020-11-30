#ifndef LGPP_OPS_ADD_HPP
#define LGPP_OPS_ADD_HPP

#include "../stack.hpp"
#include "../val.hpp"
#include "../op.hpp"

namespace lgpp::ops {

  struct Add {};

  template <>
  inline const Op* eval(const Op& op, const Add& imp, lgpp::Thread& thread, lgpp::Stack& stack) {
    Val y(pop(stack)), x(pop(stack));
    push(stack, x+y);
    return &op+1;
  }

}

#endif
