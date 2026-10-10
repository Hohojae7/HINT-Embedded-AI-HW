#include "Rte_SWC_SeatHeating.h"

VAR(uint16, Swc_SeatHeating_VAR_CLEARED) heat_setting = 0;

FUNC(void, Swc_SeatHeating_CODE) Re_SeatHeating(uint16 Heat_Value)
{
  heat_setting = Heat_Value;
  switch (heat_setting) {
  case 0: Rte_Call_R_IoHeating_SetOutputToIdle(); break;
  case 1: Rte_Call_R_IoHeating_SetDutyCycle(0x0800); break;
  case 2: Rte_Call_R_IoHeating_SetDutyCycle(0x1500); break;
  case 3: Rte_Call_R_IoHeating_SetDutyCycle(0x4000); break;
  case 4: Rte_Call_R_IoHeating_SetDutyCycle(0x8000); break;
  default: Rte_Call_R_IoHeating_SetOutputToIdle();
  }
}
