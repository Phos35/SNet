#include "message_processor.h"
#include "tcp_connection.h"
#include "logger.h"
#include "debug.h"

MessageProcessor::MessageProcessor()
:decoder_(nullptr), dispatcher_(nullptr)
{

}

MessageProcessor::~MessageProcessor()
{

}

void MessageProcessor::create()
{
    // MessageProcessor基类生成基类Decoder和基类Dispatcher
    set_decoder(std::make_unique<Decoder>());
    set_dispatcher(std::make_unique<Dispatcher>());
}

void MessageProcessor::process_data(const TCPConnSPtr& conn)
{
    // 1. 解析数据，生成客户端消息
    TCPBuffer &buffer = conn->recv_buffer_ref();
    std::string data = buffer.read(0);
    LOG_DEBUG << "TCPConnection " << conn->id() << " read data: " << SNet::DEBUG::one_line(data);
    Message *msg = decode(data);
    LOG_DEBUG << "TCPConnection " << conn->id() << " decoded, counter: " << conn.use_count();

    // 2.1 消息不完整，等待后续数据
    if(msg->get_result() == Message::DeocdeResult::FAILURE)
    {
        return;
    }

    // 2.2 消息解析完成，释放缓冲区，分发消息至对应的处理函数，获取响应
    buffer.release(msg->data_size());
    Message *response = dispatch(msg);
    LOG_DEBUG << "TCPConnection " << conn->id() << " dispatched, counter: " << conn.use_count();

    // 2.3 若响应为空，则关闭连接
    if(response == nullptr)
    {
        // 此处有问题，何时会发生response为nullptr？短报文Request应当不会被截断才对
        conn->close();
        return;
    }

    // 2.3 发送响应
    LOG_DEBUG << "TCPConnection " << conn->id() << " write data: " << SNet::DEBUG::one_line(response->data());
    conn->write(response->data());
    LOG_DEBUG << "TCPConnection " << conn->id() << " responded, counter: " << conn.use_count();

    // 2.4 处理连接状态
    process_conn(conn, msg);
}

Message* MessageProcessor::decode(const std::string &data)
{
    return decoder_->decode(data);
}

Message* MessageProcessor::dispatch(Message *msg)
{
    return dispatcher_->dispatch(msg);
}

void MessageProcessor::process_conn(const TCPConnSPtr& conn, Message* request)
{
    // 基类默认关闭连接
    conn->close();
}

void MessageProcessor::set_decoder(Decoder::UPtr&& decoder)
{
    decoder_ = std::move(decoder);
}

void MessageProcessor::set_dispatcher(Dispatcher::UPtr&& dispatcher)
{
    dispatcher_ = std::move(dispatcher);
}