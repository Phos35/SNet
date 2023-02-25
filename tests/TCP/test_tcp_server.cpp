#include "tcp_server.h"
#include "config.h"
#include "logger.h"
#include <fstream>

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
            std::string val = data.substr(colon_index + 1, data.size() - colon_index - 3);
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

void echo_time(const TCPConnPtr& conn, TestMessage* msg)
{
    conn->write(Timestamp::now().format("%Y-%m-%d %H-%M-%S.%s") + "\n");
}

void echo_file(const TCPConnPtr& conn, TestMessage* msg)
{
    std::string file_name = msg->val();
    std::ifstream file(file_name);
    if (file.is_open() == false)
    {
        conn->write("No such file: " + (msg->val()));
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
    Config &config = config.get_instance("/home/ubuntu/projects/web_server/SNet/config/config.json");

    Dispatcher::UPtr dispatcher = std::make_unique<TestDispatcher>();
    Decoder::UPtr decoder = std::make_unique<TestDecoder>();

    TestDispatcher::register_processor("time", echo_time);
    TestDispatcher::register_processor("file", echo_file);
    TestDispatcher::register_processor("close", echo_close);

    TCPServer server("127.0.0.1", 9112, std::move(decoder), std::move(dispatcher));
    server.run();
    return 0;
}