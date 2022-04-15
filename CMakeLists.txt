cmake_minimum_required(VERSION 3.5)
project(server)
project(client)
set(source 
    src/Epoll.cpp
    src/InetAddress.cpp
    src/Socket.cpp
    src/util.cpp
    src/Channel.cpp
    src/Server.cpp
    src/EventLoop.cpp
    src/Acceptor.cpp
    src/Connection.cpp
    src/Buffer.cpp
    server.cpp
)
set(cli_source
    src/InetAddress.cpp
    src/Socket.cpp
    src/Buffer.cpp
    src/util.cpp
    client.cpp
    )

add_executable(server ${source})
add_executable(client ${cli_source})

target_include_directories(
    server 
        PRIVATE    
            ${PROJECT_SOURCE_DIR}/include
)
target_include_directories(
    client 
        PRIVATE    
            ${PROJECT_SOURCE_DIR}/include
)
