#include "power_manager.h"
#include "power.h"

void power_manager_init(void)
{
    power_ll_init();
}

void power_manager_sleep(void)
{
    power_ll_sleep();
}
