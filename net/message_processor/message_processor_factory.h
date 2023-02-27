#ifndef SNET_MESSAGE_PROCESSOR_FACTORY_H
#define SNET_MESSAGE_PROCESSOR_FACTORY_H
#include "message_processor.h"

// 消息处理器的工厂方法
class MessageProcessorFactory
{
public:
    /// @brief 创建消息处理器
    virtual MessageProcessor* create_message_processor();

private:

};

#endif