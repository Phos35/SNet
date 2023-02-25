#include "http_response.h"

HTTPResponse::HTTPResponse(const std::string &version, size_t code,
                const std::string &description)
             :version_(version), code_(code), description_(description)
{

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
    headers_.insert({key, val});
}

void HTTPResponse::set_content_type(MIME type)
{
    headers_["Content-Type"] = mime_to_string(type);
}

void HTTPResponse::add_content(const std::string &content)
{
    content_ = content;
    headers_["Content-Length"] = std::to_string(content_.size());
}

void HTTPResponse::add_content(std::string &&content)
{
    content_ = std::move(content);
    headers_["Content-Length"] = std::to_string(content_.size());
}

std::string HTTPResponse::data()
{
    std::string data = version_ + " ";
    data += std::to_string(code_) + " ";
    data += description_ + "\r\n";

    for(auto header : headers_)
    {
        data += header.first + ": " + header.second + "\r\n";
    }
    data += "\r\n";

    data += content_;
    return data;
}

std::string HTTPResponse::mime_to_string(MIME type)
{
    switch(type)
    {
        case MIME::TEXT: return "text/plain"; break;
        default: return "UNKNOWN MIME"; break;
    }
}