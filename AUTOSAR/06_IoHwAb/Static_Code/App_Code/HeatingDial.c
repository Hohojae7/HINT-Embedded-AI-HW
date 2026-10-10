#include "Rte_SWC_HeatingDial.h"

VAR(boolean, Swc_HeatingDial_VAR_CLEARED) control_io_led = FALSE;
VAR(boolean, Swc_HeatingDial_VAR_CLEARED) control_dial_led = FALSE;
VAR(uint16, Swc_HeatingDial_VAR_CLEARED) value_dial = 0;
VAR(uint16, Swc_HeatingDial_VAR_CLEARED) value_position = 0;
VAR(uint16, Swc_HeatingDial_VAR_CLEARED) alive_led = 0;
VAR(uint16, Swc_HeatingDial_VAR_CLEARED) alive = 0;

FUNC(void, Swc_HeatingDial_CODE) Re_HeatingDial(void) 
{
  Rte_Call_R_IoAliveLed_WriteDirect(alive_led);
  if ((alive++ % 10) == 0)
    alive_led ^= 1;

  Rte_Call_R_IoDial_ReadDirect(&value_dial, 1);
  if (value_dial < 255)
    value_position = 4;
  else if (value_dial < 511)
    value_position = 3;
  else if (value_dial < 767)
    value_position = 2;
  else
    value_position = 1;
  Rte_Call_R_Position_Write(value_position);

  Rte_Receive_R_DialLed_On(&control_dial_led);
  if (TRUE == control_dial_led)
    control_io_led = STD_LOW;
  else
    control_io_led = STD_HIGH;
  Rte_Call_R_IoLed_WriteDirect(control_io_led);
}
