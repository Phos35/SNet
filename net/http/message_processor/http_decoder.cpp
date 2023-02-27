#include "http_decoder.h"
#include "unordered_set"

HTTPDecoder::HTTPDecoder()
{
    request_ = new HTTPRequest("");
}

HTTPDecoder::~HTTPDecoder()
{
    delete request_;
}

Message* HTTPDecoder::decode(const std::string &data)
{
    // 初始化
    cur_index_ = 0;
    state_ = DecodeState::REQUEST_LINE;
    request_->set_data(data);
    line_start_ = line_end_ = 0;

    // 逐行解析
    DecodeError error = DecodeError::NO_ERROR;
    while (error == DecodeError::NO_ERROR && state_ != DecodeState::FINISH)
    {
        switch (state_)
        {
            case DecodeState::REQUEST_LINE: error = decode_request_line(data); break;
            case DecodeState::HEADERS: error = decode_headers(data); break;
            case DecodeState::CONTENT: error = decode_content(data); break;
            default: error = DecodeError::UNKNOWN_ERROR; break;
        }
        if(error != DecodeError::NO_ERROR || state_ == DecodeState::FINISH)
            break;
    }

    // 报文解析未完成，返回uncomplete，等待后续报文
    if(error == DecodeError::UNCOMPLETE)
    {
        request_->set_result(HTTPRequest::DeocdeResult::FAILURE);
    }

    // 记录报文的错误状态
    request_->set_error(error);

    return request_;
}

DecodeError HTTPDecoder::decode_request_line(const std::string &data)
{
    // 获取一行
    DecodeError error = get_one_line(data);
    if(error != DecodeError::NO_ERROR)
        return error;

    // 解析请求方法
    error = decode_method(data);
    if(error != DecodeError::NO_ERROR)
        return error;
    
    // 解析url
    error = decode_url(data);
    if (error != DecodeError::NO_ERROR)
        return error;
    
    // 解析HTTP协议版本
    error = decode_version(data);
    if(error != DecodeError::NO_ERROR)
        return error;

    // 更新解析状态
    state_ = DecodeState::HEADERS;
    return DecodeError::NO_ERROR;
}

DecodeError HTTPDecoder::decode_method(const std::string &data)
{
    // 寻找空格
    int blank_index = data.find_first_of(' ', cur_index_);
    if(blank_index == std::string::npos || blank_index >= line_end_)
        return DecodeError::BAD_METHOD;
    
    // 获取请求方法并检验
    std::string method = data.substr(cur_index_, blank_index - cur_index_);
    if(HTTPRequest::method_valid(method) == false)
        return DecodeError::BAD_METHOD;
    
    // 更新请求方法，更新cur_index_
    request_->set_method(data.substr(cur_index_, blank_index - cur_index_));
    cur_index_ = blank_index + 1;
    return DecodeError::NO_ERROR;
}

DecodeError HTTPDecoder::decode_url(const std::string &data)
{
    // 寻找空格; 此处需要先寻找分割url和version的空格，
    // 因为后续处理过程中会见url中的+解析成空格
    int blank_index = data.find_first_of(' ', cur_index_);

    // 预处理url
    std::string url;
    DecodeError error = preprocess_url(data, url);
    if(error != DecodeError::NO_ERROR)
        return error;
    
    int query_index = data.find_first_of('?', cur_index_);
    // 若不存在?，则更新url并更新cur_index_
    if(query_index == std::string::npos)
    {
        request_->set_url(url);
        cur_index_ = blank_index + 1;
        return DecodeError::NO_ERROR;
    }

    // 若存在query部分，则更新url部分并进行query部分的解析
    std::string query = url.substr(query_index + 1, blank_index - cur_index_ - 1);
    url = url.substr(cur_index_, query_index - cur_index_);
    request_->set_url(url);
    error = decode_query(query);
    if(error != DecodeError::NO_ERROR)
        return error;

    // 更新cur_index_
    cur_index_ = blank_index + 1;
    return DecodeError::NO_ERROR;
}

// request-target
// origin-form   = absolute-path [ "?" query ]
// absolute-path = 1*( "/" segment )
// segment       = *pchar
// pchar         = unreserved / pct-encoded / sub-delims / ":" / "@"
// unreserved    = ALPHA / DIGIT / "-" / "." / "_" / "~"
// pct-encoded   = "%" HEXDIG HEXDIG
// sub-delims  = "!" / "$" / "&" / "'" / "(" / ")"
//             / "*" / "+" / "," / ";" / "="
DecodeError HTTPDecoder::preprocess_url(const std::string &data, std::string& url)
{
    static std::unordered_set<char> valids = 
    {
        ':', '@', '-', '.', '_', '~',
        '%', '!', '$', '&', ';', '\'',
        '(', ')', '*', '+', ',', '=',
        '/'
    };

    // 不以/开头，则错误
    if(data[cur_index_] != '/')
        return DecodeError::BAD_URL;

    // 寻找空格
    int blank_index = data.find(' ', cur_index_);
    if(blank_index == std::string::npos || blank_index >= line_end_)
        return DecodeError::BAD_URL;

    DecodeError error = DecodeError::NO_ERROR;
    for (int i = cur_index_; i < blank_index; i++)
    {
        // 无效字符，则错误
        if(isalpha(data[i]) == false && isdigit(data[i]) == false &&
           valids.find(data[i]) == valids.end())
        {
            return DecodeError::BAD_URL;
        }

        switch(data[i])
        {
            case '+': url.push_back(' '); break;
            case '%': error = decode_hex(i, data, url); break;
            default:url.push_back(data[i]);break;
        }
        if(error != DecodeError::NO_ERROR)
            return error;
    }

    return DecodeError::NO_ERROR;
}

DecodeError HTTPDecoder::decode_hex(int& start, const std::string &data, std::string& url)
{
    if(start + 2 >= line_end_)
        return DecodeError::BAD_URL;

    char target = 0;
    for (int i = start; i < start + 2; i++)
    {
        if(isalpha(data[i]) == true)
        {
            char ch = std::toupper(data[i]);
            if(ch > 'F')
                return DecodeError::BAD_URL;
            target = target * 16 + (ch - 'A');
        }
        else if(isdigit(data[i]) == true)
        {
            target = target * 16 + data[i] - '0';
        }
        else
            return DecodeError::BAD_URL;
    }

    start += 2;
    url.push_back(target);
    return DecodeError::NO_ERROR;
}

DecodeError HTTPDecoder::decode_query(const std::string &query_data)
{   
    // 逐位检查添加
    std::string cur, key;
    HTTPRequest::QueryMap &queries = request_->queries_ref();
    DecodeError error = DecodeError::NO_ERROR;
    for (int i = 0; i < query_data.size(); i++)
    {
        switch(query_data[i])
        {
            case '=': 
            {
                key = cur;
                cur.clear();
            }; break;

            case '&':
            {
                queries[key] = cur;
                cur.clear();
            };break;

            default: cur.push_back(query_data[i]); break;
        }
    }

    return DecodeError::NO_ERROR;
}

DecodeError HTTPDecoder::decode_version(const std::string &data)
{
    // 获取HTTP协议版本并检验是否有效
    std::string version = data.substr(cur_index_, line_end_ - cur_index_ - 1);
    if(HTTPRequest::version_valid(version) == false)
        return DecodeError::BAD_VERSION;
    
    // 更新协议版本并更新cur_index_
    request_->set_version(version);
    cur_index_ = line_end_ + 1;
    return DecodeError::NO_ERROR;
}

DecodeError HTTPDecoder::decode_headers(const std::string &data)
{
    // 获取一行
    DecodeError error = get_one_line(data);
    if(error != DecodeError::NO_ERROR)
        return error;

    // 若空行，则更新状态
    if(data[cur_index_] == '\r')
    {
        // 若为POST请求则需要读取content
        if(request_->get_method() == HTTPRequest::Method::POST)
            state_ = DecodeState::CONTENT;
        // 否则解析完成
        else
            state_ = DecodeState::FINISH;

        cur_index_ += 2;
        return DecodeError::NO_ERROR;
    }

    // 寻找:
    int colon_index = data.find_first_of(':', cur_index_);
    if(colon_index == std::string::npos)
        return DecodeError::BAD_HEADER;

    // 更新头部字段并更新cur_index_
    std::string key = data.substr(cur_index_, colon_index - cur_index_);
    std::string val = data.substr(colon_index + 2, line_end_ - colon_index - 3);
    request_->add_header(key, val);
    cur_index_ = line_end_ + 1;
    return DecodeError::NO_ERROR;
}

DecodeError HTTPDecoder::decode_content(const std::string &data)
{
    // 获取content-length值
    std::string content_length_str = request_->get_header_val("Content-Length");
    if(content_length_str.empty() == true)
        return DecodeError::BAD_HEADER;
    int content_length = std::stoi(content_length_str);

    // 若剩余数据长度小于content-length，则报文不完整，等待后续报文
    if(content_length > data.size() - cur_index_)
        return DecodeError::UNCOMPLETE;
    
    // 更新content并更新cur_index_
    request_->set_content(data.substr(cur_index_, content_length));
    cur_index_ += content_length;

    // 更新状态
    state_ = DecodeState::FINISH;
    return DecodeError::NO_ERROR;
}

DecodeError HTTPDecoder::get_one_line(const std::string &data)
{
    // 寻找\r
    int r_index = data.find_first_of('\r', cur_index_);
    if(r_index == std::string::npos)
        return DecodeError::UNCOMPLETE;
    
    // 若\r后无数据，则报文接收不完整，需要等待完整报文
    if(r_index == data.size() - 1)
        return DecodeError::UNCOMPLETE;

    // 若\r后未接\n则格式错误
    if(data[r_index + 1] != '\n')
        return DecodeError::BAD_LINE;

    // 更新行首行尾标识
    line_start_ = cur_index_;
    line_end_ = r_index + 1;
    return DecodeError::NO_ERROR;
}