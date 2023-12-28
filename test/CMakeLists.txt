# 时间戳单元测试
add_executable(Timestamp_unittest ${TIMER_SRC} ${CMAKE_SOURCE_DIR}/test/Timestamp_unittest.cpp)

# 日志测试
add_executable(AsyncLoggingTest ${TIMER_SRC} ${THREAD_SRC} ${LOG_SRC} ${CMAKE_SOURCE_DIR}/test/AsyncLoggingTest.cpp)
target_link_libraries(AsyncLoggingTest pthread)

# 线程池测试
add_executable(ThreadPool ${TIMER_SRC} ${THREAD_SRC} ${LOG_SRC} ${CMAKE_SOURCE_DIR}/test/ThreadPool.cpp)
target_link_libraries(ThreadPool pthread)