#include <algorithm>

#include <class.h>
#include <frame.h>

Class::Class(std::string path) {
    // Setup fstream
    std::ifstream file(path, std::ios::in | std::ios::ate);
    if(!file.good()) {
        throw std::invalid_argument("Input error");
    }
    // Get file size
    size_t file_size = file.tellg();
    // Go to begin
    file.seekg(0, std::ios::beg);

    // Read into vector
    file_data.resize(file_size);
    file.read((char*)file_data.data(), file_size);
    file.close();

    // Load the data
    loadData();
}

Class::Class(std::vector<uint8_t> data) : file_data(data) {
    loadData();
}

void Class::loadData() {
    // Verify CAFEBABE
    uint8_t cafebabe[4];
    cafebabe[0] = readByte();
    cafebabe[1] = readByte();
    cafebabe[2] = readByte();
    cafebabe[3] = readByte();
    if(cafebabe[0] != 0xCA || cafebabe[1] != 0xFE || cafebabe[2] != 0xBA || cafebabe[3] != 0xBE) {
        throw std::invalid_argument("File not a class file");
    }

    // Get version
    file_major_version = readShort();
    file_minor_version = readShort();

    // The class file specs says that if the major version is 56 or above, then minor must be 0 or 65535
    if(file_major_version >= 56) {
        if(file_minor_version != 0 && file_minor_version != 65535) {
            throw std::invalid_argument("File is corrupted; invalid minor version");
        }
    }

    // Read the constant pool size
    uint16_t cp_size = readShort();
    // Read the constant pool
    for(size_t i = 1; i < cp_size; i++) {
        CPInfo entry;
        entry.tag = (CPTag)readByte();
        switch(entry.tag) {
        case(CPTag::UTF8): {
            // Get size
            uint16_t size = readShort();
            // Resize string and get that data
            entry.string.resize(size);
            // Ensure the file is actually long enough
            pigeon_assert(file_data_pointer + size <= file_data.size());
            memcpy(entry.string.data(), file_data.data() + file_data_pointer, size);
            file_data_pointer += size;
            break;
        }
        case(CPTag::Integer): {
            entry.Integer = readInt();
            break;
        }
        case(CPTag::Float): {
            entry.Float = readFloat();
            break;
        }
        case(CPTag::Long): {
            entry.Long = readLong();
            // Longs and doubles take up two indexes
            constant_pool.push_back(entry);
            i++;
            break;
        }
        case(CPTag::Double): {
            entry.Double = readDouble();
            constant_pool.push_back(entry);
            i++;
            break;
        }
        case(CPTag::ClassReference): {
            entry.ClassReference = readShort();
            break;
        }
        case(CPTag::StringReference): {
            entry.StringReference = readShort();
            break;
        }
        case(CPTag::FieldReference):
        case(CPTag::MethodReference):
        case(CPTag::InterfaceMethodReference): {
            entry.ClassReference = readShort();
            entry.NameAndTypeDescriptor = readShort();
            break;
        }
        case(CPTag::NameAndTypeDescriptor): {
            entry.NameStringIndex = readShort();
            entry.TypeStringIndex = readShort();
            break;
        }
        case(CPTag::MethodHandle):
        case(CPTag::MethodType):
        case(CPTag::Dynamic):
        case(CPTag::InvokeDynamic):
        case(CPTag::Module):
        case(CPTag::Package): {
            pigeon_panic("Unsupported CP entry");
            break;
        }
        default: {
            std::cout << "Got invalid constant pool entry: " << entry.tag << "\n";
            std::cout << "At entry: " << i << "\n";
            std::cout << "Entry count: " << cp_size << "\n";
            std::cout << "Previous entries: \n";
            for(size_t y = 0; y < constant_pool.size(); y++) {
                std::cout << "\t" << constant_pool[y].toString() << "\n";
                if(constant_pool[y].tag == CPTag::UTF8) {
                    std::cout << "\t\t" << constant_pool[y].string << "\n";
                }
            }
            pigeon_panic("Invalid CP entry");
        }
        }

        constant_pool.push_back(entry);
    }
    // dumpCP();

    access_flags = readShort();
    this_class_index = readShort();
    super_class_index = readShort();


    // Read the interface table
    uint16_t interface_table_size = readShort() * sizeof(uint16_t);
    // Since its just a bunch of uint16_t's, we can just read it into
    // the vector directly
    interfaces.resize(interface_table_size);
    // Ensure the file is actually long enough
    pigeon_assert(file_data_pointer + interface_table_size <= file_data.size());
    memcpy(interfaces.data(), file_data.data() + file_data_pointer, interface_table_size);
    file_data_pointer += interface_table_size;
    // Read the field table
    class_field_list = readFieldStruct();
    // Read the method table
    methods = readMethods();
    // dumpMethods();
    // Read the attribute table
    attributes = readAttributes();

    name = readAndVerifyCPEntry(readAndVerifyCPEntry(this_class_index, CPTag::ClassReference).ClassReference, CPTag::UTF8).string;

    pigeon_log("Class", "Read class file, class name " << name);
    if(super_class_index) {
        super_name = readAndVerifyCPEntry(readAndVerifyCPEntry(super_class_index, CPTag::ClassReference).ClassReference, CPTag::UTF8).string;
        pigeon_log("Class", "Super class name "  << super_name);
    }
}

std::vector<Attribute> Class::readAttributes() {
    uint16_t attributes_count = readShort();
    std::vector<Attribute> ret;
    for(size_t i = 0; i < attributes_count; i++) {
        Attribute a;
        uint16_t name_index = readShort();
        CPInfo name = readAndVerifyCPEntry(name_index, CPTag::UTF8);
        a.name = name.string;
        // Read the info
        uint32_t attribute_length = readInt();
        a.info.resize(attribute_length);
        // Ensure the file is actually long enough
        pigeon_assert(file_data_pointer + attribute_length <= file_data.size());
        memcpy(a.info.data(), file_data.data() + file_data_pointer, attribute_length);
        file_data_pointer += attribute_length;
        ret.push_back(a);
    }

    return ret;
}

std::vector<Field> Class::readFieldStruct() {
    uint16_t amount = readShort();
    std::vector<Field> ret;

    for(size_t i = 0; i < amount; i++) {
        Field f;
        f.access_flags = readShort();
        f.name = readAndVerifyCPEntry(readShort(), CPTag::UTF8).string;
        f.descriptor = readAndVerifyCPEntry(readShort(), CPTag::UTF8).string;
        f.attributes = readAttributes();
        ret.push_back(f);
    }

    return ret;
}

std::vector<Method> Class::readMethods() {
    uint16_t amount = readShort();
    std::vector<Method> methods;

    for(size_t i = 0; i < amount; i++) {
        Method m(this);
        m.access_flags = readShort();
        m.name = readAndVerifyCPEntry(readShort(), CPTag::UTF8).string;
        m.descriptor = readAndVerifyCPEntry(readShort(), CPTag::UTF8).string;
        m.attributes = readAttributes();
        // We now go through the attributes, to find the code attribute
        bool has_code = false;
        for(size_t y = 0; y < m.attributes.size(); y++) {
            if(m.attributes[y].name != "Code") { break; }
            // This is a code attribute
            Attribute& attr = m.attributes[y];
            // Pointer into the info
            uint8_t* attr_info = (uint8_t*)attr.info.data();
            pigeon_assert(!has_code);
            has_code = true;
            CodeAttribute code;
            // Read the attribute info
            // This abomination gets the pointer of the byte at 0, casts it to a 16 bit uint,
            // then dereferences it again
            code.max_stack = be16toh(*((uint16_t*)(attr_info))); attr_info += 2;
            code.max_locals = be16toh(*((uint16_t*)(attr_info))); attr_info += 2;
            uint32_t code_size = be32toh(*((uint32_t*)(attr_info))); attr_info += 4;
            code.code.resize(code_size);

            // Copy the code itself
            memcpy(code.code.data(), attr_info, code_size); attr_info += code_size;
            // Read the exception tables
            uint16_t exception_table_length = be16toh(*((uint16_t*)(attr_info))); attr_info += 2;
            for(size_t x = 0; x < exception_table_length; x++) {
                CodeAttribute::ExceptionTable exc;
                exc.start_pc = be16toh(*((uint16_t*)(attr_info))); attr_info += 2;
                exc.end_pc = be16toh(*((uint16_t*)(attr_info))); attr_info += 2;
                exc.handler_pc = be16toh(*((uint16_t*)(attr_info))); attr_info += 2;
                exc.catch_type = be16toh(*((uint16_t*)(attr_info))); attr_info += 2;
                code.exception_tables.push_back(exc);
            }
            // Read the attributes
            uint16_t attributes_length = be16toh(*((uint16_t*)(attr_info))); attr_info += 2;
            for(size_t x = 0; x < attributes_length; x++) {
                Attribute a;
                a.name = readAndVerifyCPEntry(be16toh(*((uint16_t*)(attr_info))), CPTag::UTF8).string; attr_info += 2;
                uint32_t a_size = be32toh(*((uint32_t*)(attr_info))); attr_info += 4;
                a.info.resize(a_size);
                memcpy(a.info.data(), attr_info, a_size);
                attr_info += a_size;
                code.attributes.push_back(a);
            }
            m.code = code;
        }
        methods.push_back(m);
    }
    return methods;
}

Method& Class::findMethod(std::string name, std::string descriptor) {
    auto find_string = [name, descriptor](Method& m) { return m.name == name && m.descriptor == descriptor; };
    auto ret = std::find_if(methods.begin(), methods.end(), find_string);
    pigeon_assert(ret != std::end(methods));
    return *ret;
}

std::variant<int32_t, int64_t, float, double, Object*>& Class::getField(std::string name) {
    // Get the var
    auto var = field_storage.find(name);
    pigeon_assert(var != std::end(field_storage));
    return var->second;
}

void Class::setField(std::string name, std::variant<int32_t, int64_t, float, double, Object *> value) {
    field_storage[name] = value;
}

CPInfo &Class::readAndVerifyCPEntry(uint16_t pos, CPTag tag) {
    CPInfo& ret = constant_pool[pos - 1];
    if(ret.tag != tag) {
        CPInfo printer;
        printer.tag = tag;
        std::cout << "Got wrong tag: expected " << printer.toString() << ", got " << ret.toString() << "\n";
        std::cout << "Index: #" << pos << "\n";
    }
    pigeon_assert(ret.tag == tag);
    return ret;
}

void Class::dumpCP() {
    for(size_t i = 0; i < constant_pool.size(); i++) {
        CPInfo& info = constant_pool[i];
        pigeon_log("Class", "CP[" << i + 1 << "]: " << info.toString());
        switch(info.tag) {
        case CPTag::FieldReference:
        case CPTag::MethodReference: {
            pigeon_log("Class", "\t" << readAndVerifyCPEntry(readAndVerifyCPEntry(info.ClassReference, CPTag::ClassReference).ClassReference, CPTag::UTF8).string);
            pigeon_log("Class", "\t" << readAndVerifyCPEntry(readAndVerifyCPEntry(info.NameAndTypeDescriptor, CPTag::NameAndTypeDescriptor).NameStringIndex, CPTag::UTF8).string);
            pigeon_log("Class", "\t" << readAndVerifyCPEntry(readAndVerifyCPEntry(info.NameAndTypeDescriptor, CPTag::NameAndTypeDescriptor).TypeStringIndex, CPTag::UTF8).string);
            break;
        }
        case CPTag::UTF8: {
            pigeon_log("Class", "\t" << info.string);
            break;
        }
        }
    }
}

void Class::dumpMethods() {
    for(size_t i = 0; i < methods.size(); i++) {
        Method& m = methods[i];
        pigeon_log("Class", "Method " << i << ": " << m.name << ":" << m.descriptor);
        pigeon_log("Class", "\tMax Stack: " << m.code.max_stack
                  << "\n\tMax Locals: " << m.code.max_locals
                  << "\n\tCode: \n\t[ " << std::hex);
        for(size_t y = 0; y < m.code.code.size(); y++) {
            std::cout << (int)m.code.code[y] << " ";
        }
        pigeon_log("Class", std::dec << " ]" <<
                     "\n\tAttributes: ");
        for(size_t y = 0; y < m.code.attributes.size(); y++) {
            pigeon_log("Class", "\n\t\tattr[0]: " << m.code.attributes[y].name);
        }
    }
}

void Class::dumpAttributes() {
    for(size_t i = 0; i < attributes.size(); i++) {
        Attribute& attr = attributes[i];
        pigeon_log("Class", "attributes[" << i << "]: " << attr.name << "\n");
    }
}
