#include "Rte_SWC_SHControl.h"

VAR(boolean, Swc_SHControl_VAR_CLEARED) passenger = TRUE;
VAR(uint16, Swc_SHControl_VAR_CLEARED) read_value_dial = 0;
VAR(uint16, Swc_SHControl_VAR_CLEARED) target_heat = 0;

FUNC(void, Swc_SHControl_CODE) Re_SHControl(uint16 Heat_Dial)
{
  read_value_dial = Heat_Dial;
  if (TRUE == passenger) {
    Rte_Send_P_DialLed_On(TRUE);
    target_heat = read_value_dial;
  } else {
    Rte_Send_P_DialLed_On(FALSE);
    target_heat = 0;
  }
  Rte_Call_R_HeatingElement_Write(target_heat);
}
