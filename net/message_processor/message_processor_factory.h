#ifndef SNET_MESSAGE_PROCESSOR_FACTORY_H
#define SNET_MESSAGE_PROCESSOR_FACTORY_H
#include "message_processor.h"

// 消息处理器的工厂方法
class MessageProcessorFactory
{
public:
    typedef std::unique_ptr<MessageProcessorFactory> UPtr;

    /// @brief 创建消息处理器
    virtual MessageProcessor::UPtr create_message_processor();
    virtual ~MessageProcessorFactory();

private:

};

#endif