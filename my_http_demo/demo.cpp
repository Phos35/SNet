#include "demo.h"

int main()
{
    HTTPDemo demo;
    demo.run();
    return 0;
}

void HTTPDemo::run()
{
    // 读取参数文件
    Config &config = Config::get_instance(CONFIG_FILE);

    // 开启异步日志
    Logger::enable_async();

    // 注册处理函数
    HTTPDispatcher::register_handler("/", std::bind(&HTTPDemo::handle_slash, this, std::placeholders::_1));

    // 启动服务器
    HTTPServer server("0.0.0.0", 8000);
    server.run();
}

HTTPResponse HTTPDemo::handle_slash(HTTPRequest *request)
{
    if(request->get_method() == HTTPRequest::Method::GET)
        return handle_get(request);
    else
        return handle_post(request);
}

std::string HTTPDemo::parse_path(const std::string &url)
{
    // 请求index.html
    if(url == "/")
        return ROOT + std::string("/html/index.html");
    
    // 请求其他类型的资源
    return ROOT + url;
}

std::string HTTPDemo::get_resource(const std::string &path)
{
    std::string content;
    std::ifstream target(path);
    if(target.is_open() == false)
    {
        printf("File %s open failed! %s\n", path.c_str(), strerror(errno));
    }
    else
    {
        // 获取文件内容
        std::istreambuf_iterator<char> begin(target);
        std::istreambuf_iterator<char> end;
        content = std::string(begin, end);
    }

    // 关闭文件并返回指定内容
    return content;
}

HTTPResponse HTTPDemo::handle_not_found()
{
    return HTTPResponse("HTTP/1.1", 404, "Not Found");
}

HTTPResponse HTTPDemo::handle_get(HTTPRequest* request)
{
    // 获取请求资源的路径
    std::string resourcePath = parse_path(request->get_url());
    
    // 获取资源
    std::string content = get_resource(resourcePath);

    // 构造响应报文返回
    HTTPResponse response;
    response.add_content(std::move(content));
    response.set_content_type(HTTPResponse::judge_mime_type(resourcePath));
    return response;
}

std::unordered_map<std::string, std::string> HTTPDemo::parse_args(std::string &args)
{
    std::string normal = cancelHTTPFormat(args);
    std::unordered_map<std::string, std::string> m;
    std::string cur, key;
    for (int i = 0; i < normal.size(); i++)
    {
        char ch = normal[i];
        if (ch == '=')
            key.swap(cur);
        else if(ch == '&')
        {
            m[key] = cur;
            cur.clear();
            key.clear();
        }
        else
            cur.push_back(ch);
    }
    m[key] = cur;

    return m;
}

char HTTPDemo::hexASCIItoChar(const std::string& hexStr)
{
    char ret = 0;
    for (int i = 0; i < hexStr.size(); i++)
    {
        if(isdigit(hexStr[i]))
            ret = ret * 16 + hexStr[i] - '0';
        else
            ret = ret * 16 + hexStr[i] - 'A' + 10;
    }
    return ret;
}

std::string HTTPDemo::cancelHTTPFormat(std::string& ori)
{
    std::string ret;
    for (int i = 0; i < ori.size(); i++)
    {
        switch(ori[i])
        {
        case '+' : ret.push_back(' '); break;
        case '%' : 
            {
                ret.push_back(hexASCIItoChar(ori.substr(i + 1, 2)));
                i += 2;
            };
            break;
        default: ret.push_back(ori[i]); break;
        }
    }

    return ret;
}

HTTPResponse HTTPDemo::handle_post(HTTPRequest *request)
{
    // 拆分获取参数
    std::unordered_map<std::string, std::string> arg_map = parse_args(request->content_ref());

    // 参数错误
    auto type_itr = arg_map.find("type");
    if (type_itr == arg_map.end())
        return HTTPResponse("HTTP/1.1", 403, "Forbidden");

    // 解析指令
    if(type_itr->second == "cmd")
    {
        // 获取指令具体内容
        int blank_index = arg_map["content"].find(' ');
        std::string cmd = arg_map["content"].substr(0, blank_index);
        std::string file_name = arg_map["content"].substr(blank_index + 1);
        if (cmd == "markdown")
        {
            arg_map["type"] = "md";
            arg_map["url"] = "/markdown/" + file_name;
        }
        else if(cmd == "image")
        {
            arg_map["type"] = "image";
            arg_map["url"] = "/image/" + file_name;
        }
    }

    std::string content;
    if (type_itr->second == "md" || type_itr->second == "image")
    {
        std::string resourcePath = parse_path(arg_map["url"]);

        // 获取文件内容
        content = get_resource(resourcePath);
    }
    // 获取inttrioduction内容
    else if(type_itr->second == "introduction")
        content += get_resource(ROOT + std::string("/txt/introduction.txt"));

    // 若文件内容为空，则未找到对应的文件，返回404
    if(content.empty() == true)        
        return handle_not_found();
    // 否则正常响应
    else
    {
        HTTPResponse response;
        response.set_content_type("text/plain");
        response.add_content(content);
        return response;
    }
}