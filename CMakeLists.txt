cmake_minimum_required(VERSION 3.8)
project(untitled)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

set(SOURCE_FILES main.cpp source/http.cpp headers/http.h source/ThreadPool.cpp headers/ThreadPool.h headers/IThreadTask.h source/Handler.cpp headers/Handler.h source/HandlerTask.cpp headers/HandlerTask.h source/Server.cpp headers/Server.h)
add_executable(untitled ${SOURCE_FILES})