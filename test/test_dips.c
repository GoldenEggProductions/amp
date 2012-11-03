#include "defines.h"

int main (void)
{
   uint8_t ui8_dip_switches;
   int i;

   InitInterfaceBoard();

   while(1)
   {
      ui8_dip_switches = GetDipSwitchSetting();

      for(i = 0; i < ui8_dip_switches; i++)
      {
         FlashFaultLED(250);
      }
      delay_ms(3000);
   }

   return 0;
}
