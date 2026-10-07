#include "IfxVadc_reg.h"
#include "IfxStm_reg.h"

#include "my_tick.h"
#include "my_adc.h"
#include "my_scan.h"


#define SCAN_TIMEOUT_MS  2u


ScanRecord g_published;

volatile uint32 g_scanErrors =
    0u;

volatile uint32 g_scanSequence =
    0u;


static ScanRecord s_work;

static ScanStatus s_state =
    SCAN_IDLE;

static uint32 s_tFirstSeen =
    0u;


/* ADC queue에 channel 하나 추가 */
static void queue_entry(uint8 ch, uint8 extr)
{
    MODULE_VADC.G[4].QINR0.U =
          (uint32)ch
        | ((uint32)extr << 7);
}


/* 새로운 3-channel scan 시작 */
boolean scan_start(void)
{
    if (s_state == SCAN_BUSY)
    {
        return FALSE;
    }


    s_work.validMask =
        0u;

    s_work.tickMs =
        tick_get();

    s_work.skewUs =
        0u;

    s_tFirstSeen =
        0u;


    /*
     * 첫 CH7만 TREV 대기
     *
     * 그 뒤 CH6, CH5는
     * 바로 이어서 실행
     */
    queue_entry(
        CH_POT,
        1u);

    queue_entry(
        CH_LDR,
        0u);

    queue_entry(
        CH_LM35,
        0u);


    /*
     * 세 변환 시작
     */
    MODULE_VADC.G[4].QMR0.B.TREV =
        1;


    s_state =
        SCAN_BUSY;


    return TRUE;
}


/* 결과 하나가 준비됐으면 가져옴 */
static void take_result(
    uint8 ch,
    uint16 *destination,
    uint8 bit)
{
    uint32 result;


    /*
     * 이미 얻은 channel이면
     * 다시 읽지 않음
     */
    if ((s_work.validMask & bit) != 0u)
    {
        return;
    }


    /*
     * RES.U는 한 번만 읽음
     */
    result =
        MODULE_VADC.G[4].RES[ch].U;


    /*
     * VF == 0
     * 아직 새 결과 없음
     */
    if ((result & 0x80000000u) == 0u)
    {
        return;
    }


    /*
     * 하위 12 bit 결과 저장
     */
    *destination =
        (uint16)(
            result & 0x0FFFu);


    /*
     * 이 channel 수집 완료 표시
     */
    s_work.validMask |=
        bit;


    /*
     * 첫 결과를 본 시각 저장
     */
    if (s_tFirstSeen == 0u)
    {
        s_tFirstSeen =
            MODULE_STM0.TIM0.U;
    }
    else
    {
        s_work.skewUs =
            (MODULE_STM0.TIM0.U - s_tFirstSeen)
            / 100u;
    }
}


/* 진행 중인 scan 상태 확인 */
ScanStatus scan_service(void)
{
    if (s_state != SCAN_BUSY)
    {
        return s_state;
    }


    take_result(
        CH_POT,
        &s_work.pot,
        0x01u);

    take_result(
        CH_LDR,
        &s_work.ldr,
        0x02u);

    take_result(
        CH_LM35,
        &s_work.lm35,
        0x04u);


    /*
     * 001 | 010 | 100
     * = 111
     * = 0x07
     *
     * 세 channel 전부 수집 완료
     */
    if (s_work.validMask == 0x07u)
    {
        g_published =
            s_work;


        g_published.sequence =
            ++g_scanSequence;


        s_state =
            SCAN_OK;


        return SCAN_OK;
    }


    /*
     * 2 ms 안에 전부 못 받으면 timeout
     */
    if (elapsed(
            s_work.tickMs,
            SCAN_TIMEOUT_MS))
    {
        g_scanErrors++;


        MODULE_VADC.G[4].QMR0.B.FLUSH =
            1;


        s_state =
            SCAN_TIMEOUT;


        return SCAN_TIMEOUT;
    }


    return SCAN_BUSY;
}
