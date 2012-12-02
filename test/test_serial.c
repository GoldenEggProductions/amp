#include "defines.h"
#include "uart.h"

int main (void)
{
   char tx[128];
   char rx[128];
   uint8_t size;
   int8_t bytesRead;

   InitInterfaceBoard();

   strcpy(tx, "Enter text to be retransmitted:\r\n");
   size = strlen(tx);

   UART_Init(SER_USB_UART);
   for(;;)
   {
      UART_TX_S(SER_USB_UART, tx, size);
      delay_ms(1000);
      bytesRead = UART_RX(SER_USB_UART, rx);
      if( bytesRead > 0 ) break;
   }

   while(1)
   {
      bytesRead = UART_RX(SER_USB_UART, rx);
      if( bytesRead > 0 )
      {
         if( bytesRead > 127 ) bytesRead = 125;
         memcpy(tx, rx, bytesRead);
         tx[bytesRead] = '\r';
         tx[bytesRead+1] = '\n';
         size = bytesRead + 2;
         UART_TX_S(SER_USB_UART, tx, size);
      }
      delay_ms(1000);
   }
   return 0;
}
