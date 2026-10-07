#include "my_adc.h"

void adc_init(void)
{
    uint16 pw = IfxScuWdt_getCpuWatchdogPassword();
    uint32 timeout = 100000u;

    IfxScuWdt_clearCpuEndinit(pw);

    MODULE_VADC.CLC.U = 0x00000000u;
    (void)MODULE_VADC.CLC.U;

    while ((MODULE_VADC.CLC.B.DISS != 0u)
        && (timeout > 0u))
    {
        timeout--;
    }

    IfxScuWdt_setCpuEndinit(pw);

    if (timeout == 0u)
    {
        while (1)
        {
        }
    }

    MODULE_VADC.GLOBCFG.U =
          (1u << 31)
        | (1u << 15)
        | 5u;

    MODULE_VADC.G[4].ARBCFG.U =
          (3u << 0)
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


void adc_start_tick_stream(void)
{
    /* RES7 결과 event 허용 */
    MODULE_VADC.G[4].RCR[7].U =
        (1u << 31);

    /* RES7 → Service Request 0 */
    MODULE_VADC.G[4].REVNP0.B.REV7NP = 0;

    MODULE_VADC.G[4].REFCLR.U =
        0xFFFFu;

    SRC_VADCG4SR0.B.CLRR = 1;
    SRC_VADCG4SR0.B.SRPN = ISR_PRIO_ADC;
    SRC_VADCG4SR0.B.TOS = 0;
    SRC_VADCG4SR0.B.SRE = 1;

    /*
     * CH7
     * RF   = 변환 후 다시 queue
     * EXTR = TREV가 올 때까지 대기
     */
    MODULE_VADC.G[4].QINR0.U =
          (uint32)CH_POT
        | (1u << 5)
        | (1u << 7);
}


#define MA_N 16u

static uint16 s_buf[MA_N];
static uint32 s_sum = 0u;
static uint32 s_idx = 0u;


uint16 ma_update(uint16 x)
{
    s_sum += x;

    s_sum -=
        s_buf[s_idx];

    s_buf[s_idx] = x;

    s_idx =
        (s_idx + 1u)
        & (MA_N - 1u);

    return (uint16)(
        s_sum / MA_N);
}
