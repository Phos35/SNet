#include "http_response.h"

std::unordered_map<std::string, std::string>
HTTPResponse::mime_map = 
{
    {"txt", "text/plain"}, {"html", "text/html"},
    {"js", "application/javascript"}, {"png", "iage/png"}, 
    {"jpg", "image/jpg"}, {"gif", "image/gif"},
    {"ico", "image/*"}
};

HTTPResponse::HTTPResponse(const std::string &version, size_t code,
                const std::string &description)
:version_(version), code_(code), description_(description)
{

}

HTTPResponse::HTTPResponse(HTTPResponse &&another)
{
    version_.swap(another.version_);
    code_ = another.code_;
    headers_.swap(another.headers_);
    headers_str_.swap(another.headers_str_);
    content_.swap(another.content_);
}

HTTPResponse& HTTPResponse::operator=(HTTPResponse &&another)
{
    version_.swap(another.version_);
    code_ = another.code_;
    headers_.swap(another.headers_);
    headers_str_.swap(another.headers_str_);
    content_.swap(another.content_);
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
    headers_[key] = val;
    headers_str_ += (key + ": " + val + "\r\n");
}

void HTTPResponse::set_content_type(MIME type)
{
    std::string mime_str = mime_to_string(type);
    set_header("Content-Type", mime_str);
}

void  HTTPResponse::set_content_type(const std::string &type)
{
    set_header("Content-Type", type);
}

std::string HTTPResponse::judge_mime_type(const std::string &url)
{
    // 找到文件后缀名
    int dot_index = url.find_last_of('.');
    std::string suffix = url.substr(dot_index + 1);
    
    // 查表搜寻
    auto itr = mime_map.find(suffix);
    if(itr != mime_map.end())
        return itr->second;
    else
        return "text/" + suffix;
}

void HTTPResponse::add_content(const std::string &content)
{
    content_ = content;
    set_header("Content-Length", std::to_string(content_.size()));
}

void HTTPResponse::add_content(std::string &&content)
{
    content_ = std::move(content);
    set_header("Content-Length", std::to_string(content_.size()));
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