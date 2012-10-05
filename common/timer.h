#ifndef TIMER_H
#define TIMER_H

#include <util/delay.h>

// delay x milliseconds
void delay_ms(unsigned int xms)
{
   while(xms)
   {
      _delay_ms(0.96);
      xms--;
   }
}

#endif
