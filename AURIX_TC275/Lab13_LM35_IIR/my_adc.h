#ifndef MY_ADC_H
#define MY_ADC_H

#include "Ifx_Types.h"
#include "IfxVadc_reg.h"
#include "IfxPort_reg.h"
#include "IfxScuWdt.h"

#define CH_POT   7u
#define CH_LDR   6u
#define CH_LM35  5u

#define LM35_MC_PER_LSB  122

void adc_init(void);
uint16 adc_read_once(uint8 ch);

#endif /* MY_ADC_H */
