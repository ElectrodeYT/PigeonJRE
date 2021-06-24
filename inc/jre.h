#pragma once

#include <string>
#include <variant>
#include <algorithm>
#include <zip.h>
#include "class.h"
#include "frame.h"


// JRE Singleton.
// Contains classes, objects, ...
class JRE
{
public:
    static JRE& the() {
        static JRE jre;
        return jre;
    }

    ~JRE() {
        for(size_t i = 0; i < classes.size(); i++) {
            delete classes[i];
        }
        for(size_t i = 0; i < jar_files.size(); i++) {
            zip_close(jar_files[0]);
        }
    }

    // Add a class path
    void addClassPath(const std::string path);

    // Add a JAR file to the class path
    void addJARClassPath(const std::string file);

    // Add a class from a loaded Class Path
    void addClass(const std::string name);

    // Name should be in internal format
    // If it cannot be found, it will be loaded
    Class* findClass(std::string name, bool recursed = false);

    size_t argC(std::string descriptor) {
        char* p = (char*)(descriptor.c_str() + 1);
        size_t ret = 0;
        while(*p != ')') {
            switch(*p) {
            case 'B':
            case 'C':
            case 'I':
            case 'F':
            case 'S':
            case 'Z':
                ret++; break;
            case 'D':
            case 'J':
                ret += 2; break;
            case 'L': {
                ret++;
                while(*++p != ';');
                break;
            }
            }
            p++;
        }
        return ret;
    }

    Object* createObject() {
        Object* new_object = new Object();
        objects.push_back(new_object);
        return new_object;
    }

    bool isMethodStatic(std::string name, std::string descriptor);

private:
    // Classes
    // Contains static classes and classes that can be copied into objects
    std::vector<Class*> classes;
    std::vector<Object*> objects;

    // FS class search paths
    std::vector<std::string> class_search_paths;
    // JAR zip files
    std::vector<struct zip*> jar_files;

    void addClassFromFS(std::string path);
    void addClassFromData(std::vector<uint8_t> data);

    JRE();

};
