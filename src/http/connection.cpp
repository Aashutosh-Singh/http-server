#include "../../include/http/connection.hpp"
#include "../../include/http/parser.hpp"
#include "../../include/utils/static_file_handler.hpp"

#include <cerrno>
#include <stdexcept>
#include <sys/socket.h>
#include <utility>
#include<iostream>




Connection::Connection(int fd):client_(fd){
   
};

void Connection::handle(){
    try{
        std::string raw_request=read_request();
        if(raw_request.empty()){
            return;
        }
        HttpRequest request=HttpParser::parse(raw_request);
        StaticFileHandler file_handler("public");

        HttpResponse response = file_handler.handle(request);
        response.set_header("Connection", "close");
        send_response(response);
    }catch(const std:: exception&){
        HttpResponse response;
        response.set_status(400,"Bad Request");
        response.set_header("Content-Type", "text/plain; charset=utf-8");
        response.set_header("Connection", "close");
        response.set_body("400 Bad Request");
        send_response(response);
    }
    
}

std::string Connection:: read_request(){
    std::string data=leftover_data_;
    leftover_data_.clear();
    while(true){
      ssize_t bytes_read=recv(client_.fd(), read_buffer_.data(),read_buffer_.size(),0); 
        if(bytes_read==0){
            return "";
        }
        if(bytes_read<0){
            if(errno==EINTR){ //errno is system variable that holds the code number of error whenever os/system return error
                continue;
            }
            std::cerr<<"recv() failed\n";
        }
        data.append(read_buffer_.data(),static_cast<std::size_t>(bytes_read));
        size_t end_of_header=data.find("\r\n\r\n")!=std::string::npos;
        if(data.size()>max_request_size_){
            std::cerr<<"Request too large\n";
            return "";
        }
    }
    return data;
}

void Connection::send_response(const HttpResponse& response){
    send_all(response.serialize());
}

void Connection::send_all(const std::string&data){
    std:: size_t total_sent=0;
    while(total_sent<data.size()){
        ssize_t bytes_sent=send(client_.fd(),data.data()+total_sent,data.size()-total_sent,0);
        if(bytes_sent<0){
            if(errno==EINTR)continue;
            std::cerr<<"send() failed\n";
            return ;
        }
        total_sent+=static_cast<std::size_t>(bytes_sent);
        //bytes_sent is signed whereas total sent is not hence the type conversion.
    }
}

bool Connection::request_headers_complete(const std::string& data){
    return data.find("\r\n\r\n")!=std::string::npos;
}

//sendfile() and DMA