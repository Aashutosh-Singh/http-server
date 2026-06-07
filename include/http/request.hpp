#pragma once  //pragma means pragmatic information.

#include<string>
#include<unordered_map>

enum class HttpMethod{
    GET, 
    POST,
    PUT,  //replace
    DELETE_,
    PATCH,  //update partially
    HEAD, //for meta data
    OPTIONS, //cors
    UNKNOWN
};

class HttpRequest{
    public:
    HttpRequest()=default;

    void set_method(HttpMethod method);
    void set_path(std::string path);
    void set_version(std::string version);

    void add_header(
        std:: string key,
        std:: string value
    );
    [[nodiscard]]
    HttpMethod method() const noexcept;  //const here makes a promise to not let the internal state change through this function

    [[nodiscard]]
    const std::string& path()const noexcept; //we are returning refrence and const before that make sure that the retured reference is read only

    [[nodiscard]]
    const std::string& version() const noexcept;

    [[nodiscard]]
    const std:: unordered_map<std::string,std::string>& headers() const noexcept;
    

    private:
        HttpMethod method_ {HttpMethod:: UNKNOWN};
        std::string path_;
        std::string version_;
        std::unordered_map<std::string,std::string>headers_;

};