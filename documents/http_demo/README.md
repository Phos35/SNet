# 目的

该`Demo`为展示`SNet`扩展性的一个样例，下文将会讲述如何使用`SNet`去实现一个HTTP框架



# 一、创建HTTP报文

上层报文只要继承`Message`即可，其余部分可以根据`UpperDecoder`和`UpperDispatcher`的要求随意定义

- Message基类

  ```c++
  class Message : public std::enable_shared_from_this<Message>
  {
  public:
      typedef Message *Ptr;
      typedef std::unique_ptr<Message> UPtr;
      typedef std::shared_ptr<Message> SPtr;
  
      // 报文解析的最终结果
      enum class DeocdeResult
      {
          SUCCESS,    // 解析成功
          FAILURE     // 解析失败
      };
  
      Message();
      Message(const std::string &data);
      Message(DeocdeResult result);
      Message &operator=(Message &&another);
      virtual ~Message();
  
      /// @brief 设置解析结果
      void set_result(DeocdeResult result);
  
      /// @brief 获取解析结果
      DeocdeResult get_result();
  
      /// @brief 获取报文字符数据
      /// @return 原始数据
      virtual std::string data();
  
      /// @brief 获取原始数据的大小
      /// @return 原始数据大小
      size_t data_size();
  
      // TODO 或许可以使用移动语义做优化
      /// @brief 用于派生类设置数据
      /// @param data 待设置的数据
      void set_data(const std::string &data);
  
      /// @brief 清空数据
      void clear_data();
  
  private:
      DeocdeResult    result_;        // 报文解析结果
      std::string     data_;          // 字符串数据
  };
  ```

  1. 上层报文结构继承`Message`要注意必要时重写`data()`方法，如`HTTPResponse`就需要重写data()才能将其用于输出报文字符串
  2. 上层`Decoder`解析报文不完整时注意置`result_`为`DecodeResult::FAILURE`，这样才能等待TCP连接传输后续数据

- `HTTPRequest`

  ```c++
  class HTTPRequest : public Message
  {
  public:
      // 请求方法
      enum Method
      {
          UNKNOWN,    // 未知方法
          GET,        // 请求指定的资源，并返回资源实体
          HEAD,       // 请求指定资源的信息，即GET请求中的headers部分
          POST,       // 客户端提交数据请求处理，可能会获取或改动服务端资源
          PUT,        // 用客户端传输的数据替代指定的资源
          DELETE,     // 请求删除指定资源
          CONNECT,    // TODO 待理解
          OPTIONS,    // TODO 待理解
          TRACE,      // echo，用于测试
          PATCH,      // PUT的补充，用于对已有资源进行局部更新
      };
  
      /// 报文的解析错误信息
      enum class Error
      {
          NO_ERROR,       // 报文解析正常，无错误
  
          UNKNOWN_ERROR,  // 未知错误
  
          BAD_LINE,       // 一行未以rn换行符结尾
          UNCOMPLETE,     // 报文不全
  
          BAD_METHOD,     // 错误的请求方法，如不存在或者拼写错误
          BAD_URL,        // URL语法错误
          BAD_VERSION,    // 错误的HTTP版本，如1.2等
  
          BAD_HEADER,     // header语法错误，如:右侧没有空格
      };
  
      HTTPRequest(const std::string& data);
  
      /// @brief 清空request，便于复用
      void reset();
  
      /// @brief 获取错误码
      /// @return 错误码
      Error error();
  
      /// @brief 设置错误码
      /// @param error 待设置的错误码
      void set_error(Error error);
  
      /// @brief 获取请求方法
      /// @return 请求方法
      Method get_method();
  
      /// @brief 设置请求方法
      /// @param method 待设置的请求方法
      void set_method(Method method);
  
      /// @brief 获取请求方法对应的字符串
      /// @return 请求方法对应的字符串
      std::string get_method_str();
  
      /// @brief 设置请求方法
      /// @param method 请求方法对应的字符串
      void set_method(std::string method);
  
      /// @brief 获取url
      /// @return url
      URL get_url();
  
      /// @brief 设置url
      /// @param url 待设置的url
      void set_url(const std::string &url);
  
      /// @brief 获取协议版本
      /// @return 协议版本
      Version get_version();
  
      /// @brief 设置协议版本
      /// @param version 
      void set_version(Version version);
  
      /// @brief 获取字段值
      /// @param key 字段值对应的键
      /// @return 字符串类型的字段值
      std::string get_header_val(const std::string &key);
  
      /// @brief 添加头部字段
      /// @param key 头部字段的键
      /// @param val 头部字段的值
      void add_header(const std::string &key, const std::string &val);
  
      /// @brief 获取content的引用
      /// @return content的引用
      Content &content_ref();
      
      /// @brief 设置content
      /// @param content 待设置的content
      void set_content(const std::string& content);
  
      /// @brief 获取queries的引用
      /// @return queries引用
      QueryMap &queries_ref();
  
      /// @brief 报文相关信息字符串
      std::string to_string();
  
      /// @brief 将请求方法转换为对应的字符串
      /// @param method 请求方法
      /// @return 请求方法对应的字符串
      static std::string method_to_string(Method method);
  
      /// @brief 将请求方法字符串转化为对应的请求方法
      /// @param method_str 请求方法字符串
      /// @return 字符串对应的请求方法
      static Method string_to_method(const std::string &method_str);
  
      /// @brief 将错误码转换为对应的字符串
      /// @param error 待转换的字符串
      /// @return 错误码对应的字符串
      static std::string error_to_string(Error error);
  
      /// @brief 检验请求方法是否有效
      /// @param method 待检验的请求方法
      /// @return 有效返回true，无效返回false
      static bool method_valid(const std::string& method);
  
      /// @brief 检验版本是否有效
      /// @param version 待检验的版本
      /// @return 有效则返回true，无效则返回false
      static bool version_valid(const std::string &version);
  
  private:
      Method      method_;    // 请求方法
      URL         url_;       // 请求url
      Version     version_;   // HTTP版本
  
      HeaderMap   headers_;   // 头部字段
      Content     content_;   // 内容
  
      QueryMap    queries_;   // query参数
  
      Error       error_;     // 错误信息
  };
  ```

- `HTTPResponse`

  ```c++
  class HTTPResponse : public Message
  {
  public:
      typedef std::unordered_map<std::string, std::string> HeaderMap;
  
      /// MIME类型
      enum class MIME
      {
          TEXT,   // 纯文本类型
          HTML,   // html文件
          IMAGE,  // 图片
          PNG,    // png图片
          GIF,    // gif图片
          JPG,    // jpg图片
          JS,     // js文件
      };
  
      // HTTP响应
      HTTPResponse(const std::string &version = "HTTP/1.1", size_t code = 200,
                   const std::string &description = "OK");
      
      // 移动构造
      HTTPResponse(HTTPResponse &&another);
  
      // 移动复制
      HTTPResponse &operator=(HTTPResponse &&another);
  
      /// @brief 清空响应内容
      void reset();
  
      /// @brief 获取HTTP协议版本
      /// @return HTTP协议版本
      std::string version();
  
      /// @brief 设置HTTP协议版本
      /// @param version HTTP协议版本
      void set_version(const std::string &version);
  
      /// @brief 获取状态码
      /// @return 状态码
      size_t code();
  
      /// @brief 设置状态码
      /// @param code 待设置的值
      void set_code(size_t code);
  
      /// @brief 获取状态码描述
      /// @return 状态码描述
      std::string description();
  
      /// @brief 设置状态码描述
      /// @param val 新的描述字符串
      void set_description(const std::string &val);
  
      /// @brief 获取头部字段值
      /// @param key 头部字段的键
      /// @return 键对应的字符串值
      std::string get_header_val(const std::string &key);
  
      /// @brief 设置头部字段，若头部字段不存在则添加
      /// @param key 键
      /// @param val 值
      void set_header(const std::string &key, const std::string &val);
  
      /// @brief 设置Content-Type字段
      /// @param type 待设置的值
      void set_content_type(MIME type);
  
      /// @brief 设置Content-Type字段
      /// @param type 待设置的值
      void set_content_type(const std::string &type);
  
      /// @brief 根据url中请求的文件名判断文件类型
      /// @param url 待检查的url
      /// @return 文件类型字符串
      static std::string judge_mime_type(const std::string &url);
  
      /// @brief 添加响应体 -- 自动添加Content-Length字段
      /// @param content 待添加的内容
      void add_content(const std::string &content);
  
      /// @brief 添加响应体 -- 移动语义；自动添加Content-Length
      /// @param content 待添加的内容
      void add_content(std::string &&content);
  
      /// @brief 获取完整的响应报文
      /// @return 响应报文
      std::string data() override;
  
      /// @brief MIME转换为对应的字符串
      /// @param type 待转换的类型
      /// @return MIME type对应的字符串
      static std::string mime_to_string(MIME type);
  
  private:
      static std::unordered_map<std::string, std::string> mime_map;
  
      std::string version_;           // HTTP协议版本
      size_t          code_;          // 状态码
      std::string     description_;   // 状态码描述
  
      HeaderMap       headers_;       // 头部字段
      std::string     headers_str_;   // 存储头部字段的字符串格式
  
      std::string     content_;       // 响应体
  
  };
  ```

  

# 二、创建`HTTPDecoder`

- `Decoder`基类

  ```c++
  class Decoder
  {
  public:
      typedef std::unique_ptr<Decoder> UPtr;
  
      Decoder();
      virtual ~Decoder();
  
      /// @brief 解析数据
      /// @param data 待解析的数据
      /// @return 解析完成的报文的指针
      virtual Message* decode(const std::string& data);
  
  private:
  };
  ```

  注意上层的报文解析器`UpperDecoder`需要继承`Decoder`后根据协议的报文解析要求重写`decode`方法

- `HTTPDecoder`派生

  ```c++
  class HTTPDecoder : public Decoder
  {
  public:
      HTTPDecoder();
      ~HTTPDecoder();
  
      // 报文解析状态
      enum class DecodeState
      {
          REQUEST_LINE,   // 解析请求行
          HEADERS,        // 解析头部字段
          CONTENT,        // 解析内容
          FINISH,         // 解析完成
      };
  
      /// @brief 解析HTTP报文
      /// @param data 待解析的数据
      /// @return 解析完成的报文
      Message* decode(const std::string &data) override;
  private:
      size_t              cur_index_;     // 报文解析中的当前指针
      size_t              line_start_;    // 当前行的起始位置
      size_t              line_end_;      // 当前行的结束位置
  
      DecodeState         state_;         // 解析状态
      HTTPRequest*        request_;       // 请求报文指针，用于解析过程中临时存储
  
      /// @brief 解析请求行
      /// @param data 待解析的数据
      /// @return 解析错误码
      DecodeError decode_request_line(const std::string &data);
  
      /// @brief 解析请求方法
      /// @param data 待解析的数据
      /// @return 解析错误码
      DecodeError decode_method(const std::string &data);
  
      /// @brief 解析url
      /// @param data 待解析的数据
      /// @return 解析错误码
      DecodeError decode_url(const std::string &data);
  
      /// @brief 预处理url，+、16进制值等转化实际字符
      /// @param data 待处理的数据
      /// @param url 存储处理完成的url
      /// @return 解析错误码
      DecodeError preprocess_url(const std::string &data, std::string& url);
  
      /// @brief 解析url中的16进制字符
      /// @param start %所在位置的索引
      /// @param data 待解析的数据
      /// @param url 已完成解析的url
      /// @return 错误码
      DecodeError decode_hex(int& start, const std::string &data, std::string& url);
  
      /// @brief 解析resource-target中的query部分
      /// @param query_data 待解析的query部分数据
      /// @return 解析错误码
      DecodeError decode_query(const std::string &query_data);
  
      /// @brief 解析HTTP协议版本
      /// @param data 待解析的数据
      /// @return 解析错误码
      DecodeError decode_version(const std::string &data);
  
      /// @brief 解析头部字段
      /// @param data 待解析的数据
      /// @return 解析错误码
      DecodeError decode_headers(const std::string &data);
  
      /// @brief 解析报文内容部分
      /// @param data 待解析的数据
      /// @return 解析错误码
      DecodeError decode_content(const std::string &data);
  
      /// @brief 读取一行的
      /// @param data 
      /// @return 
      DecodeError get_one_line(const std::string &data);
  };
  
  ```

  核心点在于**重写decode方法并返回客户端数据解析完成的报文的指针**。此处Decoder要求持有一个`Message*/Message`作为`Request`，在本例中就是`HTTPRequest`。一方面是为了满足`decode`方法返回`Message` 的指针，另一方面则是为了复用(详见[SNet中的复用](.https://github.com/Phos35/SNet/tree/master/documents/SNet_reuse) TODO)。至于报文解析的具体细节则由框架开发者根据协议去随意实现即可



# 二、创建`HTTPDispatcher`

- `Dispatcher`基类

  ```c++
  class Dispatcher
  {
  public:
      typedef std::unique_ptr<Dispatcher> UPtr;
  
      Dispatcher();
      virtual ~Dispatcher();
  
      /// @brief 将消息分配到对应的处理函数
      /// @param msg 待处理的消息
      /// @return 响应数据
      virtual Message* dispatch(Message* msg);
  
  private:
  };
  ```

  注意上层`UpperDispatcher`需要继承`Dispatcher`后重写`dispatch`方法

- `HTTPDispatcher`派生

  ```c++
  class HTTPDispatcher : public Dispatcher
  {
  public:
      typedef std::function<HTTPResponse(HTTPRequest*)> Handler;
      typedef std::unordered_map<std::string, Handler> HandlerTable;
  
      HTTPDispatcher();
      virtual ~HTTPDispatcher();
  
      /// @brief 将消息分配到对应的处理函数
      /// @param conn 消息发生的连接
      /// @param msg 待处理的消息
      Message* dispatch(Message* msg) override;
  
      /// @brief 注册处理函数
      /// @param 注册函数需要处理的url
      /// @param  待注册的函数
      static void register_handler(const std::string& url, const Handler& handler);
  
      /// @brief HTTP消息处理函数，形式为<url, handler_function>
      static HandlerTable handler_table_;
  
  private:
      HTTPResponse *response_;
  };
  ```

  核心点在于**重写dispatch方法并返回对客户端的响应消息**，`UpperDispatcher`内部如何分发`Message`则交由应用层协议开发人员负责，比如此处`HTTPDispatcher`使用了一个静态的`unordered_map`来存储`url`和对应的处理函数，同时还定义了一个注册函数的接口`register`，这样上层`Server`开发人员就能够只写对于特定`url`如何处理的逻辑就足够了。



# 三、创建`HTTPMessageProcessor`

- `MessageProcessor`基类

  ```c++
  class MessageProcessor
  {
  public:
      typedef std::unique_ptr<MessageProcessor> UPtr;
  
      MessageProcessor();
      virtual ~MessageProcessor();
  
      /// @brief 创建Decoder和Dispatcher
      virtual void create();
  
      /// @brief 处理连接上的数据，解析为消息并分发做响应
      /// @param conn TCP连接
      void process_data(const TCPConnSPtr& conn);
  
  protected:
      /// @brief 设置Decoder，用于派生类设置相应的decoder派生类
      /// @param decoder Decoder指针
      void set_decoder(Decoder::UPtr&& decoder);
  
      /// @brief 设置Dispatcher，用于派生类设置相应的dispatcher派生类
      /// @param Dispatcher Dispatcher指针
      void set_dispatcher(Dispatcher::UPtr&& dispatcher);
  
      /// @brief 根据上层协议决定TCP连接的状态 -- 关闭或者保持
      /// @param TCP连接
      /// @param 客户端请求消息
      virtual void process_conn(const TCPConnSPtr& conn, Message* request);
  
      /// @brief 解析数据
      /// @param data 待解析的数据
      /// @return 解析完成的客户端消息
      Message* decode(const std::string &data);
  
      /// @brief 分发消息，获取响应
      /// @param msg 解析生成的消息
      Message* dispatch(Message *msg);
  
  private:
      Decoder::UPtr decoder_;
      Dispatcher::UPtr dispatcher_;
  };
  ```

- `HTTPMessageProcessor`派生

  ```c++
  class HTTPMessageProcessor : public MessageProcessor
  {
  public:
      typedef std::unique_ptr<HTTPMessageProcessor> UPtr;
  
      /// @brief 创建Decoder和Dispatcher
      void create()override;
  
  protected :
      /// @brief 单个响应发送完毕后对连接的处理 -- 关闭或保持
      /// @param conn TCP连接
      /// @param request 客户端请求
      void process_conn(const TCPConnSPtr &conn, Message *request) override;
  
  private:
  
  };
  
  ```

  注意`UpperMessageProcessor`继承`MessageProcessor`后需要重写`create`方法用于创建`UpperDecoder`和`UpperDispatcher`；需要重写`process_conn`方法用于在发送完数据后如何处理连接（保持连接或者断开），如`HTTPMessageProcessor`中对于持有`keep-alive`选项的连接进行保持，而对于不持有该选项的连接则直接进行关闭



# 四、创建`HTTPMessageProcessorFactory`

- `MessageProcessorFactory`

  ```c++
  class MessageProcessorFactory
  {
  public:
      typedef std::unique_ptr<MessageProcessorFactory> UPtr;
  
      /// @brief 创建消息处理器
      virtual MessageProcessor::UPtr create_message_processor();
      virtual ~MessageProcessorFactory();
  
  private:
  
  };
  ```

  注意`UpperMessageProcessorFactory`需要在继承后重写`create_message_processor`方法，用于创建`UpperMessageProcessor`

- `HTTPMessageProcessorFactory`

  ```c++
  class HTTPMessageProcessorFactory : public MessageProcessorFactory
  {
  public:
      /// @brief 创建HTTP消息处理器
      /// @return HTTP消息处理器的指针
      MessageProcessor::UPtr create_message_processor()override;
  private:
  };
  ```

  



# 五、创建`HTTPServer`

- 原因
  `SNet`与上层的直接接口就是`TCPServer`。上述定义了许多的结构，最终都是通过`TCPServer`的`create_factory`方法来讲`UpperMessageProcessorFactory`传递给`WorkerPool`，使得`WorkerPool`能够在创建`Worker`线程时为每个线程创建`UpperMessageProcessor`，从而让`SNet`能够处理不同协议的数据

- 做法——`HTTPServer`

  ```c++
  class HTTPServer : public TCPServer
  {
  public:
      /// @brief HTTP服务器
      /// @param addr 绑定的地址
      /// @param port 绑定的端口
      HTTPServer(const std::string &addr, uint16_t port);
  
  protected:
      /// @brief 创建消息处理器工厂
      void create_factory() override;
  
  private:
  
  };
  
  void HTTPServer::create_factory()
  {
      set_factory(std::make_unique<HTTPMessageProcessorFactory>());
  }
  ```