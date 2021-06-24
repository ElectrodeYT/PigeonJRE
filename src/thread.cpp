#include <jre.h>
#include <thread.h>

Thread::Thread(std::string first_method, std::string descriptor, std::vector<Frame::StackEntry> arguments, Object* obj) {
    // Get class and method names
    std::string class_name = first_method.substr(0, first_method.find_first_of('.'));
    std::string function_name = first_method.substr(first_method.find_first_of('.') + 1);
    Class* c = JRE::the().findClass(class_name);
    Method& m = c->findMethod(function_name, descriptor);
    if(JRE::the().isMethodStatic(first_method, descriptor)) {
        // Create initial frame
        frame_stack.emplace_back(std::make_unique<Frame>(m, obj, arguments, this));
    } else {
        pigeon_assert(obj);
        frame_stack.emplace_back(std::make_unique<Frame>(m, obj, arguments, this));
    }
}

void Thread::begin() {
    frame_stack[frame_stack.size() - 1]->execute();
}

std::variant<int32_t, int64_t, float, double, Object*> Thread::invoke(std::string method, std::string descriptor, std::vector<Frame::StackEntry> arguments, Object* obj) {
    std::string class_name = method.substr(0, method.find_first_of('.'));
    std::string function_name = method.substr(method.find_first_of('.') + 1);
    pigeon_log("Thread", "Calling virtual function " << function_name << ", from class " << class_name << "; full ID " << method);
    if(obj) {
        pigeon_log("Thread", "Object class name: " << obj->class_->name << "\n");
    }

    Class* c = JRE::the().findClass(class_name);
    Method& m = c->findMethod(function_name, descriptor);
    if(m.access_flags & Method::AccessFlags::Native) {
        std::cout << "TODO: attempted to call function marked native " << method << "\n";
        pigeon_panic("TODO");
    }

    frame_stack.emplace_back(std::make_unique<Frame>(m, obj, arguments, this));
    std::variant<int32_t, int64_t, float, double, Object*> ret = frame_stack[frame_stack.size() - 1]->execute();
    frame_stack.pop_back();
    pigeon_log("Thread", "Done executing " << method);
    return ret;
}
