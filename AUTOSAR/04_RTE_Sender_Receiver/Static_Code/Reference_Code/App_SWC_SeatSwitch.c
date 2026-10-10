#define RTE_ALLOW_CROSS_HEADER_INCLUSION
#include "Rte_IoHwAb.h"

/* Include SWC Header */
#include "Rte_SWC_SeatSwitch.h"

/* Runnable*/
void SeatSwitch(void)
{
  static uint8 Passenger = 0;

  Rte_Write_P_SeatSwitch_PassengerDetected( Passenger);

  if (Passenger == 1)
  {
    Passenger = 0;
  }
  else
  {
    Passenger = 1;
  }
}
