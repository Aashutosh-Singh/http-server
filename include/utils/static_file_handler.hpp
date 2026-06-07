#pragma once

#include "../http/request.hpp"
#include "../http/response.hpp"
#include <filesystem>
#include<string_view>//to reduce cost of copying string holds pointer and length 
class StaticFileHandler
{
public:
    explicit StaticFileHandler(std::filesystem::path web_root); // constructor
    HttpResponse handle(const HttpRequest& request)const;
    private:
    std::filesystem::path web_root_;

    static std::string_view mime_type(const std::filesystem::path& path);

    static bool is_within_root(const std::filesystem::path& root, const std::filesystem::path&resolved);

    HttpResponse make_error(int code,std::string_view reason,std::string_view body)const;
};

//MIME=multipurpose Internet Mail Extensions. Tells browser or computer what type of data a file contains so it know how to open or render it.