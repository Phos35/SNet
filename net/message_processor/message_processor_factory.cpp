#include "message_processor_factory.h"

MessageProcessor* create_message_processor()
{
    // 基类工厂产出基类消息处理器
    return new MessageProcessor();
}
