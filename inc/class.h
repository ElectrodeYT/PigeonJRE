#pragma once

#include <cstring>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <variant>
#include <vector>
#include "debug.h"

// Nicely stolen from stack overflow
#if defined(__linux__)

#include <endian.h>

#elif defined(__FreeBSD__) || defined(__NetBSD__)
#include <sys/endian.h>
#elif defined(__OpenBSD__)
#include <sys/types.h>
#define be16toh(x) betoh16(x)
#define be32toh(x) betoh32(x)
#define be64toh(x) betoh64(x)
#endif

// Forward defintion
class Class;

class Object;

enum CPTag { UTF8 = 1, Integer = 3, Float = 4, Long = 5, Double = 6, ClassReference = 7, StringReference = 8, FieldReference = 9, MethodReference = 10, InterfaceMethodReference = 11, NameAndTypeDescriptor = 12, MethodHandle = 15, MethodType = 16, Dynamic = 17, InvokeDynamic = 18, Module = 19, Package = 20 };

// A Constant Pool Entry
struct CPInfo {
  CPTag tag;
  std::string string;
  union {
    uint64_t Long;
    uint32_t Integer;
    float Float;
    double Double;
  };
  uint16_t ClassReference;
  uint16_t StringReference;
  uint16_t NameAndTypeDescriptor;

  // For the Name and Type Descriptor
  uint16_t NameStringIndex;
  uint16_t TypeStringIndex;

  std::string toString() {
    switch (tag) {
      case CPTag::UTF8: {
	return "UTF8";
      }
      case CPTag::Integer: {
	return "Integer";
      }
      case CPTag::Float: {
	return "Float";
      }
      case CPTag::Long: {
	return "Long";
      }
      case CPTag::Double: {
	return "Double";
      }
      case CPTag::ClassReference: {
	return "ClassReference";
      }
      case CPTag::StringReference: {
	return "StringReference";
      }
      case CPTag::FieldReference: {
	return "FieldReference";
      }
      case CPTag::MethodReference: {
	return "MethodReference";
      }
      case CPTag::InterfaceMethodReference: {
	return "InterfaceMethodReference";
      }
      case CPTag::NameAndTypeDescriptor: {
	return "NameAndTypeDescriptor";
      }
      case CPTag::MethodHandle: {
	return "MethodHandle";
      }
      case CPTag::MethodType: {
	return "MethodType";
      }
      case CPTag::Dynamic: {
	return "Dynamic";
      }
      case CPTag::InvokeDynamic: {
	return "InvokeDynamic";
      }
      case CPTag::Module: {
	return "Module";
      }
      case CPTag::Package: {
	return "Package";
      }
    }
    return "Invalid";
  }
};

// Attribute struct
// Defined in jvms-4 4.7
struct Attribute {
  std::string name;
  std::vector<uint8_t> info;
};

// Code attribute
struct CodeAttribute {
  struct ExceptionTable {
    uint16_t start_pc;
    uint16_t end_pc;
    uint16_t handler_pc;
    uint16_t catch_type;
  };

  uint16_t max_stack;
  uint16_t max_locals;
  std::vector<uint8_t> code;
  std::vector<ExceptionTable> exception_tables;
  std::vector<Attribute> attributes;
};

// General field structure
struct Field {
  uint16_t access_flags;
  std::string name;
  std::string descriptor;
  std::vector<Attribute> attributes;
};

struct Method {
  Method(Class* c) : method_class(c) {}

  uint16_t access_flags;
  std::string name;
  std::string descriptor;
  std::vector<Attribute> attributes;
  CodeAttribute code;
  Class* method_class;

  enum AccessFlags { Public = 0x0001, Private = 0x0002, Protected = 0x0004, Static = 0x0008, Final = 0x0010, Synchronized = 0x0020, Bridge = 0x0040, Varargs = 0x0080, Native = 0x0100, Abstract = 0x0400, Strict = 0x0800, Synthetic = 0x1000 };
};

class Class {
 public:
  // Empty constructor.
  // Used for native classes.
  Class() { isNative = true; }

  // Load a class file from disk.
  Class(std::string file_name);

  // Load a class file from raw data.
  Class(std::vector<uint8_t> data);

  Class* clone() {
    Class* new_class = new Class();
    new_class->isNative = isNative;
    new_class->constant_pool = constant_pool;
    new_class->interfaces = interfaces;
    new_class->class_field_list = class_field_list;
    new_class->methods = methods;
    new_class->attributes = attributes;
    new_class->name = name;
    new_class->super_name = name;

    new_class->file_major_version = file_major_version;
    new_class->file_minor_version = file_minor_version;

    return new_class;
  }

  // Find a method in this class
  Method& findMethod(std::string name, std::string descriptor);

  // Find a field in this class
  std::variant<int32_t, int64_t, float, double, Object*>& getField(std::string name);

  void setField(std::string name, std::variant<int32_t, int64_t, float, double, Object*> value);

  CPInfo& readAndVerifyCPEntry(uint16_t pos, CPTag tag);

  CPInfo& readCPEntry(uint16_t pos) { return constant_pool[pos - 1]; }

  void dumpCP();

  void dumpMethods();

  void dumpAttributes();

  std::vector<CPInfo> constant_pool;
  std::vector<uint16_t> interfaces;
  std::vector<Field> class_field_list;
  std::vector<Method> methods;
  std::vector<Attribute> attributes;

  bool isNative = false;

  std::string name = "";
  std::string super_name = "";

 private:
  // Actual implementation of Class(std::vector<uint8_t> data)
  void loadData();

  std::map<std::string, std::variant<int32_t, int64_t, float, double, Object*>> field_storage;

  // Class data
  std::vector<uint8_t> file_data;
  // Current pointer into file_data
  size_t file_data_pointer = 0;

  // File major version
  uint16_t file_major_version;
  // File minor version
  uint16_t file_minor_version;

  // Class access flags
  uint16_t access_flags;
  // Class "this" class index
  uint16_t this_class_index;
  // Class "super" class index
  uint16_t super_class_index;

  uint8_t readByte() {
    pigeon_assert(file_data.size() - file_data_pointer >= 1);
    return file_data[file_data_pointer++];
  }

  uint16_t readShort() {
    uint16_t ret;
    pigeon_assert(file_data.size() - file_data_pointer >= 2);
    ret = *((uint16_t*)(file_data.data() + file_data_pointer));
    file_data_pointer += 2;
    ret = be16toh(ret);
    return ret;
  }

  uint32_t readInt() {
    uint32_t ret;
    pigeon_assert(file_data.size() - file_data_pointer >= 4);
    ret = *((uint32_t*)(file_data.data() + file_data_pointer));
    file_data_pointer += 4;
    ret = be32toh(ret);
    return ret;
  }

  uint64_t readLong() {
    uint64_t ret;
    pigeon_assert(file_data.size() - file_data_pointer >= 8);
    ret = *((uint64_t*)(file_data.data() + file_data_pointer));
    file_data_pointer += 8;
    ret = be64toh(ret);
    return ret;
  }

  float readFloat() {
    float ret;
    pigeon_assert(file_data.size() - file_data_pointer >= sizeof(float));
    ret = *((float*)(file_data.data() + file_data_pointer));
    file_data_pointer += sizeof(float);
    return ret;
  }

  double readDouble() {
    double ret;
    pigeon_assert(file_data.size() - file_data_pointer >= sizeof(double));
    ret = *((double*)(file_data.data() + file_data_pointer));
    file_data_pointer += sizeof(double);
    return ret;
  }

  std::vector<Attribute> readAttributes();

  std::vector<Field> readFieldStruct();

  std::vector<Method> readMethods();
};
