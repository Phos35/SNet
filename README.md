# SNet

SNet，即 Simple Net，一个基于事件驱动实现的简易网络库



# 一、特性

- 使用C++11特性编写，基于事件循环、遵循One Loop Per Thread思想，简易轻便
- 利用主从Reactor模型和IO多路复用机制实现连接高并发
- 模块化设计，能够在本网络库的基础上扩展应用层协议框架，如HTTP协议的框架
- 双缓冲区机制的高效异步日志系统，及时记录服务器运行状态，便于追踪和检查错误
- 利用timerfd机制实现定期关闭不活跃连接的功能，避免无效连接占用资源
- 利用智能指针等RAII机制管理动态资源，有效减少内存泄漏的可能





# 二、整体架构

![](http://43.138.43.178:8000/images/SNet_Basic_Model.drawio.svg)

- 本项目主体由网络库`SNet`和日志系统`Logger`组成
- [`SNet`](https://github.com/Phos35/SNet/tree/master/documents/SNet/README.md)，是项目的核心模块，该模块使用主从Reactor模型、事件循环机制、IO多路复用机制等实现了TCP连接的高并发处理，以此为上层提供了高性能网络交互接口
- [`Logger`](https://github.com/Phos35/SNet/tree/master/documents/log/README.md)是一个高效的异步日志模块，该日志模块基于双缓冲区、前后端机制实现，能够在最小程度影响网络库性能的同时完成大量日志的写出
- 基于`SNet`提供的接口而延伸出的`Application Layer Protocol Framework`、`Application Layer Server`均为基于`SNet`的扩展特性诞生的产物，在本项目中，会提供一个简单的`HTTPServer`（[`http_demo`](https://github.com/Phos35/SNet/tree/master/documents/http_demo/README.md)）的实现，来说明`SNet`为什么具备扩展性、如何使用这种扩展性去开发其它应用层协议框架



# 三、并发模型

本项目使用的是目前主流网络库框架均在使用的主从`Ractor`多线程模型，其模型的基本框架如下：
![](http://43.138.43.178:8000/images/Master_Sub_Reactor.drawio.svg)
主从`Reactor`多线程模型中`Master Reactor`专注于接收新到来的连接、`Sub Reactor`完成对于`socket`的数据读写。而对于请求的处理和响应的生成则全部交由`worker`来完成，这种模型充分利用了服务器的CPU和IO，能够最大程度的使用服务器性能去服务尽可能多的客户端。
<a href="https://github.com/Phos35/SNet/tree/master/documents/SNet/README.md#5. 主从`Reactor`模型——多线程高并发">SNet中的主从Reactor模型</a>



# 四、Demo

[demo](https://img1.imgtp.com/2023/02/23/ifGDZUY4.gif)



# 五、压力测试

- 环境
  - CPU 
    - 型号：Intel(R) Xeon(R) Platinum 8255C CPU @ 2.50GHz
    - 核数：4核
  - 内存：8GB
  - 带宽：10Mbps
  - 编译选项：`O3`
- Apache Bench压测
  - Nginx
    ![](http://43.138.43.178:8000/images/nginx_pressure.jpg)
  - SNet
    ![](http://43.138.43.178:8000/images/snet_pressure.jpg)
  - 可以看到基于`SNet`的`HTTPServer`与`Nginx`的`QPS`相对要高一些，这说明`SNet`有处理高并发连接的能力





# 六、使用方法

- `my_http_demo`

  ```
  cd build
  cmake ..
  cmake --build .
  https://github.com/Phos35/SNet/tree/master/web_server
  ```

  `my_http_demo`是基于`SNet`实现的一个简易HTTP服务器，它能够响应`resources`文件夹下的文档、图片等资源。

- `SNet`
  `SNet`是对`TCP`的封装，其目的在于为上层 -- 应用层提供便捷的`TCP`数据处理接口。`SNet`为上层协议应用开发提供了设置底层数据处理器 -- `MessageProcessor`的接口，应用层协议应用开发只需要根据协议设计一个报文解析器 -- `Decoder`、消息/请求分发器`Dispatcher`，并将这两个关键部分封装入`MessageProcessor`，然后创建一个生产`MessageProcessor`的具体工厂`MessageProcessorFactory`，利用`TCPServer`的接口`create_factory`即可将应用层的报文解析、消息分发交给`SNet`来完成。在[http_demo](https://github.com/Phos35/SNet/tree/master/documents/http_demo/README.md)中可以看到更为详细的使用方法



# 七、待改进

- [ ] `HTTP`中的`Response`设计不完善，糅杂了`MIME`等内容，需要深入`HTTP`协议完善
- [ ] `HTTPDecoder`的解析流程较为混乱，需要参考优秀解析方法（如nginx的报文解析）加以实现
- [ ] 目前关于服务器性能测试指标了解不足，只是用了`Apache Bench`进行了简单测试，需要深入测试相关的知识加以补全
- [ ] 文档不够详细，需要补全



# 八、后续计划

- [ ] `SNet` 目前只封装了`TCP`，且只有`TCPServer`而没有`TCPClient`，因此`SNet`为网络库而言并不完善。后续会逐步添加`TCPClient`、`UDP`，最后若有可能，会进一步封装`QUIC`，提高完整性
- [ ] 目前基于`SNet`实现的`demo`只有`HTTP`协议而没有其它基于`TCP`的应用层协议，因此很难说明`SNet`的拓展性是确实可行的。后续需要学习其它应用层协议并利用`SNet`加以实现
- [ ] 目前基于`SNet`实现的`HTTPDeom`过于简陋，不能进行完善的性能测试以验证`SNet`的速度足够快。后续会对`HTTPDemo`加以完善，在体现出更好的界面效果的同时加强性能测试，最终用于完整的性能测试