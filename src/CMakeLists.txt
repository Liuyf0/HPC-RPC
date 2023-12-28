# server模块
add_executable(server 
    ${EVENTLOOP_SRC} 
    ${TCP_SRC} 
    ${TIMER_SRC} 
    ${THREAD_SRC} 
    ${LOG_SRC} 
    ${BUFFER_SRC}
    ${CHANNEL_SRC}
    ${POLLER_SRC}
    ${HTTP_SRC}         
    ${CMAKE_SOURCE_DIR}/src/main.cpp)
target_link_libraries(server pthread)