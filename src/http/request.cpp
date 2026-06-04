#include "../../include/http/request.hpp"

void HttpRequest:: set_method(HttpMethod method){
    method_=method;
}

void HttpRequest::set_path(std::string path){
    path_=std::move(path);
}

void HttpRequest::set_version(std::string version){
    version_=std::move(version);
}
void HttpRequest:: add_header(
    std::string key,
    std::string value
)
{
    headers_.insert_or_assign(
        std::move(key),
        std::move(value)
    );
}
HttpMethod HttpRequest::method()const noexcept{
    return method_;
}
const std:: string& HttpRequest::path()const noexcept{
    return path_;
}
const std::string& HttpRequest::version()const noexcept{
    return version_;
}
const std:: unordered_map<std::string,std::string>& HttpRequest::headers()const noexcept{
    return headers_;
}

//in http request you get 4 things method, path, version, headers_