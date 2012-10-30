#ifndef _DEFINES_H
#define _DEFINES_H

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

#define SER_485_USART 0
#define SER_USB_USART 1

#define DIP_SW_0_MASK 0x1
#define DIP_SW_1_MASK 0x2
#define DIP_SW_2_MASK 0x4
#define DIP_SW_3_MASK 0x8

void InitInterfaceBoard(void)
{
   DDRA  = 0x20; // set FAULT_LED as output and dip switches as input
   PORTA = 0x0F; // enable pullup resistor for dip switches

   DDRC  = 0xC0;
   PORTC = 0x00;
}

void FlashFaultLED(uint16_t interval)
{
   C_CLEARBIT(FAULT_LED);
   delay_ms(interval);
   C_SETBIT(FAULT_LED);
   delay_ms(interval);
}

uint8_t GetDipSwitchSetting(void)
{
   uint8_t retval = 0;
   retval = (PINA & 0x0F);
   return retval;
}

#endif
