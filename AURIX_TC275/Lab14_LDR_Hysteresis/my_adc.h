#ifndef MY_ADC_H
#define MY_ADC_H

#include "Ifx_Types.h"
#include "IfxVadc_reg.h"
#include "IfxPort_reg.h"
#include "IfxScuWdt.h"

#define CH_POT   7u
#define CH_LDR   6u
#define CH_LM35  5u

void adc_init(void);
uint16 adc_read_once(uint8 ch);
uint16 med3_update(uint16 x);

#endif /* MY_ADC_H */
