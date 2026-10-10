#define RTE_ALLOW_CROSS_HEADER_INCLUSION
#include "Rte_IoHwAb.h"

/* Include SWC Header */
#include "Rte_SWC_SeatHeatingControl.h"

/* Runnable */
void SeatHeatingControl(void)
{
  boolean PassengerSeated = FALSE;

  /* Receive Data */
  Rte_Read_R_SeatSwitch_PassengerDetected(&PassengerSeated);

  if (PassengerSeated == TRUE)
    IoHwAb_DigDirWriteDirect(0, IOHWAB_HIGH);
  else
    IoHwAb_DigDirWriteDirect(0, IOHWAB_LOW);
}
