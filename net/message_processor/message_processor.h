#ifndef SNET_MESSAGE_PROCESSOR_H
#define SNET_MESSAGE_PROCESSOR_H
#include "decoder.h"
#include "dispatcher.h"

/// 前向声明TCP连接
class TCPConnection;
typedef std::shared_ptr<TCPConnection> TCPConnSPtr;

// 消息处理器，处理TCP连接接收到的数据
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

#endif