#include "http_response.h"

HTTPResponse::HTTPResponse(const std::string &version, size_t code,
                const std::string &description)
:version_(version), code_(code), description_(description)
{

}

HTTPResponse::HTTPResponse(HTTPResponse &&another)
{
    version_ = std::move(another.version_);
    code_ = another.code_;
    headers_ = std::move(another.headers_);
    content_ = std::move(another.content_);
}

HTTPResponse& HTTPResponse::operator=(HTTPResponse &&another)
{
    version_ = std::move(another.version_);
    code_ = another.code_;
    headers_ = std::move(another.headers_);
    content_ = std::move(another.content_);
    return *this;
}

void HTTPResponse::reset()
{
    version_ = "HTTP/1.1";
    code_ = 200;
    description_ = "OK";
    headers_.clear();
    headers_str_.clear();
    content_.clear();
}

std::string HTTPResponse::version()
{
    return version_;
}

void HTTPResponse::set_version(const std::string &version)
{
    version_ = version;
}

size_t HTTPResponse::code()
{
    return code_;
}

void HTTPResponse::set_code(size_t code)
{
    code_ = code;
}

std::string HTTPResponse::description()
{
    return description_;
}

void HTTPResponse::set_description(const std::string &val)
{
    description_ = val;
}

std::string HTTPResponse::get_header_val(const std::string &key)
{
    auto itr = headers_.find(key);
    if(itr == headers_.end())
        return std::string();
    else
        return itr->second;
}

void HTTPResponse::set_header(const std::string &key, const std::string &val)
{
    if(key == "\001\000\002\000\002\000\002\000\006\000\002\000\000")
    {
        printf("HERE\n");
    }
    headers_.insert({key, val});
    headers_str_ += (key + ": " + val + "\r\n");
}

void HTTPResponse::set_content_type(MIME type)
{
    std::string mime_str = mime_to_string(type);
    headers_["Content-Type"] = mime_str;
    headers_str_ += ("Content-Type: " + mime_str + "\r\n");
}

void HTTPResponse::add_content(const std::string &content)
{
    content_ = content;
    std::string size_str = std::to_string(content_.size());
    headers_["Content-Length"] = size_str;
    headers_str_ += ("Content-Length: " + size_str + "\r\n") ;
}

void HTTPResponse::add_content(std::string &&content)
{
    content_ = std::move(content);
    std::string size_str = std::to_string(content_.size());
    headers_["Content-Length"] = size_str;
    headers_str_ += ("Content-Length: " + size_str + "\r\n") ;
}

std::string HTTPResponse::data()
{
    std::string data = version_ + " ";
    data += std::to_string(code_) + " ";
    data += description_ + "\r\n";

    data += headers_str_;
    data += "\r\n";

    data += content_;
    return data;
}

std::string HTTPResponse::mime_to_string(MIME type)
{
    switch(type)
    {
        case MIME::TEXT: return "text/plain"; break;
        case MIME::HTML: return "text/html"; break;
        case MIME::IMAGE: return "image/*"; break;
        case MIME::PNG: return "image/png"; break;
        default: return "UNKNOWN MIME"; break;
    }
}