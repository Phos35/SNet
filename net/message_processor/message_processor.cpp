#include "message_processor.h"
#include "tcp_connection.h"

MessageProcessor::MessageProcessor()
:decoder_(nullptr), dispatcher_(nullptr)
{

}

MessageProcessor::~MessageProcessor()
{
    if(decoder_ != nullptr)
        delete decoder_;
    
    if(dispatcher_ != nullptr)
        delete dispatcher_;
}

void MessageProcessor::create()
{
    // MessageProcessor基类生成基类Decoder和基类Dispatcher
    set_decoder(new Decoder());
    set_dispatcher(new Dispatcher);
}

void MessageProcessor::process_data(const TCPConnSPtr& conn)
{
    // 1. 解析数据，生成客户端消息
    TCPBuffer &buffer = conn->recv_buffer_ref();
    Message *msg = decode(buffer.read(0));

    // 2.1 消息不完整，等待后续数据
    if(msg->get_result() == Message::DeocdeResult::FAILURE)
    {
        return;
    }

    // 2.2 消息解析完成，释放缓冲区，分发消息至对应的处理函数，获取响应
    buffer.release(msg->data_size());
    Message *response = dispatch(msg);

    // 2.3 若响应为空，则关闭连接
    if(response == nullptr)
    {
        conn->close();
        return;
    }

    // 2.3 发送响应
    conn->write(response->data());

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

void MessageProcessor::set_decoder(Decoder *decoder)
{
    decoder_ = decoder;
}

void MessageProcessor::set_dispatcher(Dispatcher *dispatcher)
{
    dispatcher_ = dispatcher;
}