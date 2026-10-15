#include "power.h"
#include "power_ll.h"

void power_init(void)
{
    power_ll_init();
}

void power_sleep(void)
{
    power_ll_sleep();
}
