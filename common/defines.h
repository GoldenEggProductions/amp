#ifndef _DEFINES_H
#define _DEFINES_H

#define F_CPU 16000000L
#define BAUD 57600

#include <avr/io.h>
#define TRUE  (1)
#define FALSE (0)

// Microcontroller
#define DIP_SW_0 PORTA, 0
#define DIP_SW_1 PORTA, 1
#define DIP_SW_2 PORTA, 2
#define DIP_SW_3 PORTA, 3
#define FAULT_LED PORTA, 5

#define SER_485_RX PORTD, 0
#define SER_485_TX PORTD, 1
#define SER_USB_RX PORTD, 2
#define SER_USB_TX PORTD, 3

#define SER_485_UART 0
#define SER_USB_UART 1

#define DIP_SW_0_MASK 0x1
#define DIP_SW_1_MASK 0x2
#define DIP_SW_2_MASK 0x4
#define DIP_SW_3_MASK 0x8

#endif
