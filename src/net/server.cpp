#include "../../include/net/server.hpp"
#include "../../include/http/connection.hpp"
#include "../../include/utils/thread_pool.hpp"
#include <arpa/inet.h>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <stdexcept> //for error types to use in try and throw
#include <sys/socket.h>
#include <unistd.h>

Server::Server(std::uint16_t port) : port_(port) {}

void Server::start()
{
    create_socket();
    configure_socket();
    bind_socket();
    start_listening();
    ThreadPool pool(4);
    std::cout << "Server listening on port " << port_ << '\n';
    while (true)
    {
        sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        int client_fd = accept(
            listen_socket_.fd(),
            (sockaddr *)&client_addr,
            &client_len);

        if (client_fd < 0)
        {
            std::cerr << "accept() failed\n";
            continue;
        }

        std::cout << "Client connected \n";
        pool.submit([client_fd](){
            Connection conn(client_fd);
            conn.handle();
        });
    }
}

void Server::create_socket(){
    int fd=socket(AF_INET,SOCK_STREAM,0);
    if(fd<0){
        throw std::runtime_error("socket() failed");
    }
    listen_socket_.reset(fd);
}
//has to done before bind
void Server::configure_socket(){
    int opt=1;
    if(setsockopt(
        listen_socket_.fd(),
        SOL_SOCKET,
        SO_REUSEADDR,
        &opt,
        sizeof(opt)
    )<0){
        throw std::runtime_error("Setsockopt() failed");
    }
}

void Server::bind_socket(){
    sockaddr_in addr;
    addr.sin_family=AF_INET;
    addr.sin_addr.s_addr=INADDR_ANY;
    addr.sin_port=htons(port_);
    if(bind(
        listen_socket_.fd(),
        (sockaddr*)&addr,
        sizeof(addr)
    )<0){
        throw std::runtime_error("bind() failed ");
    }
}

void Server::start_listening(){
    constexpr int backlog=128;
    if(listen(listen_socket_.fd(),backlog)<0){
        throw std:: runtime_error("listen() failed");
    }
}