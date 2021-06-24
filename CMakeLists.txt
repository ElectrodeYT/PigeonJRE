cmake_minimum_required(VERSION 3.17)
project(PigeonJRE)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(libzip REQUIRED)
add_executable(PigeonJRE src/class.cpp src/frame.cpp src/jre.cpp src/main.cpp src/thread.cpp)
target_include_directories(PigeonJRE PRIVATE inc)
target_link_libraries(PigeonJRE zip)