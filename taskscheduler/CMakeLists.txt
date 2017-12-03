cmake_minimum_required(VERSION 3.6)
project(os)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp scheduler.c scheduler.h view.h view.c)
add_executable(os ${SOURCE_FILES} scheduler.c scheduler.h view.h view.c)

find_package(Threads)
target_link_libraries (os ${CMAKE_THREAD_LIBS_INIT})

