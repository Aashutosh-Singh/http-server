#include "../../include/http/parser.hpp"
#include <sstream>
#include <stdexcept>
#include <iostream>
HttpRequest HttpParser::parse(const std::string& raw_request){
    HttpRequest request;
    //method, path,version,headers_
    std::istringstream stream(raw_request);
    std::string line;
    if(!std::getline(stream,line)){
        throw std::runtime_error("Empty Http request");
    }
    if(!line.empty() && line.back()=='\r'){
        line.pop_back();
    }
    HttpParser:: parse_request_line(line,request);

    while(std::getline(stream,line)){
        if(!line.empty() && line.back()=='\r'){
            line.pop_back();
        }
        if(line.empty())break;
        parse_header_line(line,request);
    }
    
    return request;
    
}

HttpMethod HttpParser::parse_method(const std::string& method){
    HttpMethod meth;
    if(method=="GET"){
        meth=HttpMethod::GET;
    }else if(method=="POST"){
        meth=HttpMethod::POST;
    }else if(method=="PUT"){
        meth=HttpMethod::PUT;
    }else if(method=="DELETE_"){
        meth=HttpMethod::DELETE_;
    }else if(method=="PATCH"){
        meth=HttpMethod::PATCH;
    }else if(method=="HEAD"){
        meth=HttpMethod::HEAD;
    }else if(method=="OPTIONS"){
        meth=HttpMethod::OPTIONS;
    }else{
        meth=HttpMethod::UNKNOWN;
    }
    return meth;
}


void HttpParser::parse_request_line(const std::string& line, HttpRequest& request){
    std::istringstream stream(line);
    std::string method,path,version;
    if(!(stream>>method>>path>>version)){
        throw std::runtime_error("Invalid request line");
    }
    std::cout<<method<<" "<<path<<" "<<version<<"\n";
    request.set_method(parse_method(method));
    request.set_path(path);
    request.set_version(version);

}

void HttpParser:: parse_header_line(const std::string&line, HttpRequest& request){
    std::size_t pos=line.find(':');
    if(pos==std::string::npos){
        return;
    }
    std::string key=line.substr(0,pos);
    std::string value=line.substr(pos+1);
    if(!value.empty() && value.front()==' '){
        value.erase(0,1);
    }
    request.add_header(key,value);
}
//istringstream i=input input from string
//body parsing still left.