#pragma once
#include "socket.hpp"

#include <cstdint>

class Server{
    public:
     explicit Server(std::uint16_t port);
     void start();
     
     private:
        void create_socket();
        void configure_socket();
        void bind_socket();
        void start_listening();

        //data
        Socket listen_socket_;
        std:: uint16_t port_;
};