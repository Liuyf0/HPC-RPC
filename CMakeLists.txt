# 设置最低 CMake 版本要求
cmake_minimum_required(VERSION 3.10)

# 设置项目名称
project(CPP11-NetworkLib)

# 设置语言标准编译
set(CMAKE_CXX_STANDARD 11)

# 添加库文件
include_directories(${CMAKE_SOURCE_DIR}/include)

# 设置可执行文件生成目录
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)

# 包含子目录
add_subdirectory(${CMAKE_SOURCE_DIR}/test)
