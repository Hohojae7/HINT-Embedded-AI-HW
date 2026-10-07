#include "my_adc.h"

void adc_init(void)
{
    uint16 pw = IfxScuWdt_getCpuWatchdogPassword();

    /* 1) VADC module clock */
    IfxScuWdt_clearCpuEndinit(pw);

    MODULE_VADC.CLC.U = 0x00000000u;

    (void)MODULE_VADC.CLC.U;

    uint32 timeout = 100000u;

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


    volatile uint32 settle = 8u;

    while (settle > 0u)
    {
        settle--;
    }


    /*
     * 2) ADC clock
     * 100 MHz / (5 + 1)
     * ≈ 16.67 MHz
     */
    MODULE_VADC.GLOBCFG.U =
          (1u << 31)
        | (1u << 15)
        | (5u << 0);


    /*
     * 3) Group 4 analog converter ON
     */
    MODULE_VADC.G[4].ARBCFG.U =
          (3u << 0)
        | (1u << 7);

    while (MODULE_VADC.G[4].ARBCFG.B.CAL != 0u)
    {
    }


    /*
     * 4) Queue arbitration slot 0
     */
    MODULE_VADC.G[4].ARBPR.U =
          (1u << 24)
        | (3u << 0);


    /*
     * 5) 12-bit
     * sample time = 360 ns
     */
    MODULE_VADC.G[4].ICLASS[0].U =
          (0u << 8)
        | (4u << 0);


    /*
     * 6) Channel → Result register
     */
    MODULE_VADC.G[4].CHCTR[7].U =
          (7u << 16)
        | (1u << 21);

    MODULE_VADC.G[4].CHCTR[6].U =
          (6u << 16)
        | (1u << 21);

    MODULE_VADC.G[4].CHCTR[5].U =
          (5u << 16)
        | (1u << 21);


    /*
     * 7) P40.7 / P40.8 / P40.9
     * digital input disable
     */
    IfxScuWdt_clearCpuEndinit(pw);

    MODULE_P40.PDISC.U |=
          (1u << 7)
        | (1u << 8)
        | (1u << 9);

    (void)MODULE_P40.PDISC.U;

    IfxScuWdt_setCpuEndinit(pw);


    /*
     * 8) 같은 A0/A1/A2에 연결된
     * GPIO alias의 pull-up 제거
     */
    MODULE_P32.IOCR0.B.PC3 = 0x00u;
    MODULE_P32.IOCR4.B.PC4 = 0x00u;
    MODULE_P23.IOCR0.B.PC1 = 0x00u;
    MODULE_P23.IOCR0.B.PC2 = 0x00u;


    /*
     * 9) Queue request gate open
     */
    MODULE_VADC.G[4].QMR0.U =
        (1u << 0);
}


uint16 adc_read_once(uint8 ch)
{
    uint32 result;


    /*
     * 변환할 channel을 queue에 넣음
     */
    MODULE_VADC.G[4].QINR0.U =
        (uint32)ch;


    /*
     * VF = 1이 될 때까지 polling
     */
    do
    {
        result =
            MODULE_VADC.G[4].RES[ch].U;

    } while ((result & 0x80000000u) == 0u);


    /*
     * 하위 12 bit만 ADC 결과
     */
    return (uint16)(result & 0x0FFFu);
}


/*
 * 뒤 Lab에서 사용할 함수들
 */
void adc_start_tick_stream(void)
{
    MODULE_VADC.G[4].RCR[7].U =
        (1u << 31);

    MODULE_VADC.G[4].REVNP0.B.REV7NP = 0;

    MODULE_VADC.G[4].REFCLR.U =
        0xFFFFu;

    SRC_VADCG4SR0.B.CLRR = 1;
    SRC_VADCG4SR0.B.SRPN = ISR_PRIO_ADC;
    SRC_VADCG4SR0.B.TOS = 0;
    SRC_VADCG4SR0.B.SRE = 1;

    MODULE_VADC.G[4].QINR0.U =
          (uint32)CH_POT
        | (1u << 5)
        | (1u << 7);
}


#define MA_N 16u

static uint16 s_maBuf[MA_N];
static uint32 s_maSum = 0u;
static uint32 s_maIdx = 0u;

uint16 ma_update(uint16 x)
{
    s_maSum += x;

    s_maSum -=
        s_maBuf[s_maIdx];

    s_maBuf[s_maIdx] = x;

    s_maIdx =
        (s_maIdx + 1u)
        & (MA_N - 1u);

    return (uint16)(
        s_maSum / MA_N);
}


uint16 med3_update(uint16 x)
{
    static uint16 a = 0u;
    static uint16 b = 0u;

    uint16 hi =
        (a > b) ? a : b;

    uint16 lo =
        (a > b) ? b : a;

    uint16 middle =
        (x > hi)
        ? hi
        : ((x < lo) ? lo : x);

    a = b;
    b = x;

    return middle;
}
