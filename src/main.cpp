#include <iostream>
#include <string>
#include <class.h>
#include <frame.h>
#include <debug.h>
#include <jre.h>
#include <thread.h>

std::string class_file = "Add.class";
std::string runtime_file = "/usr/lib/jvm/java-8-openjdk/jre/lib/rt.jar";

int main() {
    // Add . to the class path
    JRE::the().addClassPath(".");
    JRE::the().addClassPath(runtime_file);
    std::vector<Object*> very_much_nothing;
    // JRE::the().findClass("Main")->dumpCP();
    // JRE::the().findClass("Main")->dumpMethods();
    // JRE::the().findClass("Main")->dumpAttributes();

    std::vector<Frame::StackEntry> main_args;
    Thread main_thread("Main.main", "([Ljava/lang/String;)V", main_args, NULL);
    main_thread.begin();
    // std::cout << std::get<int>(JRE::the().executeStatic("Main.main", "([Ljava/lang/String;)V", main_args)) << "\n";
    std::vector<Frame::StackEntry> max_args;
    max_args.push_back(Frame::StackEntry() = {.value = 60});
    max_args.push_back(Frame::StackEntry() = {.value = 30});
    // std::cout << std::get<int>(JRE::the().executeStatic("java/lang/Math.max", "(II)I", max_args)) << "\n";
    return 0;
}
