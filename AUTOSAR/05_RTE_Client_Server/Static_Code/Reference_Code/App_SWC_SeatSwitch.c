#define RTE_ALLOW_CROSS_HEADER_INCLUSION
#include "Rte_IoHwAb.h"

/* Include SWC Header */
#include "Rte_SWC_SeatSwitch.h"

/* Runnable*/
void SeatSwitch(void)
{
  static boolean Passenger = IOHWAB_LOW;

  /* Read Digtal Input */

  Rte_Call_R_IO_ReadDirect(&Passenger);

  if (Passenger == IOHWAB_HIGH)
    Rte_Write_P_SeatSwitch_PassengerDetected(TRUE);
  else
    Rte_Write_P_SeatSwitch_PassengerDetected(FALSE);
}
