#include "http_message_processor_factory.h"
#include "http_message_processor.h"

MessageProcessor*
HTTPMessageProcessorFactory::create_message_processor()
{
    return new HTTPMessageProcessor();
}