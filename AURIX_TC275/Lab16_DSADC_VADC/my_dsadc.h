#ifndef MY_DSADC_H
#define MY_DSADC_H

#include "Ifx_Types.h"
#include "IfxDsadc_reg.h"
#include "IfxScuWdt.h"
#include "my_adc.h"

#define DSADC_COUNTS_PER_VOLT  4340
#define DSADC_VCM_MV           2500
#define DSADC_SETTLE_MS        2u

void dsadc_init(void);
uint16 dsadc_read_once(uint8 ch);
void dsadc_select(uint8 ch);
uint16 dsadc_read_current(void);
uint8 dsadc_current(void);

#endif /* MY_DSADC_H */
