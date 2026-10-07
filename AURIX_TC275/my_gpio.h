#ifndef MY_GPIO_H
#define MY_GPIO_H

#include "Ifx_Types.h"
#include "IfxPort_reg.h"

#define LED1_PORT  MODULE_P10
#define LED1_PIN   1

#define LED2_PORT  MODULE_P10
#define LED2_PIN   2

#define SW1_PORT   MODULE_P02
#define SW1_PIN    0

#define SW2_PORT   MODULE_P02
#define SW2_PIN    1

#define RGB_R_PORT MODULE_P02
#define RGB_R_PIN  7

#define RGB_G_PORT MODULE_P10
#define RGB_G_PIN  5

#define RGB_B_PORT MODULE_P10
#define RGB_B_PIN  3

static inline void gpio_init_out(volatile Ifx_P *port, int pin)
{
    volatile unsigned int *iocr =
        (volatile unsigned int *)&port->IOCR0;

    iocr += (pin / 4);

    *iocr = (*iocr & ~(0xFFu << ((pin % 4) * 8)))
          | (0x80u << ((pin % 4) * 8));
}

static inline void gpio_init_in_pullup(volatile Ifx_P *port, int pin)
{
    volatile unsigned int *iocr =
        (volatile unsigned int *)&port->IOCR0;

    iocr += (pin / 4);

    *iocr = (*iocr & ~(0xFFu << ((pin % 4) * 8)))
          | (0x10u << ((pin % 4) * 8));
}

static inline void gpio_write(volatile Ifx_P *port, int pin, int level)
{
    port->OMR.U =
        level ? (1u << pin)
              : (1u << (pin + 16));
}

static inline int gpio_read(volatile Ifx_P *port, int pin)
{
    return (int)((port->IN.U >> pin) & 1u);
}

static inline void gpio_toggle(volatile Ifx_P *port, int pin)
{
    port->OMR.U =
        (1u << pin)
        | (1u << (pin + 16));
}

#endif /* MY_GPIO_H */
