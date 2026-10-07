#include "my_dsadc.h"
#include "my_tick.h"


#define MUX_POT  ((1u << 15) | (1u << 8) | (2u << 0))
#define MUX_LDR  ((1u << 15) | (1u << 8) | (2u << 2))


static uint8 s_current =
    0xFFu;


void dsadc_init(void)
{
    uint16 pw =
        IfxScuWdt_getCpuWatchdogPassword();


    IfxScuWdt_clearCpuEndinit(pw);

    MODULE_DSADC.CLC.U =
        0u;

    (void)MODULE_DSADC.CLC.U;

    IfxScuWdt_setCpuEndinit(pw);


    while (MODULE_DSADC.CLC.B.DISS != 0u)
    {
    }


    MODULE_DSADC.GLOBCFG.B.MCSEL =
        1;


    MODULE_DSADC.CH[2].MODCFG.U =
          (1u << 31)
        | (2u << 24);


    MODULE_DSADC.CH[3].MODCFG.U =
          (1u << 31)
        | (2u << 24)
        | (1u << 23)
        | (4u << 16)
        | MUX_POT;


    s_current =
        CH_POT;


    MODULE_DSADC.CH[3].DICFG.U =
          (1u << 31)
        | (1u << 20);


    MODULE_DSADC.CH[3].FCFGC.U =
          63u
        | (2u << 8)
        | (1u << 10)
        | (3u << 12)
        | (63u << 16);


    MODULE_DSADC.CH[3].FCFGM.U =
          (1u << 0)
        | (1u << 1)
        | (1u << 3);


    MODULE_DSADC.GLOBRC.U |=
          (1u << 19)
        | (1u << 3);
}


static void mux_of(
    uint8 ch,
    uint32 *mux,
    sint32 *sign)
{
    switch (ch)
    {
        case CH_POT:

            *mux =
                MUX_POT;

            *sign =
                -1;

            break;


        case CH_LDR:

        default:

            *mux =
                MUX_LDR;

            *sign =
                +1;

            break;
    }
}


static void wait_ms(uint32 ms)
{
    uint32 start =
        tick_get();

    while (!elapsed(start, ms))
    {
    }
}


void dsadc_select(uint8 ch)
{
    uint32 mux;
    sint32 sign;


    if (ch == s_current)
    {
        return;
    }


    mux_of(
        ch,
        &mux,
        &sign);

    (void)sign;


    MODULE_DSADC.CH[3].MODCFG.U =
        mux;


    s_current =
        ch;
}


uint8 dsadc_current(void)
{
    return s_current;
}


uint16 dsadc_read_current(void)
{
    uint32 mux;
    sint32 sign;
    sint32 raw;
    sint32 mv;


    mux_of(
        s_current,
        &mux,
        &sign);

    (void)mux;


    raw =
        (sint32)(sint16)
        MODULE_DSADC.CH[3].RESM.U;


    mv =
        DSADC_VCM_MV
        + sign
        * ((raw * 1000)
        / DSADC_COUNTS_PER_VOLT);


    if (mv < 0)
    {
        mv = 0;
    }


    if (mv > 5000)
    {
        mv = 5000;
    }


    return (uint16)(
        ((uint32)mv * 4095u)
        / 5000u);
}


uint16 dsadc_read_once(uint8 ch)
{
    if (ch != s_current)
    {
        dsadc_select(ch);

        wait_ms(
            DSADC_SETTLE_MS);
    }


    return dsadc_read_current();
}
