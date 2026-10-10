#include "Rte_Swc_SeatSwitch.h"

VAR(uint8, Swc_SeatSwitch_VAR_CLEARED) Sig1 = 0;

FUNC(void, Swc_SeatSwitch_CODE) Re_SeatSwitch(void) /* Rx_PD <--> Tx_SH */
{
  Rte_Read_R_RcCommand_ECU2_Msg_PD_Sig1(&Sig1);

  if (Sig1) {
      Rte_Write_P_SeatSwitch_PassengerDetected(TRUE);
  } else {
      Rte_Write_P_SeatSwitch_PassengerDetected(FALSE);
  }

  Rte_Write_P_RcStatus_ECU1_Msg_SH_Sig1(Sig1);
}
