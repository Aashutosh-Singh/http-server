#include "../../include/http/parser.hpp"
#include <sstream>
#include <stdexcept>
HttpRequest HttpParser::parse(const std::string& raw_request){
    HttpRequest request;
    //method, path,version,headers_
    std::istringstream stream(raw_request);

}