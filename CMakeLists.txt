# 设置最低 CMake 版本要求
cmake_minimum_required(VERSION 3.10)

# 设置项目名称
project(CPP11-NetworkLib)

# 设置编译选项
set(CXX_FLAGS
    -g
    -Wall
    -std=c++11
    )

# 添加头文件
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/include/log)
include_directories(${CMAKE_SOURCE_DIR}/include/Thread)
include_directories(${CMAKE_SOURCE_DIR}/include/Timer)
include_directories(${CMAKE_SOURCE_DIR}/include/HTTP)
include_directories(${CMAKE_SOURCE_DIR}/include/Buffer)
include_directories(${CMAKE_SOURCE_DIR}/include/TCP)
include_directories(${CMAKE_SOURCE_DIR}/include/EventLoop)
include_directories(${CMAKE_SOURCE_DIR}/include/Channel)
include_directories(${CMAKE_SOURCE_DIR}/include/Poller)

# 添加源文件
aux_source_directory(${CMAKE_SOURCE_DIR}/src/log LOG_SRC)
aux_source_directory(${CMAKE_SOURCE_DIR}/src/Thread THREAD_SRC)
aux_source_directory(${CMAKE_SOURCE_DIR}/src/Timer TIMER_SRC)
aux_source_directory(${CMAKE_SOURCE_DIR}/src/Buffer BUFFER_SRC)
aux_source_directory(${CMAKE_SOURCE_DIR}/src/HTTP HTTP_SRC)
aux_source_directory(${CMAKE_SOURCE_DIR}/src/TCP TCP_SRC)
aux_source_directory(${CMAKE_SOURCE_DIR}/src/EventLoop EVENTLOOP_SRC)
aux_source_directory(${CMAKE_SOURCE_DIR}/src/Channel CHANNEL_SRC)
aux_source_directory(${CMAKE_SOURCE_DIR}/src/Poller POLLER_SRC)

# 设置可执行文件生成目录
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)

# 包含子目录
add_subdirectory(${CMAKE_SOURCE_DIR}/test)
add_subdirectory(${CMAKE_SOURCE_DIR}/src)
