#include "my_adc.h"

void adc_init(void)
{
    uint16 pw = IfxScuWdt_getCpuWatchdogPassword();
    uint32 timeout = 100000u;

    IfxScuWdt_clearCpuEndinit(pw);

    MODULE_VADC.CLC.U = 0u;
    (void)MODULE_VADC.CLC.U;

    while ((MODULE_VADC.CLC.B.DISS !=0u)
        && (timeout > 0u))
    {
        timeout--;
    }

    IfxScuWdt_setCpuEndinit(pw);

    if (timeout == 0u)
    {
        while(1)
        {
        }
    }

    MODULE_VADC.GLOBCFG.U =
          (1u << 31)
        | (1u << 15)
        | 5u;

    MODULE_VADC.G[4].ARBCFG.U =
          3u
        | (1u << 7);

    while (MODULE_VADC.G[4].ARBCFG.B.CAL != 0u)
    {
    }

    MODULE_VADC.G[4].ARBPR.U =
          (1u << 24)
        | 3u;

    MODULE_VADC.G[4].ICLASS[0].U =
        4u;

    MODULE_VADC.G[4].CHCTR[7].U =
          (7u << 16)
        | (1u << 21);

    MODULE_VADC.G[4].CHCTR[6].U =
          (6u << 16)
        | (1u << 21);

    MODULE_VADC.G[4].CHCTR[5].U =
          (5u << 16)
        | (1u << 21);

    IfxScuWdt_clearCpuEndinit(pw);

    MODULE_P40.PDISC.U |=
          (1u << 7)
        | (1u << 8)
        | (1u << 9);

    (void)MODULE_P40.PDISC.U;

    IfxScuWdt_setCpuEndinit(pw);

    MODULE_P32.IOCR0.B.PC3 = 0x00u;
    MODULE_P32.IOCR4.B.PC4 = 0x00u;
    MODULE_P23.IOCR0.B.PC1 = 0x00u;
    MODULE_P23.IOCR0.B.PC2 = 0x00u;

    MODULE_VADC.G[4].QMR0.U = 1u;
}


uint16 adc_read_once(uint8 ch)
{
    uint32 result;

    MODULE_VADC.G[4].QINR0.U =
        (uint32)ch;

    do
    {
        result =
            MODULE_VADC.G[4].RES[ch].U;

    } while ((result & 0x80000000u) == 0u);

    return (uint16)(
        result & 0x0FFFu);
}
