#include "helper.h"
#include "bit.h"
#include "timer.h"

#include <avr/io.h>

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
   return (PINA & 0x0F);
}
