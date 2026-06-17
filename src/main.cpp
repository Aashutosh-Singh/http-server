#include "../include/net/server.hpp"
#include<exception>
#include<iostream>

int main(){
    try{
        Server server(8080);
        server.start();
    }catch(const std::exception& error){
        std::cerr<<error.what()<<'\n';
        return -1;
    }
    return 0;
}