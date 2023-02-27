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

void MessageProcessor::process_data(const TCPConnSPtr& conn, const std::string &data)
{
    // 1. 解析数据，生成客户端消息
    Message* msg = decode(data);

    // 2.1 消息不完整，等待后续数据
    if(msg->get_result() == Message::DeocdeResult::FAILURE)
    {
        delete msg;
        return;
    }

    // 2.2 消息解析完成，分发至对应的处理函数，获取响应
    Message* response = dispatch(msg);

    // 2.3 发送响应
    conn->write(response->data());

    // 2.4 处理连接状态
    process_conn(conn, msg);

    // 销毁指针
    delete msg;
    delete response;
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