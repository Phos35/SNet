#include "log_stream.h"

int main()
{
    LogStream stream;
    stream << "A "
           << "B " << 123 << " " << 12.3 << "\n";
    printf("the stream is: %s\n", stream.data());
    return 0;
}