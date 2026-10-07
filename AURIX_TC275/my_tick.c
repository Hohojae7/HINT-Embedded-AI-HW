#include "IfxCpu.h"
#include "my_tick.h"

volatile uint32 g_msTicks = 0;

IFX_INTERRUPT(isrTick, 0, ISR_PRIO_STM_TICK);

void isrTick(void)
{
    /* STM CMP0 비교 상태 정리 */
    MODULE_STM0.ISCR.U = (1u << 0);

    /* 다음 1 ms 시점 예약 */
    MODULE_STM0.CMP[0].U += TICK_1MS;

    /* 소프트웨어 시간 1 ms 증가 */
    g_msTicks++;
}

void tick_init(void)
{
    /* STM 하위 32 bit 전체 비교 */
    MODULE_STM0.CMCON.U = 0x0000001Fu;

    /* 남아 있는 비교 flag 정리 */
    MODULE_STM0.ISCR.U = (1u << 0);

    /* 현재 시각 + 1 ms를 첫 비교 시점으로 */
    MODULE_STM0.CMP[0].U =
        MODULE_STM0.TIM0.U + TICK_1MS;

    /* CMP0 → STMIR0 */
    MODULE_STM0.ICR.B.CMP0OS = 0;

    /* CMP0 비교 인터럽트 허용 */
    MODULE_STM0.ICR.B.CMP0EN = 1;

    /* 이전 서비스 요청 정리 */
    SRC_STM0SR0.B.CLRR = 1;

    /* priority 10 */
    SRC_STM0SR0.B.SRPN = ISR_PRIO_STM_TICK;

    /* CPU0 */
    SRC_STM0SR0.B.TOS = 0;

    /* 서비스 요청 허용 */
    SRC_STM0SR0.B.SRE = 1;
}
