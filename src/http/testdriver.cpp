#include "../../include/http/parser.hpp"
#include "parser.cpp"
#include "request.cpp"
#include<iostream>
int main(){
    std::cout<<"The program is runnign";
    std::string http_req=
         "GET /index.html HTTP/1.1\r\n"
        "Host: example.com\r\n"
        "User-Agent: curl\r\n"
        "\r\n";
        
        HttpRequest request;
        request=HttpParser::parse(http_req);
        HttpMethod meth;
        meth=request.method();
        
         //std::cout << request.method()<< '\n';
        std::cout << request.path() << '\n';
        std::cout << request.version() << '\n';
}