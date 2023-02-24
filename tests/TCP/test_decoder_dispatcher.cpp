#include "tcp_connection.h"
#include "logger.h"
#include "config.h"
#include "acceptor.h"
#include "event_loop_thread_pool.h"
#include "decoder.h"
#include "dispatcher.h"
#include <unordered_map>
#include <iostream>

using TCPConnPtr = TCPConnection::SPtr;
using SocketPtr = std::unique_ptr<Socket>;

std::unordered_map<size_t, TCPConnPtr> m;
EventLoopThreadPool pool(4);
bool end = false;

class TestMessage: public Message
{
public:
    typedef TestMessage *Ptr;
    typedef std::unique_ptr<TestMessage> UPtr;
    TestMessage(const std::string &data, const std::string &key, const std::string &val)
        : label_({key, val}), Message(data)
    {}
    TestMessage(Message::DeocdeResult result)
    :Message(result)
    {}

    std::string key() { return label_.first; }
    std::string val() { return label_.second; }

private:
    std::pair<std::string, std::string> label_;
};

class TestDecoder : public Decoder
{
public:
    TestDecoder(){}
    ~TestDecoder(){}

    TestMessage::Ptr decode(const std::string &data) override
    {
        int colon_index = data.find(':');
        if(colon_index == std::string::npos)
        {
            return new TestMessage(Message::DeocdeResult::FAILURE);
        }
        else
        {
            std::string key = data.substr(0, colon_index);
            std::string val = data.substr(colon_index);
            return new TestMessage(data, key, val);
        }
    }

private:
};

class TestDispatcher : public Dispatcher
{
public:
    typedef std::function<void(const TCPConnPtr &, const TestMessage::Ptr &)> Processor;
    static std::unordered_map<std::string, Processor> test_dispatch_table;
    
    /// @brief 注册特定数据的处理函数
    /// @param name 处理函数的名称
    /// @param processor 处理函数
    static void register_processor(const std::string &name, const Processor &processor)
    {
        test_dispatch_table[name] = processor;
    }

    void dispatch(const TCPConnPtr& conn, const Message::Ptr& msg) override
    {
        TestMessage::Ptr test_msg = dynamic_cast<TestMessage::Ptr>(msg);
        test_dispatch_table[test_msg->key()](conn, test_msg);
        delete test_msg;
        conn->buffer_ref().release(msg->raw_size());
    }

private:
};
std::unordered_map<std::string, TestDispatcher::Processor> TestDispatcher::test_dispatch_table;

void on_read(const TCPConnPtr& conn, const char* buf)
{
    // TestDecoder decoder;
    // TestDispatcher dispatcher;

    // Message::Ptr msg = decoder.decode(buf);
    // if(msg->get_result() == Message::DeocdeResult::FAILURE)
    // {
    //     printf("invalid data\n");
    // }
    // else
    // {
    //     dispatcher.dispatch(conn, msg);
    // }
}

void on_close(const TCPConnPtr& conn)
{
    // 获取对应的指针
    TCPConnPtr last_ptr = conn;

    // 从map中移除
    m.erase(conn->id());

    // 调用destroy, 在事件循环中回收资源
    EventLoop *loop = conn->event_loop();
    loop->run_in_loop(std::bind(&TCPConnection::destroy, last_ptr));
}

void on_arrive(SocketPtr&& socket)
{
    static size_t id = 0;

    // 从事件循环线程池中选中一个事件循环
    EventLoop *loop = pool.get_one();

    // 创建TCPConnection并加入到map中
    TCPConnPtr tcp_conn = std::make_unique<TCPConnection>(id, loop, std::move(socket),
                                                          std::make_unique<TestDecoder>(),
                                                          std::make_unique<TestDispatcher>());
    tcp_conn->set_msg_callback(on_read);
    tcp_conn->set_close_callback(on_close);
    m.insert({id, tcp_conn});

    // 调用创建函数，使其在事件循环中完成初始化
    loop->run_in_loop(std::bind(&TCPConnection::create, tcp_conn));
}

void echo_time(const TCPConnPtr& conn, TestMessage* msg)
{
    conn->write(Timestamp::now().format("%Y-%m-%d %H-%M-%S.%s") + "\n");
}

void echo_file(const TCPConnPtr& conn, TestMessage* msg)
{
    std::ifstream file(msg->val());
    if(file.is_open() == false)
    {
        conn->write("No such file\n");
        return;
    }
    else
    {
        char buf[1024] = {0};
        file.readsome(buf, 1024);
        conn->write(buf);
    }
}

void echo_close(const TCPConnPtr& conn, TestMessage* msg)
{
    conn->close();
}

int main()
{
    Config &config = Config::get_instance("/home/ubuntu/projects/web_server/SNet/config/config.json");

    /*模拟Server启动过程*/
    // 创建监听socket
    SocketPtr sock = std::make_unique<Socket>(AF_INET, SOCK_STREAM, 0);
    sock->set_reuse_addr();
    sock->bind("127.0.0.1", 9112);

    // 启动事件循环线程池
    pool.start();

    // 注册消息处理函数
    TestDispatcher::register_processor("time", echo_time);
    TestDispatcher::register_processor("file", echo_file);
    TestDispatcher::register_processor("close", echo_close);

    // 启动Acceptor
    Acceptor a(sock->fd());
    a.set_conn_callback(on_arrive);
    a.start();
    /*模拟Server启动过程*/

    std::string input;
    while (input.find("QUIT") == std::string::npos)
    {
        std::cin >> input;
    }

    // 测试析构函数能否正常结束各个线程

    return 0;
}