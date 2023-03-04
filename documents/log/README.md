# 日志

## 一、模型

![](http://43.138.43.178:8000/images/log_system_model.png)



## 二、Logger

日志系统与外界的交界层，该层隐藏了日志系统底层的**数据存储方式和写出方式**，利用`#define`宏定义了一系列统一的方法以供上层使用

- 对上层方法

  - 方法定义

      ```c++
      // 定义宏，便于使用
      #define LOG(level)  if (level <= Logger::get_log_level()) Logger(level, __FILE__, __func__, __LINE__).stream()
      #define LOG_TRACE LOG(Logger::LogLevel::TRACE)
      #define LOG_DEBUG LOG(Logger::LogLevel::DEBUG)
      #define LOG_INFO LOG(Logger::LogLevel::INFO)
      #define LOG_WARN LOG(Logger::LogLevel::WARN)
      #define LOG_ERROR LOG(Logger::LogLevel::ERROR)
      #define LOG_FATAL LOG(Logger::LogLevel::FATAL)
      ```
      
  - 特点

      - 创建匿名对象，顺序构造析构
      - 根据日志输出等级对输出的日志进行过滤
      - 流式输入，方便使用

- 下层的隐藏

  - 数据存储
    - 写入`Logger`的数据均通过`Logstream`提供的读写方法保存到其缓冲区中
  - 数据输出
    - `Logger`利用回调函数实现自定义输出方法的可能性。在日志系统中，默认采用输出到`stdout`的输出回调函数，但可以通过设置配置文件启用异步日志，此时，输出回调函数就会被设置为异步日志的输入函数，由此将`Logger`中的数据提交给异步日志完成输出。



## 三、数据存储

`Logger`的数据存储完全依靠`LogStream`类来实现。同时`LogStream`也提供了流式输入的机制

- 缓冲区
  `LogStream`使用封装了字符数组`char []`的`FixedBuffer`作为其缓冲区，任何对`LogStream`的写入，均被转入到`FixedBuffer`的字符数组中。
- 流式输入实现
  LogStream重载了一系列的`operator<<`方法，由此实现对`LogStream`的流式输入。
  在`Logger`对上层的方法定义中可以看到，这些方法最终都调用了`Logger::stream()`函数，这实际上就是将`Logger`中的`LogStream`成员返回给上层，然后由`LogStream`完成流式输入



## 四、数据写出

`Logger`的数据写出有同步和异步两种方式，并且在同步状况下输出的位置（文件、stdout等）是可以自定义的，这是因为`Logger`在输出数据时会调用其**输出回调函数成员**`output_func_`，因此数据的输出实际上是由定义的回调函数来完成的。

- 写出格式
  日志系统采用统一的输出格式
  `<time> <thread_id> <body> <file> <function> <line>`
  格式默认是每条日志一行，但是\<body>中允许存在换行。在输出多行日志时，上层可以在\<body>数据的前后添加换行，使最终的日志形式更加美观

  ```
  <time> <thread_id>
  <body>
  <file> <function> <line>
  ```

- 写出时机
  `Logger`写出一条完整的日志依靠于其构造和析构函数。构造函数期间，`Logger`会写入`<time> <thread_id> <body>`，而在析构时则会写入`<file> <function> <line>`并调用输出函数完成输出

  - 构造函数

    ```
    Logger(...)
    {
    	stream_ << <time> << " " << <thread_id> << " " << <body>;
    }
    ```

  - 析构函数

    ```
    ~Logger()
    {
    	stream_ << <file> << " " << <function> << " " << <line>;
    	output();
    }
    ```



## 五、工作流程

- 时序图
  ![](http://43.138.43.178:8000/images/log-工作流程.png)
- 解析
  1. 上层调用`LOG_DEBUG`等宏定义方法，实际上获得到了`Logger`对象的`LogStream`成员，使用`<<`将数据写入`LogStream`中
  2. `LogStream`调用底层`FixedBuffer`的`write()`方法将数据写入缓冲区中
  3. `Logger`析构，回调输出函数，此时根据回调函数的不同，有回调异步日志前端接口`AsyncLog::write()`方法、默认输出函数`Logger::default_ouput()`两种



## 六、异步日志

异步日志系统需要完成的主要目标是高效和线程安全。本日志系统参考`muduo`的前后端分离、双缓冲技术满足高效、安全的要求。

- 前后端分离
  - 模型
    ![](http://43.138.43.178:8000/images/front_back_model.png)
  - 前端
    前端为`AsyncLog::write()`函数，该函数将各线程的日志存储到前端缓冲区中，并在数据量足够时唤醒后端线程拿取数据
  - 后端
    定期或被前端唤醒后，拿取前端的数据，并将这些数据写入到文件中
- 双缓冲区
  - 模型
    ![](http://43.138.43.178:8000/images/double_buffer_model.png)
  - 工作原理
    - 双缓冲区：前后端各自维护一个缓冲区
    - 前端缓冲区空间不足时，交换前后端两个缓冲区，为前端提供足量的写入空间的同时也允许后端线程进行写出的工作，效率很高
  - 实现方法
    参考muduo的实现方法
    - 前后端缓冲区为`Buffer`数组，多个大尺寸（4MB）`Buffer` 构成它们各自的缓冲区
    - 前端缓冲区再次采用双缓冲机制。设置`current_`、`next_`两个`Buffer`，当`current_`空间不足时交换`current_`与`next_`，进一步减少了前端写入的阻塞时间，提高了写入速度
  - 优势
    - 相对于单缓冲区同时刻只允许写入、写出其中一种操作，双缓冲区允许前端写入的同时后端写出，显然双缓冲区相较于单缓冲区有着更大的吞吐量
    - 双缓冲区机制只有在写入和交换前后端缓冲区时需要加锁，相较于单缓冲区缩小了临界区的范围，提高了效率