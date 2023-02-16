#include "timestamp.h"
#include <stdio.h>

int main()
{
    printf("current time : %s\n", Timestamp::now().format("%Y-%m-%d %H:%M:%S.%s").c_str());
    return 0;
}