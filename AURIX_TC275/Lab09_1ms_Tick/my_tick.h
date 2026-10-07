#ifndef MY_TICK_H
#define MY_TICK_H

#include "Ifx_Types.h"
#include "IfxStm_reg.h"
#include "IfxSrc_reg.h"

#define ISR_PRIO_STM_TICK 10
#define TICK_1MS           100000u

extern volatile uint32 g_msTicks;

void tick_init(void);

static inline uint32 tick_get(void)
{
    return g_msTicks;
}

static inline boolean elapsed(uint32 t0, uint32 ms)
{
    return (boolean)((uint32)(tick_get() - t0) >= ms);
}

static inline boolean every_ms(uint32 *t0, uint32 ms)
{
    if (!elapsed(*t0,ms))
    {
        return FALSE;
    }

    *t0 += ms;

    return TRUE;
}

#endif /* MY_TICK_H */
