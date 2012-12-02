#include "helper.h"

int main (void)
{
   InitInterfaceBoard();
   while(1)
   {
      FlashFaultLED(500);
   }
}
