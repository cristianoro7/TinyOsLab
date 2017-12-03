cmake_minimum_required(VERSION 3.6)
project(jobscheduled)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES jobscheduler.c jobscheduler.h view.h view.c)
add_executable(jobscheduled ${SOURCE_FILES} jobscheduler.c jobscheduler.h view.h view.c)