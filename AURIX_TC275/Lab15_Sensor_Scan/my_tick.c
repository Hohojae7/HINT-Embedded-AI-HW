#include "IfxCpu.h"
#include "my_tick.h"

volatile uint32 g_msTicks = 0u;

IFX_INTERRUPT(isrTick, 0, ISR_PRIO_STM_TICK);

void isrTick(void)
{
    MODULE_STM0.ISCR.U =
        (1u << 0);

    MODULE_STM0.CMP[0].U +=
        TICK_1MS;

    g_msTicks++;
}


void tick_init(void)
{
    MODULE_STM0.CMCON.U =
        0x0000001Fu;

    MODULE_STM0.ISCR.U =
        (1u << 0);

    MODULE_STM0.CMP[0].U =
        MODULE_STM0.TIM0.U + TICK_1MS;

    MODULE_STM0.ICR.B.CMP0OS = 0;
    MODULE_STM0.ICR.B.CMP0EN = 1;

    SRC_STM0SR0.B.CLRR = 1;
    SRC_STM0SR0.B.SRPN = ISR_PRIO_STM_TICK;
    SRC_STM0SR0.B.TOS = 0;
    SRC_STM0SR0.B.SRE = 1;
}
