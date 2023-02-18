#include "epoller.h"
#include <vector>
#include <unistd.h>

void read_process()
{
    char buf[64] = {0};
    read(0, buf, 64);
    printf("What you input is %s\n", buf);
}

int main()
{
    EPoller epoller;

    Event e(0, EPOLLIN);
    e.set_read_callback(read_process);
    epoller.add_event(e);
    for (int j = 0; j < 10; j++)
    {
        std::vector<Event> events;
        size_t fired_cnt = epoller.get_fired_events(events);
        printf("Fired cnt: %lu\n", fired_cnt);

        for (int i = 0; i < fired_cnt; i++)
        {
            printf("events: %s\n", Event::mask_to_string(e.mask()).c_str());
            e.process();
        }
    }

    return 0;
}