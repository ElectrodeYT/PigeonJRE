#include "frame.h"
#include "jre.h"

std::variant<int32_t, int64_t, float, double, Object*> Frame::execute()  {
    size_t ip = 0;
    while(ip <= method.code.code.size()) {
        switch(method.code.code[ip]) {

        // Stack modify shit
        case 0x59: push(stack[stack.size() - 1]); break; // dup
        case 0x57: pop(); break; // pop

        case 0x11: {
            int16_t val = (method.code.code[ip + 1] << 8) | method.code.code[ip + 2];
            ip += 2;
            push(val);
            break;
        }


        // Constants
        case 0x03: push(0); break; // iconst_0
        case 0x04: push(1); break; // iconst_1
        case 0x05: push(2); break; // iconst_2
        case 0x06: push(3); break; // iconst_3
        case 0x07: push(4); break; // iconst_4
        case 0x08: push(5); break; // iconst_5


        // Loading stuff
        case 0x1a: case 0x2a: push(locals[0]); break; // iload_0, aload_0
        case 0x1b: case 0x2b: push(locals[1]); break; // iload_1, aload_1
        case 0x1c: case 0x2c: push(locals[2]); break; // iload_2, aload_2
        case 0x1d: case 0x2d: push(locals[3]); break; // iload_3, aload_3


        // Storing stuff
        case 0x3c: case 0x4c: locals[1] = pop(); break; // istore_1, astore_1
        case 0x3d: case 0x4d: locals[2] = pop(); break; // istore_2, astore_2
        case 0x3e: case 0x4e: locals[3] = pop(); break; // istore_3, astore_3

        // Arith
        case 0x60: push((int)(pop_i() + pop_i())); break; // iadd

        // Returns
        case 0xb1: pigeon_assert(return_value == 'V'); return (int32_t)0;
        case 0xac: pigeon_assert(return_value == 'I'); return (int32_t)pop_i(); // iret

        // Branches
        case 0x9c: { // ifge
            uint16_t offset = (method.code.code[ip + 1] << 8) | method.code.code[ip + 2];
            ip += 2;
            int comp = pop_i();
            if(comp >= 0) { ip += offset; ip -= 3; }
            break;
        }
        case 0xa1: { // if_icmplt
            uint16_t offset = (method.code.code[ip + 1] << 8) | method.code.code[ip + 2];
            ip += 2;
            dumpStack();
            int comp2 = pop_i();
            int comp1 = pop_i();
            if(comp1 < comp2) { ip += offset; ip -= 3; }
            break;
        }
        case 0xa7: { // goto
            uint16_t offset = (method.code.code[ip + 1] << 8) | method.code.code[ip + 2];
            ip += offset; ip -= 3;
            break;
        }

        // Invokes
        case 0xb6: { // invokevirtual
            uint16_t index = (method.code.code[ip + 1] << 8) | method.code.code[ip + 2];
            ip += 2;
            std::vector<StackEntry> arguments;
            CPInfo method_reference = method.method_class->readAndVerifyCPEntry(index, CPTag::MethodReference);
            // Get the class that the function is from
            std::string class_name = method.method_class->readAndVerifyCPEntry(method.method_class->readAndVerifyCPEntry(method_reference.ClassReference, CPTag::ClassReference).ClassReference, CPTag::UTF8).string ;
            // Name of the function we are calling
            std::string function_name = method.method_class->readAndVerifyCPEntry(method.method_class->readAndVerifyCPEntry(method_reference.NameAndTypeDescriptor, CPTag::NameAndTypeDescriptor).NameStringIndex, CPTag::UTF8).string;
            // Descriptor of the function we are calling
            std::string function_descriptor = method.method_class->readAndVerifyCPEntry(method.method_class->readAndVerifyCPEntry(method_reference.NameAndTypeDescriptor, CPTag::NameAndTypeDescriptor).TypeStringIndex, CPTag::UTF8).string;
            // Get amount of arguments
            size_t argument_count = JRE::the().argC(function_descriptor);
            for(size_t i = 0; i < argument_count; i++) { arguments.push_back(pop()); }
            Object* obj = std::get<Object*>(pop().value);
            // Execute function and get return value, if any
            std::variant<int32_t, int64_t, float, double, Object*> virt_ret = JRE::the().executeVirtual(obj, class_name + "." + function_name, function_descriptor, arguments);
            if(function_descriptor[function_descriptor.size() - 1] != 'V') {
                StackEntry e;
                e.value = virt_ret;
                push(e);
            }
            std::cout << "Function " << class_name << "." << function_name << "returned\n";
            dumpStack();
            break;
        }
        case 0xb7: { // invokespecial
            uint16_t index = (method.code.code[ip + 1] << 8) | method.code.code[ip + 2];
            ip += 2;
            std::vector<StackEntry> arguments;
            StackEntry last;
            while(!(last = pop()).isObjectReference) { arguments.push_back(last); }
            Object* obj = std::get<Object*>(last.value);
            CPInfo method_reference = method.method_class->readAndVerifyCPEntry(index, CPTag::MethodReference);
            // Get function information
            std::string class_name = method.method_class->readAndVerifyCPEntry(method.method_class->readAndVerifyCPEntry(method_reference.ClassReference, CPTag::ClassReference).ClassReference, CPTag::UTF8).string ;
            std::string function_name = method.method_class->readAndVerifyCPEntry(method.method_class->readAndVerifyCPEntry(method_reference.NameAndTypeDescriptor, CPTag::NameAndTypeDescriptor).NameStringIndex, CPTag::UTF8).string;
            std::string function_descriptor = method.method_class->readAndVerifyCPEntry(method.method_class->readAndVerifyCPEntry(method_reference.NameAndTypeDescriptor, CPTag::NameAndTypeDescriptor).TypeStringIndex, CPTag::UTF8).string;
            if(class_name == "java/lang/Object") { break; }
            JRE::the().executeVirtual(obj, class_name + "." + function_name, function_descriptor, arguments);
            break;
        }
        case 0xb8: { // invokestatic
            uint16_t index = (method.code.code[ip + 1] << 8) | method.code.code[ip + 2];
            ip += 2;
            std::vector<StackEntry> arguments;
            CPInfo method_reference = method.method_class->readAndVerifyCPEntry(index, CPTag::MethodReference);
            // Get function information
            std::string class_name = method.method_class->readAndVerifyCPEntry(method.method_class->readAndVerifyCPEntry(method_reference.ClassReference, CPTag::ClassReference).ClassReference, CPTag::UTF8).string ;
            std::string function_name = method.method_class->readAndVerifyCPEntry(method.method_class->readAndVerifyCPEntry(method_reference.NameAndTypeDescriptor, CPTag::NameAndTypeDescriptor).NameStringIndex, CPTag::UTF8).string;
            std::string function_descriptor = method.method_class->readAndVerifyCPEntry(method.method_class->readAndVerifyCPEntry(method_reference.NameAndTypeDescriptor, CPTag::NameAndTypeDescriptor).TypeStringIndex, CPTag::UTF8).string;
            // Get the argument count
            size_t argument_count = JRE::the().argC(function_descriptor);
            if(argument_count) { arguments.insert(arguments.end(), stack.end() - argument_count, stack.end()); stack.resize(stack.size() - argument_count); };
            if(class_name == "java/lang/Object") { break; }
            std::variant<int32_t, int64_t, float, double, Object*> static_ret = JRE::the().executeStatic(class_name + "." + function_name, function_descriptor, arguments);
            // If the function returns a value, add it to the stack
            if(function_descriptor[function_descriptor.size() - 1] != 'V') {
                StackEntry e;
                e.value = static_ret;
                push(e);
            }
            dumpStack();
            break;
        }

        // Object shit
        case 0xbb: { // new
            uint16_t index = (method.code.code[ip + 1] << 8) | method.code.code[ip + 2];
            ip += 2;
            // Get reference of the class we need to create
            CPInfo ref = method.method_class->readAndVerifyCPEntry(index, CPTag::ClassReference);
            std::string class_name = method.method_class->readAndVerifyCPEntry(ref.ClassReference, CPTag::UTF8).string;
            pigeon_log("Frame", "Creating new object " << class_name);
            Object* obj = JRE::the().createObject();
            // Clone the class used
            obj->class_ = JRE::the().findClass(class_name)->clone();
            push(obj);
            break;
        }


        // Fields
        case 0xb4: { // getfield
            uint16_t index = (method.code.code[ip + 1] << 8) | method.code.code[ip + 2];
            ip += 2;
            CPInfo ref = method.method_class->readAndVerifyCPEntry(index, CPTag::FieldReference);
            std::string class_name = method.method_class->readAndVerifyCPEntry(method.method_class->readAndVerifyCPEntry(ref.ClassReference, CPTag::ClassReference).ClassReference, CPTag::UTF8).string ;
            std::string field_name = method.method_class->readAndVerifyCPEntry(method.method_class->readAndVerifyCPEntry(ref.NameAndTypeDescriptor, CPTag::NameAndTypeDescriptor).NameStringIndex, CPTag::UTF8).string;
            std::string field_descriptor = method.method_class->readAndVerifyCPEntry(method.method_class->readAndVerifyCPEntry(ref.NameAndTypeDescriptor, CPTag::NameAndTypeDescriptor).TypeStringIndex, CPTag::UTF8).string;
            pigeon_log("Frame", "Getting field " << class_name << "." << field_name << ":" << field_descriptor);
            StackEntry stack_object = pop();
            pigeon_assert(stack_object.isObjectReference);
            Object* object = std::get<Object*>(stack_object.value);
            StackEntry e;
            e.value = object->class_->getField(field_name);
            // TODO: check if value is a object
            push(e);
            break;
        }
        case 0xb5: { // putfield
            uint16_t index = (method.code.code[ip + 1] << 8) | method.code.code[ip + 2];
            ip += 2;
            CPInfo ref = method.method_class->readAndVerifyCPEntry(index, CPTag::FieldReference);
            std::string class_name = method.method_class->readAndVerifyCPEntry(method.method_class->readAndVerifyCPEntry(ref.ClassReference, CPTag::ClassReference).ClassReference, CPTag::UTF8).string ;
            std::string field_name = method.method_class->readAndVerifyCPEntry(method.method_class->readAndVerifyCPEntry(ref.NameAndTypeDescriptor, CPTag::NameAndTypeDescriptor).NameStringIndex, CPTag::UTF8).string;
            std::string field_descriptor = method.method_class->readAndVerifyCPEntry(method.method_class->readAndVerifyCPEntry(ref.NameAndTypeDescriptor, CPTag::NameAndTypeDescriptor).TypeStringIndex, CPTag::UTF8).string;
            pigeon_log("Frame", "Setting field " << class_name << "." << field_name << ":" << field_descriptor);
            StackEntry val = pop();
            StackEntry stack_object = pop();
            pigeon_assert(stack_object.isObjectReference);
            Object* object = std::get<Object*>(stack_object.value);
            object->class_->setField(field_name, val.value);
            break;
        }

        default: {
            pigeon_log("Frame", "Got unknown instruction: " << std::hex << (int)method.code.code[ip] << std::dec);
            pigeon_log("Frame", "\tIP: " << ip);
            pigeon_log("Frame", "\tMethod: " << method.name);
            pigeon_log("Frame", "\tClass: " << method.method_class->name);
            dumpStack();
            pigeon_panic("Unknown instruction");
        }
        }
        ip++;
    }
    return NULL;
}

void Frame::dumpStack() {
    pigeon_log("Frame", "Current Stack:");
    if(stack.size() == 0) { std::cout << "(empty)"; return; }
    for(int i = stack.size() - 1; i >= 0; i--) {
        StackEntry& e = stack[i];
        if(e.isObjectReference) {
            pigeon_log("Frame", i << ": " << "[Reference]");
        } else if(auto I = std::get_if<int>(&e.value)) {
            pigeon_log("Frame", i << ": " << *I);
        } else {
            pigeon_panic("Unexpected stack value in dumpStack()");
        }
    }
}
