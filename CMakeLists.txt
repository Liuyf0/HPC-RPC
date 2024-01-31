cmake_minimum_required(VERSION 3.16.3)
project(Cpp11-NetworkLib)

set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++11 -fPIC")

# 添加头文件
include_directories(${CMAKE_SOURCE_DIR}/include)

# 添加源文件
aux_source_directory(${CMAKE_SOURCE_DIR}/src SRC_LIST)

add_library(netlib SHARED ${SRC_LIST})
