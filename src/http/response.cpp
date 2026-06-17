#include "../../include/http/response.hpp"
#include <sstream> //for serialization


    HttpResponse::HttpResponse() : version_("HTTP/1.1"),
                                   status_code_(200),
                                   reason_phrase_("OK")
    {
    }

    void HttpResponse::set_status(int status_code, std::string reason_phrase)
    {
        status_code_ = status_code;
        reason_phrase_ = std::move(reason_phrase);
    }
    void HttpResponse::set_header(std::string key, std::string value)
    {
        headers_[std::move(key)] = std::move(value);
    }
    void HttpResponse::set_body(std::string body)
    {
        body_ = std::move(body);
        headers_["Content-Length"] = std::to_string(body_.size());
    }

    int HttpResponse::status_code() const noexcept
    {
        return status_code_;
    }
    const std::string &HttpResponse::reason_phrase() const noexcept
    {
        return reason_phrase_;
    }
    const std::string &HttpResponse::body() const noexcept
    {
        return body_;
    }
    const HttpResponse::Headers &HttpResponse::header() const noexcept
    {
        return headers_;
    }

    std::string HttpResponse::serialize() const
    {
        std::ostringstream stream;
        stream << version_ << ' ' << status_code_ << ' ' << reason_phrase_ << "\r\n";
        for (const auto &[name, value] : headers_)
        {
            stream << name << ": " << value << "\r\n";
        }
        stream << "\r\n";
        stream << body_; //you can try to stream only a chunk of the whole body;
        return stream.str();
    }
