#include "stdint.h"
#include "stdbool.h"

#include "stdio.h"
#include "event.h"

static uint32_t sEventFlag;

void Kernel_event_flag_init(void)
{
    sEventFlag = 0;
}

void Kernel_event_flag_set(KernelEventFlag_t event)
{
    sEventFlag |= (uint32_t)event;
}

void Kerenl_event_flag_clear(KernelEventFlag_t event)
{
    sEventFlag &= ~((uint32_t)event);
}

bool Kernel_event_flag_check(KernelEventFlag_t event)
{
    if(sEventFlag & (uint32_t)event)
    {
        Kerenl_event_flag_clear(event);
        return true;
    }
    return false;
}