cmake_minimum_required(VERSION 3.5)
project(server)

set(source 
    src/Epoll.cpp
    src/InetAddress.cpp
    src/Socket.cpp
    src/util.cpp
    src/Channel.cpp
    src/Server.cpp
    src/EventLoop.cpp
    server.cpp
)

add_executable(server ${source})

target_include_directories(
    server 
        PRIVATE    
            ${PROJECT_SOURCE_DIR}/include
)