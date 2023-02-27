#include "message_processor_factory.h"

MessageProcessor* MessageProcessorFactory::create_message_processor()
{
    // 基类工厂产出基类消息处理器
    MessageProcessor* processor = new MessageProcessor();
    processor->create();
    return processor;
}

MessageProcessorFactory::~MessageProcessorFactory()
{

}