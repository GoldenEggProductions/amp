#define F_CPU 16000000L
#define BAUD 57600

#include "bit.h"
#include "timer.h"
#include "defines.h"
#include "usart.h"

int main (void)
{
   char tx[128];
   char rx[128];
   uint8_t size;
   int8_t bytesRead;

   InitInterfaceBoard();

   strcpy(tx, "Enter text to be retransmitted:\r\n");
   size = strlen(tx);

   USART_Init(SER_USB_USART);
   for(;;)
   {
      USART_TX_S(SER_USB_USART, tx, size);
      delay_ms(1000);
      bytesRead = USART_RX(SER_USB_USART, rx);
      if( bytesRead > 0 ) break;
   }

   while(1)
   {
      bytesRead = USART_RX(SER_USB_USART, rx);
      if( bytesRead > 0 )
      {
         if( bytesRead > 127 ) bytesRead = 125;
         memcpy(tx, rx, bytesRead);
         tx[bytesRead] = '\r';
         tx[bytesRead+1] = '\n';
         size = bytesRead + 2;
         USART_TX_S(SER_USB_USART, tx, size);
      }
      delay_ms(1000);
   }
   return 0;
}
