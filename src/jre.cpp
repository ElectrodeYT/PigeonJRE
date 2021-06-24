#include <cstdarg>
#include <sys/stat.h>
#include <linux/limits.h>
#include <jre.h>
#include <frame.h>

JRE::JRE() {

}

void JRE::addClassFromFS(std::string path) {
    Class* c = new Class(path);
    // We need to check if there is a super class to load as well
    if(c->super_name != "java/lang/Object" && c->super_name != "") {
        addClass(c->super_name);
    }
    classes.push_back(c);
}

void JRE::addClassFromData(std::vector<uint8_t> data) {
    Class* c = new Class(data);
    // We need to check if there is a super class to load as well
    if(c->super_name != "java/lang/Object" && c->super_name != "") {
        addClass(c->super_name);
    }
    classes.push_back(c);
}

void JRE::addClassPath(std::string path) {
    struct stat s;
    if(stat(path.c_str(), &s) == 0) {
        if(s.st_mode & S_IFDIR) {
            // Get the proper path
            char realpath_buffer[PATH_MAX];
            realpath(path.c_str(), realpath_buffer);

            // Add it to the search path
            std::string new_search_path(realpath_buffer);
            if(new_search_path[new_search_path.size() - 1] != '/') { new_search_path.push_back('/'); }
            class_search_paths.push_back(new_search_path);
            pigeon_log("JRE", "Added " << new_search_path << " to the class path");
        } else if(s.st_mode & S_IFREG) {
            // Try to add it as a JAR class path
            pigeon_log("JRE", "Trying to add " << path << " as a JAR class path");
            addJARClassPath(path);
            return;
        } else {
            pigeon_panic("Unexpected stat result in addClassPath()");
        }
    } else {
        pigeon_panic("stat() error in addClassPath()")
    }
}

void JRE::addJARClassPath(const std::string file) {
    int error = 0;
    struct zip* jar_file = zip_open(file.c_str(), ZIP_RDONLY, &error);
    pigeon_assert(jar_file);
    jar_files.push_back(jar_file);
}

void JRE::addClass(std::string name) {
    // File name: since we operate in only internal formatting, we dont need to replace . with /
    std::string file_name = name + ".class";
    // Now its time to find the file in the class paths
    for(size_t i = 0; i < class_search_paths.size(); i++) {
        // Create the full path and stat() it
        std::string full_path = class_search_paths[i] + file_name;
        struct stat s;
        if(stat(full_path.c_str(), &s) == 0 && s.st_mode & S_IFREG) {
            pigeon_log("JRE", "Adding class from FS: " << full_path);
            // Got the file, load it and add it
            addClassFromFS(full_path);
            return;
        }
    }
    // Not found in FS, search the JARS instead
    for(size_t i = 0; i < jar_files.size(); i++) {
        struct zip* file = jar_files[i];
        zip_stat_t file_stat;
        if(zip_stat(file, file_name.c_str(), 0, &file_stat) == -1) continue;
        // File exists, now check if we can get the info we need
        if(!(file_stat.valid & ZIP_STAT_SIZE)) continue;

        // We found the file and we know the size, load it
        zip_file_t* file_in_zip = zip_fopen(file, file_name.c_str(), 0);
        std::vector<uint8_t> file_data;
        file_data.resize(file_stat.size);

        // We can now read the file
        // As libzip does not guarantee that all bytes will be read,
        // we need to keep track of how many we have read.
        // Amount we still need to read
        size_t file_remaining = file_stat.size;
        // Buffer into the vector
        uint8_t* buffer = file_data.data();
        while(file_remaining) {
            // Read up to file_remaining
            size_t amount_read = zip_fread(file_in_zip, buffer, file_remaining);
            // Decrease file_remaining
            file_remaining -= amount_read;
            // Advance the buffer
            buffer += amount_read;
        }
        // We can now add the class
        addClassFromData(file_data);
        pigeon_log("JRE", "Added class from zip file: " << file_name);
        zip_fclose(file_in_zip);
        return;
    }
    pigeon_panic("addClass() failed");
}

Class *JRE::findClass(std::string name, bool recursed) {
    auto find_string = [name](Class* c) { return c->name == name; };
    auto ret = std::find_if(classes.begin(), classes.end(), find_string);
    if(ret == std::end(classes)) {
        if(recursed) { pigeon_panic("findClass recursed and didn't find class!"); }
        pigeon_log("JRE", "Attempting to load class " << name);
        addClass(name);
        return findClass(name, true);
    }
    return *ret;
}

bool JRE::isMethodStatic(std::string name, std::string descriptor) {
    std::string class_name = name.substr(0, name.find_first_of('.'));
    std::string function_name = name.substr(name.find_first_of('.') + 1);

    Class* c = findClass(class_name);
    Method& m = c->findMethod(function_name, descriptor);
    return m.access_flags & Method::AccessFlags::Static;
}
