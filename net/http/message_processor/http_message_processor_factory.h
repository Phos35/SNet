#ifndef SNET_HTTP_MESSAGE_PROCESSOR_FACTORY_H
#define SNET_HTTP_MESSAGE_PROCESSOR_FACTORY_H
#include "message_processor_factory.h"

class HTTPMessageProcessorFactory : MessageProcessorFactory
{
public:
    /// @brief 创建HTTP消息处理器
    /// @return HTTP消息处理器的指针
    MessageProcessor *create_message_processor();
private:
};

#endif