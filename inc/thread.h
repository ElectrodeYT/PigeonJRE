#pragma once

#include <frame.h>
#include <memory>
#include <vector>

class Thread {
 public:
  Thread(std::string first_method, std::string descriptor, std::vector<Frame::StackEntry> arguments, Object* obj = NULL);
  void begin();
  std::variant<int32_t, int64_t, float, double, Object*> invoke(std::string method, std::string descriptor, std::vector<Frame::StackEntry> arguments, Object* obj = NULL);

 private:
  std::variant<int32_t, int64_t, float, double, Object*> dispatchNativeCall(std::string class_name, std::string method_name, std::string descriptor, std::vector<Frame::StackEntry> arguments, Object* obj = NULL);

  std::vector<std::unique_ptr<Frame>> frame_stack;
};
