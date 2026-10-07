#ifndef MY_DHT11_H
#define MY_DHT11_H

#include "Ifx_Types.h"

typedef enum
{
    DHT_NONE = 0,
    DHT_OK,
    DHT_TIMEOUT,
    DHT_TIMING,
    DHT_CHECKSUM
} DhtStatus;


/* 정상 프레임일 때 갱신되는 값 */
extern volatile uint8 g_dhtHumidity;
extern volatile uint8 g_dhtTemperature;

extern volatile uint32 g_dhtLastGoodMs;
extern volatile uint32 g_dhtAgeMs;


/* 진단용 값 */
extern volatile DhtStatus g_dhtStatus;

extern volatile uint32 g_dhtOkCount;
extern volatile uint32 g_dhtErrCount;

extern volatile uint32 g_captureUs;
extern volatile uint32 g_captureMaxUs;


void dht_init(void);
void dht_task(void);

/* DHT11 펄스 한 비트의 low/high 시간 */
typedef struct
{
        uint32 lowUs;
        uint32 highUs;
} DhtPulse;


DhtStatus dht_decode_pulses(
    const DhtPulse *pulse,
    uint8 *bytesOut);

#endif /* MY_DHT11_H */
