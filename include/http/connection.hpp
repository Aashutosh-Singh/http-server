#pragma once
//connection.hpp.

#include "../net/socket.hpp"
#include "../utils/static_file_handler.hpp"
#include "response.hpp"

#include<cstddef> //macros like size_t etc.
#include<string>
#include<array>
class Connection{
public:
    Connection(int fd);
    void handle();
private:
    std::string read_request();
    void send_response (const HttpResponse& response);
    void send_all(const std:: string& data);
    int read_body(const HttpRequest& request);

    bool should_keep_alive(const HttpRequest& request)const;

    Socket client_;
    static constexpr std:: size_t buffer_size_=4096;
    static constexpr std:: size_t max_request_size_= 16*1024;
    //constexpr: this tell compiler that the value won't change hence conpiler replaces buffer_size_ with it's value - optimization.
    //beause of this cpu doesn't waste any cycle looking up for it's value.

    std::array<char,buffer_size_>read_buffer_{};
    std::array<char,buffer_size_>send_buffer_{};
    std::string leftover_data_;
    //array is faster than heap kind vector and better than traddition arr[].

    std:: size_t bytes_read_so_far_=0;
};
