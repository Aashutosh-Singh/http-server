#include "../../include/net/socket.hpp"
#include <unistd.h>
#include <utility> // what is this?

Socket::Socket() noexcept : fd_(-1) {}

Socket::Socket(int fd) noexcept : fd_(fd) {}

Socket::~Socket()
{
    reset();
}
Socket::Socket(Socket &&other) noexcept : fd_(other.release()) {}

 Socket& Socket:: operator=(Socket&& other)noexcept{
    if(this!=&other){
        reset();
        fd_=other.release();
    }
    return *this;
}

int Socket:: fd()const noexcept{
    return fd_;
}

bool Socket:: is_valid()const noexcept{
    return fd_>=0;
}

void Socket:: reset(int fd)noexcept {
    if(fd_>=0){
        close(fd);
    }
    fd_=fd;
}

int Socket:: release()noexcept{
    int temp=fd_;
    fd_=-1;
    return temp;
}

void Socket:: swap(Socket& other)noexcept{
    std::swap(fd_,other.fd_);
}