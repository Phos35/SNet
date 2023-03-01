#include "http_message_processor_factory.h"
#include "http_message_processor.h"

MessageProcessor::UPtr
HTTPMessageProcessorFactory::create_message_processor()
{
    HTTPMessageProcessor::UPtr processor = std::make_unique<HTTPMessageProcessor>();
    processor->create();
    return processor;
}