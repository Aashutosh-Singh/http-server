#pragma once
#include <string>
#include <unordered_map>

class HttpResponse
{
    using Headers = std::unordered_map<std::string, std::string>;

private:
    std::string version_;
    int status_code_;
    std::string reason_phrase_;
    Headers headers_;
    std::string body_;

public:
    HttpResponse();

    void set_status(int status_code, std::string reason_phrase);
    void set_header(std::string key, std::string value);
    void set_body(std::string body);

    [[nodiscard]]
    const std::string &reason_phrase() const noexcept;

    [[nodiscard]]
    const int status_code() const noexcept;

    [[nodiscard]]
    const std::string &body() const noexcept;

    [[nodiscard]]
    const Headers &header() const noexcept;
    // first const say that the returned output cannot be changed and second say that the function won't change any value;

    [[nodiscard]]
    std::string serialize() const;
};