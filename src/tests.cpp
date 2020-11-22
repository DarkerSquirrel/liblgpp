#include <cassert>

#include "lgpp/label.hpp"
#include "lgpp/ops/add.hpp"
#include "lgpp/ops/branch_eq.hpp"
#include "lgpp/ops/call.hpp"
#include "lgpp/ops/cp.hpp"
#include "lgpp/ops/dec.hpp"
#include "lgpp/ops/drop.hpp"
#include "lgpp/ops/inc.hpp"
#include "lgpp/ops/push.hpp"
#include "lgpp/ops/ret.hpp"
#include "lgpp/ops/stop.hpp"
#include "lgpp/ops/sub.hpp"
#include "lgpp/ops/swap.hpp"
#include "lgpp/stack.hpp"
#include "lgpp/thread.hpp"
#include "lgpp/type.hpp"
#include "lgpp/vm.hpp"

using namespace lgpp;

Type<int> Int("Int");

void vm_branch_tests(VM &vm) {
  Stack s;

  vm.thread().ops.reserve(10);
  vm.emit<ops::Push>(Int, 1);
  Label b;
  vm.emit<ops::BranchEq>(b, Int, 1);
  vm.emit<ops::Push>(Int, 2);
  vm.emit<ops::Stop>();
  b.pc = vm.last_op().pc;
  vm.eval(0, s);
  
  assert(s.size() == 1);
  assert(s.back().as(Int) == 1);
  vm.clear_ops();
}

void vm_call_tests(VM &vm) {
  Stack s;

  Label f;
  vm.emit<ops::Call>(f);
  vm.emit<ops::Stop>();
  vm.emit<ops::Push>(Int, 42);
  f.pc = vm.last_op().pc;
  vm.emit<ops::Ret>();
  vm.eval(0, s);
  
  assert(s.size() == 1);
  assert(s.back().as(Int) == 42);
  vm.clear_ops();
}

void vm_inc_tests(VM &vm) {
  Stack s;

  vm.emit<ops::Push>(Int, 35);
  vm.emit<ops::Inc>(Int, 7);
  vm.emit<ops::Stop>();
  vm.eval(0, s); 

  assert(s.size() == 1);
  assert(s.back().as(Int) == 42);
  vm.clear_ops();
}

void vm_dec_tests(VM &vm) {
  Stack s;

  vm.emit<ops::Push>(Int, 49);
  vm.emit<ops::Dec>(Int, 7);
  vm.emit<ops::Stop>();
  vm.eval(0, s); 

  assert(s.size() == 1);
  assert(s.back().as(Int) == 42);
  vm.clear_ops();
}

void vm_add_tests(VM &vm) {
  Stack s;

  vm.emit<ops::Push>(Int, 35);
  vm.emit<ops::Push>(Int, 7);
  vm.emit<ops::Add>();
  vm.emit<ops::Stop>();
  vm.eval(0, s); 

  assert(s.size() == 1);
  assert(s.back().as(Int) == 42);
  vm.clear_ops();
}

void vm_sub_tests(VM &vm) {
  Stack s;

  vm.emit<ops::Push>(Int, 49);
  vm.emit<ops::Push>(Int, 7);
  vm.emit<ops::Sub>();
  vm.emit<ops::Stop>();
  vm.eval(0, s); 

  assert(s.size() == 1);
  assert(s.back().as(Int) == 42);
  vm.clear_ops();
}

void vm_math_tests(VM &vm) {
  vm_inc_tests(vm);
  vm_dec_tests(vm);
  vm_add_tests(vm);
  vm_sub_tests(vm);
}

void vm_stack_cp_tests(VM &vm) {
  Stack s;

  vm.emit<ops::Push>(Int, 1);
  vm.emit<ops::Push>(Int, 2);
  vm.emit<ops::Push>(Int, 3);
  vm.emit<ops::Cp>(2, 2);
  vm.emit<ops::Stop>();
  vm.eval(0, s);
  
  assert(s.size() == 5);
  assert(pop(s).as(Int) == 2);
  assert(pop(s).as(Int) == 1);
  assert(pop(s).as(Int) == 3);
  assert(pop(s).as(Int) == 2);
  assert(pop(s).as(Int) == 1);
  vm.clear_ops();  
}

void vm_stack_drop_tests(VM &vm) {
  Stack s;

  vm.emit<ops::Push>(Int, 1);
  vm.emit<ops::Push>(Int, 2);
  vm.emit<ops::Push>(Int, 3);
  vm.emit<ops::Drop>(1, 2);
  vm.emit<ops::Stop>();
  vm.eval(0, s); 

  assert(s.size() == 1);
  assert(pop(s).as(Int) == 1);
  vm.clear_ops();  
}

void vm_stack_swap_tests(VM &vm) {
  Stack s;

  vm.emit<ops::Push>(Int, 1);
  vm.emit<ops::Push>(Int, 2);
  vm.emit<ops::Swap>();
  vm.emit<ops::Stop>();
  auto &stop(vm.last_op());

  assert(stop.pc == 3);
  assert(&vm.eval(0, s) == &stop); 
  assert(s.size() == 2);
  assert(pop(s).as(Int) == 1);
  assert(pop(s).as(Int) == 2);
  vm.clear_ops();
}


void vm_stack_tests(VM &vm) {
  vm_stack_cp_tests(vm);
  vm_stack_drop_tests(vm);
  vm_stack_swap_tests(vm);
}

void vm_tests() {
  VM vm;
  
  vm_branch_tests(vm);
  vm_call_tests(vm);
  vm_math_tests(vm);
  vm_stack_tests(vm);
}

int main() {
  vm_tests();
  return 0;
}
