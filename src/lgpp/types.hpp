#ifndef LGPP_TYPES_HPP
#define LGPP_TYPES_HPP

#include "lgpp/coro.hpp"
#include "lgpp/pair.hpp"
#include "lgpp/stack.hpp"
#include "lgpp/thread.hpp"
#include "lgpp/type.hpp"

namespace lgpp::types {
  
  extern Trait Num, Seq;

  extern Type<Coro> Coro;
  
  extern Type<int> Int;

  template <>
  inline void dump(Type<int>& type, const int& x, ostream &out) { out << x; }

  template <>
  inline Val add(Type<int>& type, const int& x, Val y) { return Val(type, x + y.as(type)); }
  
  template <>
  inline Val sub(Type<int>& type, const int& x, Val y) { return Val(type, x - y.as(type)); }

  extern Type<lgpp::Trait *> Meta;

  template <>
  inline void dump(Type<lgpp::Trait*>& type, lgpp::Trait* const& x, ostream &out) { out << x->name; }

  extern Type<nullptr_t> Nil;

  template <>
  inline void dump(Type<nullptr_t>& type, const nullptr_t& x, ostream &out) { out << "_"; }

  template <>
  inline bool eq(Type<nullptr_t> &type, const nullptr_t& x, Val y) { return true; }
  
  template <>
  inline bool gt(Type<nullptr_t> &type, const nullptr_t& x, Val y) { return false; }
  
  template <>
  inline bool lt(Type<nullptr_t> &type, const nullptr_t& x, Val y) { return false; }

  extern Type<lgpp::Pair> Pair;

  template <>
  inline void dump(Type<lgpp::Pair>& type, const lgpp::Pair& x, ostream &out) {
    out << x.first << ' ' << x.second;
  }

  extern Type<lgpp::Stack> Stack;

  template <>
  inline void dump(Type<lgpp::Stack>& type, const lgpp::Stack& x, ostream &out) { dump(x, out); }

  extern Type<Thread::Id> Thread;

}

#endif
