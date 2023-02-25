#include "http_request.h"

std::unordered_map<std::string, HTTPRequest::Method> HTTPRequest::string_method_table =
{
    {"GET", HTTPRequest::Method::GET}, {"HEAD", HTTPRequest::Method::HEAD},
    {"POST", HTTPRequest::Method::POST}, {"PUT", HTTPRequest::Method::PUT},
    {"DELETE", HTTPRequest::Method::DELETE}, {"CONNECT", HTTPRequest::Method::CONNECT},
    {"OPTIONS", HTTPRequest::Method::OPTIONS}, {"TRACE", HTTPRequest::Method::TRACE},
    {"PATCH", HTTPRequest::Method::PATCH}, 
};

HTTPRequest::HTTPRequest(const std::string& data)
:Message(data)
{

}

HTTPRequest::Method HTTPRequest::get_method()
{
    return method_;
}

void HTTPRequest::set_method(Method method)
{
    method_ = method;
}

std::string HTTPRequest::get_method_str()
{
    return method_to_string(method_);
}

void HTTPRequest::set_method(std::string method)
{
    method_ = string_to_method(method);
}

HTTPRequest::URL HTTPRequest::get_url()
{
    return url_;
}

void HTTPRequest::set_url(const std::string &url)
{
    url_ = url;
}

HTTPRequest::Version HTTPRequest::get_version()
{
    return version_;
}

void HTTPRequest::set_version(Version version)
{
    version_ = version_;
}

std::string HTTPRequest::get_header_val(const std::string &key)
{
    auto itr = headers_.find(key);
    if (itr == headers_.end())
        return std::string();
    else 
        return itr->second;
}

void HTTPRequest::add_header(const std::string &key, const std::string &val)
{
    headers_[key] = val;
}

HTTPRequest::Content& HTTPRequest::content_ref()
{
    return content_;
}

void HTTPRequest::set_content(const std::string& content)
{
    content_ = content;
}

HTTPRequest::QueryMap& HTTPRequest::queries_ref()
{
    return queries_;
}

HTTPRequest::Error HTTPRequest::error()
{
    return error_;
}

void HTTPRequest::set_error(Error error)
{
    error_ = error;
}

std::string HTTPRequest::to_string()
{
    std::string msg;
    msg += error_to_string(error_) + "\n";

    msg += method_to_string(method_) + " ";
    msg += url_ + " ";
    msg += version_ + "\n";

    for (auto itr = headers_.begin(); itr != headers_.end(); ++itr)
    {
        msg += itr->first + ": " + itr->second + "\n";
    }
    msg += "\n";

    msg += content_;
    return msg;
}

std::string HTTPRequest::method_to_string(Method method)
{
    switch(method)
    {
        case GET: return "GET"; break;
        case HEAD: return "HEAD"; break;
        case POST: return "POST"; break;
        case PUT: return "PUT"; break;
        case DELETE: return "DELETE"; break;
        case CONNECT: return "CONNECT"; break;
        case OPTIONS: return "OPTIONS"; break;
        case TRACE: return "TRACE"; break;
        case PATCH: return "PATCH"; break;
        default: return "UNKNOWN"; break;
    }
}

HTTPRequest::Method HTTPRequest::string_to_method(const std::string &method_str)
{
    auto itr = string_method_table.find(method_str);
    if(itr == string_method_table.end())
        return Method::UNKNOWN;
    else
        return itr->second;
}

std::string HTTPRequest::error_to_string(Error error)
{
    switch(error)
    {
        case Error::NO_ERROR: return "NO_ERROR"; break;
        case Error::UNKNOWN_ERROR: return "UNKNOWN_ERROR"; break;
        case Error::BAD_LINE: return "BAD_LINE"; break;
        case Error::UNCOMPLETE: return "UNCOMPLETE"; break;
        case Error::BAD_METHOD: return "BAD_METHOD"; break;
        case Error::BAD_URL: return "BAD_URL"; break;
        case Error::BAD_VERSION: return "BAD_VERSION"; break;
        case Error::BAD_HEADER: return "BAD_HEADER"; break;
        default: return "DEFAULT"; break;
    }
}

bool HTTPRequest::method_valid(const std::string& method)
{
    return string_method_table.find(method) != string_method_table.end();
}

bool HTTPRequest::version_valid(const std::string& version)
{
    return version == "HTTP/1.0" || version == "HTTP/1.1";
}