#include "message_processor_factory.h"

MessageProcessor::UPtr MessageProcessorFactory::create_message_processor()
{
    // 基类工厂产出基类消息处理器
    MessageProcessor::UPtr processor = std::make_unique<MessageProcessor>();
    processor->create();
    return processor;
}

MessageProcessorFactory::~MessageProcessorFactory()
{

}