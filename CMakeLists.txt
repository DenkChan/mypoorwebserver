cmake_minimum_required(VERSION 3.5)
project(server)
project(client)
project(stress_test)
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
    src/ThreadPool.cpp
    server.cpp
)
set(cli_source
    src/InetAddress.cpp
    src/Socket.cpp
    src/Buffer.cpp
    src/util.cpp
    client.cpp
    )

set(test_source
    src/Buffer.cpp 
    src/Socket.cpp
    src/InetAddress.cpp 
    src/ThreadPool.cpp 
    src/util.cpp
    test.cpp
    )


add_executable(server ${source})
add_executable(client ${cli_source})
add_executable(stress_test ${test_source})
find_package(Threads)
target_include_directories(
    server 
        PRIVATE    
            ${PROJECT_SOURCE_DIR}/include
)
# link threads 
target_link_libraries (server ${CMAKE_THREAD_LIBS_INIT}) 
target_include_directories(
    client 
        PRIVATE    
            ${PROJECT_SOURCE_DIR}/include
)

target_include_directories(
    stress_test 
        PRIVATE    
            ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries (stress_test ${CMAKE_THREAD_LIBS_INIT}) 