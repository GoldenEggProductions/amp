#include "defines.h"
#include "ff.h"
#include "mmc.h"

int main(void)
{
//   InitInterfaceBoard();
//   while(1)
//   {
//      FlashFaultLED(500);
//   }
//   return 0;

   int i;
   for(i = 0; i < 10000; i++) FlashFaultLED(500);
   C_SETBIT(FAULT_LED);

   delay_ms(5000);

   FRESULT f_err_code;
   static FATFS FATFS_Obj;

   disk_initialize(0);

   f_err_code = f_mount(0, &FATFS_Obj);

   FIL fh;
   int variableName = 5;

   f_open(&fh, "/test.txt", FA_WRITE);

   f_printf(&fh, "bar %d", variableName);

   f_close(&fh);

   while(1)
   {
      FlashFaultLED(500);
   }
   return 0;
}
