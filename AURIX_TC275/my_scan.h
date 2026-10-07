#ifndef MY_SCAN_H
#define MY_SCAN_H

#include "Ifx_Types.h"


typedef enum
{
    SCAN_IDLE = 0,
    SCAN_BUSY,
    SCAN_OK,
    SCAN_TIMEOUT
} ScanStatus;


typedef struct
{
    uint16 pot;
    uint16 ldr;
    uint16 lm35;

    uint8 validMask;

    uint32 tickMs;
    uint32 skewUs;
    uint32 sequence;

} ScanRecord;


extern ScanRecord g_published;

extern volatile uint32 g_scanErrors;
extern volatile uint32 g_scanSequence;


boolean scan_start(void);
ScanStatus scan_service(void);


static inline uint32 scan_age_ms(uint32 nowMs)
{
    return nowMs - g_published.tickMs;
}


#endif /* MY_SCAN_H */
