#include <avr/io.h>

#define F_CPU 16000000L

#include "bit.h"
#include "timer.h"

#define FAULT_LED PORTA, 5

int main (void)
{
   DDRA=0x20;
   while(1)
   {
      C_CLEARBIT(FAULT_LED);
      delay_ms(500);
      C_SETBIT(FAULT_LED);
      delay_ms(500);
   }
}
