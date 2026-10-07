#ifndef MY_ADC_H
#define MY_ADC_H

#include "Ifx_Types.h"
#include "IfxVadc_reg.h"
#include "IfxPort_reg.h"
#include "IfxSrc_reg.h"
#include "IfxScuWdt.h"

#define CH_POT             7u
#define ISR_PRIO_ADC       15u
#define ISR_PRIO_STM_TICK  10u

void adc_init(void);
void adc_start_tick_stream(void);
uint16 ma_update(uint16 x);

#endif /* MY_ADC_H */
