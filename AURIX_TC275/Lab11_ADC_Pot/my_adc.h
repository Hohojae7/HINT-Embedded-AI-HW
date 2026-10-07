#ifndef MY_ADC_H
#define MY_ADC_H

#include "Ifx_Types.h"
#include "IfxVadc_reg.h"
#include "IfxPort_reg.h"
#include "IfxSrc_reg.h"
#include "IfxScuWdt.h"

#define CH_POT   7u /* A0: potentiometer */
#define CH_LDR   6u /* A1: light divider */
#define CH_LM35  5u /* A2: temperature */

#define ISR_PRIO_STM_TICK  10u
#define ISR_PRIO_ADC       15u

#define ADC_FULL_SCALE       4095u
#define ADC_MV_FROM_CODE(c)  (((uint32)(c) * 5000u) / 4096u)

#define LM35_MC_PER_LSB      122

void adc_int(void);
uint16 adc_read_once(uint8 ch);
void adc_start_tick_stream(void);

uint16 ma_update(uint16 x);
uint16 med3_update(uint16 x);

#endif /* MY_ADC_H */
