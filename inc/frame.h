#pragma once

#include <cstdarg>
#include <string>
#include <variant>
#include <vector>
#include "class.h"

struct Object {
  Class* class_;
  std::variant<int32_t, int64_t, uint32_t, uint64_t, float, double> value;
};

// A Java frame.
// This actually executes the bytecode

// Forward declaration
class Thread;

class Frame {
 public:
  struct StackEntry {
    bool isObjectReference = false;
    std::variant<int32_t, int64_t, float, double, Object*> value;
    std::vector<Object*> value_list = {};
  };

  Frame(const Method m, Object* this_, std::vector<StackEntry> args, Thread* t) : method(m), thread(t) {
    stack.reserve(method.code.max_stack);
    locals.reserve(method.code.max_locals);
    if (this_) {
      StackEntry e;
      e.isObjectReference = true;
      e.value = this_;
      locals.push_back(e);
    }
    locals.insert(locals.end(), args.begin(), args.end());
    // Increase the size to max_locals
    locals.resize(method.code.max_locals);
    return_value = method.descriptor[method.descriptor.size() - 1];
  }

  std::variant<int32_t, int64_t, float, double, Object*> execute();

 private:
  std::vector<StackEntry> stack;
  std::vector<StackEntry> locals;

  inline void push(StackEntry e) { stack.push_back(e); }

  inline void push(int a) {
    StackEntry e;
    e.value = a;
    stack.push_back(e);
  }

  inline void push(float f) {
    StackEntry e;
    e.value = (float)f;
    stack.push_back(e);
  }

  inline void push(Object* obj) {
    StackEntry e;
    e.value = obj;
    e.isObjectReference = true;
    stack.push_back(e);
  }

  inline StackEntry pop() {
    StackEntry e = stack[stack.size() - 1];
    stack.pop_back();
    return e;
  }

  inline int pop_i() {
    StackEntry e = pop();
    return std::get<int>(e.value);
  }

  inline float pop_f() {
    StackEntry e = pop();
    return std::get<float>(e.value);
  }

  void dumpStack();

  char return_value = 0;

  const Method method;
  Thread* thread;
};
