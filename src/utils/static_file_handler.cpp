#include "../../include/utils/static_file_handler.hpp"
#include <fstream> //to operate with files
#include <iterator>
#include <unordered_map>
StaticFileHandler::StaticFileHandler(std::filesystem::path web_root) : web_root_(std::filesystem::weakly_canonical(std::move(web_root))) {}
// weakly_canonical converts path into absolute,nique,cleanest version of path resolving .or../ etc. it works even when path doesn't exits on computer, hence doesn't crashes like filesytem::canonical();


// helpers
bool StaticFileHandler::is_within_root(const std::filesystem::path &root, const std::filesystem::path &resolved)
{
    auto [root_end, _] = std::mismatch(root.begin(), root.end(), resolved.begin(), resolved.end());
    return root_end == root.end();
}
// fun fact _ is common programmig convetion that means "I am forced to extract this variable to make the code compile, but I don't care about it and will never use it."
std::string_view StaticFileHandler::mime_type(const std::filesystem::path &path)
{   //static creates this map ones when program loads and closes when program end for performance
    static const std::unordered_map<std::string, std::string_view>table={
        { ".html",  "text/html; charset=utf-8"       },
        { ".htm",   "text/html; charset=utf-8"       },
        { ".css",   "text/css; charset=utf-8"        },
        { ".js",    "application/javascript"         },
        { ".json",  "application/json"               },
        { ".png",   "image/png"                      },
        { ".jpg",   "image/jpeg"                     },
        { ".jpeg",  "image/jpeg"                     },
        { ".gif",   "image/gif"                      },
        { ".svg",   "image/svg+xml"                  },
        { ".ico",   "image/x-icon"                   },
        { ".txt",   "text/plain; charset=utf-8"      },
        { ".pdf",   "application/pdf"                },
        { ".woff",  "font/woff"                      },
        { ".woff2", "font/woff2"                     },
        { ".webp",  "image/webp"                     },
        { ".pdf",   "application/pdf"                },
    };
    auto ext=path.extension().string();
    for(auto& c:ext)c=static_cast<char>(std::tolower(c));

    auto it=table.find(ext);
    return it!=table.end()? it->second : "application/octet-stream";

}

HttpResponse StaticFileHandler::make_error(int code,std::string_view reason, std::string_view body)const{
    HttpResponse r;
    r.set_status(code,std::string(reason));
    r.set_header("Content-Type","text/plain; charset=utf-8");
    r.set_header("Content-Length", std::to_string(body.size()));
    r.set_body(std::string(body));
    return r;
}

//The main handler...
HttpResponse StaticFileHandler::handle(const HttpRequest& request)const{
    std::string url_path=request.path();
    if(auto q=url_path.find('?'); q!=std::string::npos)url_path.resize(q);

    if(url_path.empty() || url_path[0]!='/'){
        return make_error(400,"Bad Request","400 Bad Request");
    }
    std::filesystem::path relative=url_path.substr(1);
    std::filesystem::path candidate=web_root_/relative;
    std::error_code ec;
    if(std::filesystem::is_directory(candidate,ec))candidate/= "index.html";

    std::filesystem::path resolved=std::filesystem::weakly_canonical(candidate,ec);
    if(ec){
        return make_error(500,"Internal Sever Error","500 Internal Server Error");
    }
    if(!is_within_root(web_root_, resolved))return make_error(403, "Forbidden","403 Forbidden");

    if(!std::filesystem::exists(resolved,ec)|| !std::filesystem::is_regular_file(resolved,ec)){
        return make_error(404,"Not Found","404 Not Found");
    }

    std::ifstream file(resolved, std::ios::binary);
    if(!file){
        return make_error(500,"internal Server Error","500 Internal Sever Error");
    }
    std::string body(
        std::istreambuf_iterator<char>(file),
        std::istreambuf_iterator<char>{}
    );
    if(file.fail()&&!file.eof()){
        return make_error(500, "Internal Server Error", "500 Internal Server Error");
    }

    HttpResponse response;
    response.set_status(200, "OK");
    response.set_header("Content-Type", std::string(mime_type(resolved)));
    response.set_header("Content-Length", std::to_string(body.size()));
    response.set_body(std::move(body));
    return response;
}