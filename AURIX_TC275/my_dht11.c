#include "IfxPort_reg.h"
#include "IfxStm_reg.h"

#include "my_gpio.h"
#include "my_tick.h"
#include "my_dht11.h"


#define DHT_PORT MODULE_P10
#define DHT_PIN  4u


/* fSTM = 100 MHz 기준 1 us = 100 ticks */
#define US_TICKS           100u

/* edge 하나를 최대 200 us 기다림 */
#define EDGE_TIMEOUT_TICKS (200u * US_TICKS)

/* 전원 안정 대기 */
#define POWERUP_MS         1000u

/* DHT11 start signal */
#define START_LOW_MS       20u

/* 측정 사이 휴식 */
#define REST_MS            2000u

/* 0/1 구분 기준 */
#define BIT_SPLIT_US       50u


typedef enum
{
    ST_IDLE,
    ST_START_LOW,
    ST_CAPTURE,
    ST_WAIT
} DhtState;


static DhtState s_state = ST_IDLE;

static uint32 s_t0 = 0;

static DhtPulse s_pulse[40];


/* 정상 결과 */
volatile uint8 g_dhtHumidity = 0;
volatile uint8 g_dhtTemperature = 0;

volatile uint32 g_dhtLastGoodMs = 0;
volatile uint32 g_dhtAgeMs = 0;


/* 진단값 */
volatile DhtStatus g_dhtStatus = DHT_NONE;

volatile uint32 g_dhtOkCount = 0;
volatile uint32 g_dhtErrCount = 0;

volatile uint32 g_captureUs = 0;
volatile uint32 g_captureMaxUs = 0;


/* DATA 선을 LOW로 당김 */
static inline void dht_pull_low(void)
{
    DHT_PORT.OMR.U =
        (1u << (DHT_PIN + 16));
}


/* open-drain DATA 선 release */
static inline void dht_release(void)
{
    DHT_PORT.OMR.U =
        (1u << DHT_PIN);
}


/* 실제 DATA 핀 상태 읽기 */
static inline uint32 dht_line(void)
{
    return (DHT_PORT.IN.U >> DHT_PIN) & 1u;
}


void dht_init(void)
{
    /*
     * P10.4
     * open-drain output 설정
     */
    DHT_PORT.IOCR4.U =
        (DHT_PORT.IOCR4.U & ~0xFFu)
        | (0x18u << 3);

    dht_release();

    s_state = ST_IDLE;
    s_t0 = tick_get();
}


/* pulse 폭이 허용 범위인지 확인 */
static inline boolean width_ok(
    uint32 ticks,
    uint32 minUs,
    uint32 maxUs)
{
    uint32 us = ticks / US_TICKS;

    return (boolean)(
        (us >= minUs)
        && (us <= maxUs));
}


/*
 * DATA 선이 원하는 level이 될 때까지 기다린다.
 * edge 하나당 최대 200 us 대기.
 */
static boolean wait_level(
    uint32 level,
    uint32 *tEdge)
{
    uint32 start = MODULE_STM0.TIM0.U;

    for (;;)
    {
        uint32 now = MODULE_STM0.TIM0.U;

        if (dht_line() == level)
        {
            *tEdge = now;
            return TRUE;
        }

        if ((uint32)(now-start)
           >= EDGE_TIMEOUT_TICKS)
        {
            return FALSE;
        }
    }
}


/*
 * 센서 응답과
 * 40개 data bit의 low/high 폭 측정
 */
static DhtStatus dht_capture(void)
{
    uint32 t0;
    uint32 tRise;
    uint32 tFall;
    uint32 tPrevFall;
    uint32 i;


    /* sensor response low */
    if (!wait_level(0, &t0))
    {
        return DHT_TIMEOUT;
    }

    /* sensor response high */
    if (!wait_level(1, &tRise))
    {
        return DHT_TIMEOUT;
    }

    /* 첫 data low */
    if (!wait_level(0, &tFall))
    {
        return DHT_TIMEOUT;
    }

    /*
     * sensor response:
     * 약 80 us LOW
     * 약 80 us HIGH
     */
    if (!width_ok(tRise - t0, 60u, 100u)
        || !width_ok(tFall - tRise, 60u, 100u))
    {
        return DHT_TIMING;
    }


    tPrevFall = tFall;


    for (i = 0; i < 40u; i++)
    {
        if (!wait_level(1, &tRise))
        {
            return DHT_TIMEOUT;
        }

        if (!wait_level(0, &tFall))
        {
            return DHT_TIMEOUT;
        }


        s_pulse[i].lowUs =
            (tRise - tPrevFall) / US_TICKS;

        s_pulse[i].highUs =
            (tFall - tRise) / US_TICKS;


        tPrevFall = tFall;
    }


    return DHT_OK;
}


/* 40개 pulse를 5 byte로 복원 */
DhtStatus dht_decode_pulses(
    const DhtPulse *pulse,
    uint8 *bytesOut)
{
    uint8 bytes[5] = {0, 0, 0, 0, 0};

    uint8 sum;

    uint32 i;


    for (i = 0; i < 40u; i++)
    {
        uint32 lowUs =
            pulse[i].lowUs;

        uint32 highUs =
            pulse[i].highUs;

        uint8 bit;


        /*
         * 이상한 pulse 폭이면
         * 데이터 자체를 거부
         */
        if (lowUs < 20u
            || lowUs > 100u
            || highUs < 10u
            || highUs > 110u)
        {
            return DHT_TIMING;
        }


        /*
         * High 시간이
         * 50 us보다 길면 1,
         * 아니면 0
         */
        bit =
            (highUs > BIT_SPLIT_US)
            ? 1u
            : 0u;


        bytes[i / 8u] =
            (uint8)(
                (bytes[i / 8u] << 1)
                | bit);
    }


    /*
     * 앞 4 byte 합의 하위 8 bit와
     * checksum byte 비교
     */
    sum =
        (uint8)(
            bytes[0]
            + bytes[1]
            + bytes[2]
            + bytes[3]);


    if (sum != bytes[4])
    {
        return DHT_CHECKSUM;
    }

    if (bytesOut != (uint8 *)0)
    {
        for (i = 0; i < 5u; i++)
        {
            bytesOut[i] = bytes[i];
        }
    }

    return DHT_OK;
}


/* 정상 pulse를 실제 온습도 값으로 publish */
static DhtStatus dht_decode(void)
{
    uint8 bytes[5];

    DhtStatus st =
        dht_decode_pulses(
            s_pulse,
            bytes);


    if (st != DHT_OK)
    {
        return st;
    }


    g_dhtHumidity =
        bytes[0];

    g_dhtTemperature =
        bytes[2];

    g_dhtLastGoodMs =
        tick_get();

    g_dhtOkCount++;


    return DHT_OK;
}


/* DHT11 상태 머신 */
void dht_task(void)
{
    g_dhtAgeMs =
        tick_get() - g_dhtLastGoodMs;


    switch (s_state)
    {
        case ST_IDLE:

            /*
             * 부팅 후 센서 안정화
             * 1초 대기
             */
            if (elapsed(s_t0, POWERUP_MS))
            {
                dht_pull_low();


                s_t0 = tick_get();

                s_state =
                    ST_START_LOW;
            }

            break;


        case ST_START_LOW:

            /*
             * DATA를 약 20 ms LOW
             */
            if (elapsed(s_t0, START_LOW_MS))
            {
                s_state =
                    ST_CAPTURE;
            }

            break;


        case ST_CAPTURE:
        {
            uint32 tBurst =
                MODULE_STM0.TIM0.U;

            DhtStatus st;


            /* DATA 선 release */
            dht_release();


            /* 응답 + 40 bits capture */
            st = dht_capture();


            if (st == DHT_OK)
            {
                st = dht_decode();
            }


            g_captureUs =
                (MODULE_STM0.TIM0.U - tBurst)
                / US_TICKS;


            if (g_captureUs
                > g_captureMaxUs)
            {
                g_captureMaxUs =
                    g_captureUs;
            }


            g_dhtStatus = st;


            if (st != DHT_OK)
            {
                g_dhtErrCount++;
            }


            s_t0 =
                tick_get();

            s_state =
                ST_WAIT;

            break;
        }


        case ST_WAIT:

        default:

            /*
             * 측정 후 2초 휴식
             */
            if (elapsed(s_t0, REST_MS))
            {
                dht_pull_low();

                s_t0 =
                    tick_get();

                s_state =
                    ST_START_LOW;
            }

            break;
    }
}
