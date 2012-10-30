#define F_CPU 16000000L

#include "bit.h"
#include "timer.h"
#include "defines.h"

int main (void)
{
   InitInterfaceBoard();
   while(1)
   {
      FlashFaultLED(500);
   }
}
