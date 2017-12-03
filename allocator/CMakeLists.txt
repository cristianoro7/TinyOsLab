cmake_minimum_required(VERSION 3.6)
project(allocator)

set(CMAKE_CXX_STANDARD 11)

set(main.cpp allocator.c allocator.h view.h view.c)
add_executable(allocator ${SOURCE_FILES} allocator.c allocator.h view.h view.c)