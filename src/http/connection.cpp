#include "../../include/http/connection.hpp"
#include "../../include/http/parser.hpp"
#include "../../include/utils/static_file_handler.hpp"

#include <cerrno>
#include <stdexcept>
#include <sys/socket.h>
#include <utility>
#include <iostream>

Connection::Connection(int fd) : client_(fd) {

                                 };

void Connection::handle()
{
    while (true)
    {
        try
        {
            std::string raw_request = read_request();
            if (raw_request.empty())
            {
                std::cout<<"Client Disconnected\n";
                return;
            }
            HttpRequest request = HttpParser::parse(raw_request);
            int r_body = read_body(request);
            if (r_body == -1){
                std::cout<<"Client Disconnected\n";
                return;
            }
            StaticFileHandler file_handler("public");
            bool keep_alive = should_keep_alive(request);
            HttpResponse response = file_handler.handle(request);
            if(keep_alive){
                response.set_header("Connection","keep-alive");
            }else response.set_header("Connnection","close");
            
            send_response(response);
            if(!keep_alive){
                std::cout<<"Client Disconnected\n";
                return;
            }
        }
        catch (const std::exception &)
        {
            HttpResponse response;
            response.set_status(400, "Bad Request");
            response.set_header("Content-Type", "text/plain; charset=utf-8");
            response.set_header("Connection", "close");
            response.set_body("400 Bad Request");
            send_response(response);
            return;
        }
    }
}

std::string Connection::read_request()
{
    std::string data = leftover_data_;
    leftover_data_.clear();
    while (true)
    {
        ssize_t bytes_read = recv(client_.fd(), read_buffer_.data(), read_buffer_.size(), 0);
        if (bytes_read == 0)
        {
            return "";
        }
        if (bytes_read < 0)
        {
            if (errno == EINTR)
            { // errno is system variable that holds the code number of error whenever os/system return error
                continue;
            }
            std::cerr << "recv() failed\n";
            return "";
        }
        data.append(read_buffer_.data(), static_cast<std::size_t>(bytes_read));
        size_t end_of_header = data.find("\r\n\r\n"); // size_t is unsigned integer and it's size varies based on system. If compiled on 32 bt system it will have,
        // ssize_t can hold signed integer
        if (end_of_header != std::string::npos)
        {
            leftover_data_ = data.substr(end_of_header + 4);
            return data.substr(0, end_of_header + 4);
        }
        if (data.size() > max_request_size_)
        {
            std::cerr << "Request too large\n";
            return "";
        }
    }
    return data;
}
int Connection::read_body(const HttpRequest &request)
{
    // This funtion discard body, whereas a real one will read the same way and try to use the read data.
    const auto &headers = request.headers();
    auto it = headers.find("Content-Length");
    if (it == headers.end())
        return 0;
    size_t to_read = static_cast<std::size_t>(std::stoul(it->second));
    if (to_read >= leftover_data_.size())
    {
        to_read -= leftover_data_.size();
    }
    else
    {
        leftover_data_ = leftover_data_.substr(to_read);
    }
    while (to_read > 0)
    {
        ssize_t bytes_read = recv(client_.fd(), read_buffer_.data(), read_buffer_.size(), 0);
        if (bytes_read == 0)
        {
            return -1;
        }
        if (bytes_read < 0)
        {
            if (errno == EINTR)
                continue;
            std::cerr << "recv() failed\n";
            return -1;
        }
        if (to_read >= static_cast<std::size_t>(bytes_read))
            to_read -= bytes_read;
        else
        {
            std::string data(read_buffer_.data(), bytes_read);
            leftover_data_ = data.substr(to_read);
            return 0;
        }
    }

    return 0;
}

void Connection::send_response(const HttpResponse &response)
{
    send_all(response.serialize());
}

void Connection::send_all(const std::string &data)
{
    std::size_t total_sent = 0;
    while (total_sent < data.size())
    {
        ssize_t bytes_sent = send(client_.fd(), data.data() + total_sent, data.size() - total_sent, 0);
        if (bytes_sent < 0)
        {
            if (errno == EINTR)
                continue;
            std::cerr << "send() failed\n";
            return;
        }
        total_sent += static_cast<std::size_t>(bytes_sent);
        // bytes_sent is signed whereas total sent is not hence the type conversion.
    }
}

bool Connection::should_keep_alive(const HttpRequest &request) const
{
    const auto &headers = request.headers();

    auto it = headers.find("Connection");

    if (it != headers.end())
    {
        if (it->second == "close")
        {
            return false;
        }

        if (it->second == "keep-alive")
        {
            return true;
        }
    }

    return request.version() == "HTTP/1.1";
}

// sendfile() and DMA