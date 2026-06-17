#pragma once
#include "request.hpp"
#include <string>
#include<iostream>
class HttpParser
{
    public:
    [[nodiscard]]
    static HttpRequest parse(const std::string& raw_request);
    //static means you can call this even without creating a object of this class by the name of class.
    private:
    [[nodiscard]]
    static HttpMethod parse_method(const std::string& method);

    static void parse_request_line(const std::string& line, HttpRequest& request);

    static void parse_header_line(const std:: string& line,HttpRequest& request);

};