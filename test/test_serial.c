#define F_CPU 16000000L
#define BAUD 57600

#include "bit.h"
#include "timer.h"
#include "defines.h"
#include "usart.h"

int main (void)
{
   char data[] = "Hello World!\r\n";
   uint8_t size = strlen(data);

   USART_Init(SER_USB_USART);
   while(1)
   {
      USART_TX_S(SER_USB_USART, data, size);
      delay_ms(1000);
   }
   return 0;
}
