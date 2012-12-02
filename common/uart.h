// vim: set foldlevel=0 foldmethod=syntax spell:
#ifndef _UART_H
#define _UART_H

#include <avr/io.h>
#include <util/setbaud.h>
#include <avr/interrupt.h>
#include <string.h>

#define UART_BUFFER_SIZE 128
#define UART_BUFFER_MASK ( UART_BUFFER_SIZE - 1 )

static uint8_t UART_RX0_Buffer[UART_BUFFER_SIZE];
static uint8_t UART_RX1_Buffer[UART_BUFFER_SIZE];
static uint8_t UART_TX0_Buffer[UART_BUFFER_SIZE];
static uint8_t UART_TX1_Buffer[UART_BUFFER_SIZE];

static volatile uint8_t UART_RX0_Head;
static volatile uint8_t UART_RX0_Tail;
static volatile uint8_t UART_RX1_Head;
static volatile uint8_t UART_RX1_Tail;

static volatile uint8_t UART_TX0_Head;
static volatile uint8_t UART_TX0_Tail;
static volatile uint8_t UART_TX1_Head;
static volatile uint8_t UART_TX1_Tail;

// Initialize UART specified by uart
void UART_Init(uint8_t uart)
{
   switch(uart)
   {
      case SER_485_UART:
         // baud rate
         UBRR0 = UBRR_VALUE;
         // transmission speed doubler
#if USE_2X
         SETBIT(UCSR0A, U2X0);
#else
         CLEARBIT(UCSR0A, U2X0);
#endif
         // enable rx, tx, and rx interrupt
         UCSR0B = _BV(RXCIE0) | _BV(RXEN0)  | _BV(TXEN0);
         // 1 stop bit, 8 bit data frame size
         UCSR0C = _BV(USBS0)  | _BV(UCSZ01) | _BV(UCSZ00);

         UART_RX0_Head = 0;
         UART_TX0_Head = 0;
         UART_RX0_Tail = 0;
         UART_TX0_Tail = 0;
         break;

      case SER_USB_UART:
         // baud rate
         UBRR1 = UBRR_VALUE;
         // transmission speed doubler
#if USE_2X
         SETBIT(UCSR1A, U2X1);
#else
         CLEARBIT(UCSR1A, U2X1);
#endif
         // enable rx, tx, and rx interrupt
         UCSR1B = _BV(RXCIE1) | _BV(RXEN1)  | _BV(TXEN1);
         // 8 bit data frame size
         UCSR1C = _BV(UCSZ11) | _BV(UCSZ10);

         UART_RX1_Head = 0;
         UART_TX1_Head = 0;
         UART_RX1_Tail = 0;
         UART_TX1_Tail = 0;
         break;

      default:
         break;
   }
   sei();
}

// UART receive interrupt
ISR(USART0_RX_vect)
{
   uint8_t data = UDR0;
   UART_RX0_Head = (UART_RX0_Head + 1) & UART_BUFFER_MASK;

   if( UART_RX0_Head == UART_RX0_Tail )
   {
      // buffer overflow
   }

   UART_RX0_Buffer[UART_RX0_Head] = data;
}

// UART receive interrupt
ISR(USART1_RX_vect)
{
   uint8_t data = UDR1;
   UART_RX1_Head = (UART_RX1_Head + 1) & UART_BUFFER_MASK;

   if( UART_RX1_Head == UART_RX1_Tail )
   {
      // buffer overflow
   }

   UART_RX1_Buffer[UART_RX1_Head] = data;
}

ISR(USART0_UDRE_vect)
{
   if( UART_TX0_Head != UART_TX0_Tail )
   {
      UART_TX0_Tail = ( UART_TX0_Tail + 1 ) & UART_BUFFER_MASK;
      UDR0 = UART_TX0_Buffer[UART_TX0_Tail];
   }
   else
   {
      CLEARBIT(UCSR0B, UDRIE0);
   }
}

ISR(USART1_UDRE_vect)
{
   if( UART_TX1_Head != UART_TX1_Tail )
   {
      UART_TX1_Tail = (UART_TX1_Tail + 1) & UART_BUFFER_MASK;
      UDR1 = UART_TX1_Buffer[UART_TX1_Tail];
   }
   else
   {
      CLEARBIT(UCSR1B, UDRIE1);
   }
}

// UART receive
int8_t UART_RX(uint8_t uart, char *data)
{
   switch(uart)
   {
      case SER_485_UART:
         if( UART_RX0_Head == UART_RX0_Tail ) return 0;
         UART_RX0_Tail = ( UART_RX0_Tail + 1 ) & UART_BUFFER_MASK;
         memcpy(data, &UART_RX0_Buffer[UART_RX0_Tail], 1);
         return 1;
         break;
      case SER_USB_UART:
         if( UART_RX1_Head == UART_RX1_Tail ) return 0;
         UART_RX1_Tail = ( UART_RX1_Tail + 1 ) & UART_BUFFER_MASK;
         memcpy(data, &UART_RX1_Buffer[UART_RX1_Tail], 1);
         return 1;
         break;
   }
   return -1;
}

// UART transmit
int8_t UART_TX(uint8_t uart, char *data)
{
   uint8_t temp;

   switch(uart)
   {
      case SER_485_UART:
         temp = ( UART_TX0_Head + 1 ) & UART_BUFFER_MASK;
         if( temp == UART_TX0_Tail ) return -1;

         UART_TX0_Buffer[temp] = *data;
         UART_TX0_Head = temp;

         SETBIT(UCSR0B, UDRIE0);
         break;
      case SER_USB_UART:
         temp = ( UART_TX1_Head + 1 ) & UART_BUFFER_MASK;
         if( temp == UART_TX1_Tail ) return -1;

         UART_TX1_Buffer[temp] = *data;
         UART_TX1_Head = temp;

         SETBIT(UCSR1B, UDRIE1);
         break;
   }
   return 1;
}

// UART transmit string
int8_t UART_TX_S(uint8_t uart, char *data, uint8_t length)
{
   uint8_t retval = 0;
   uint8_t i;

   for(i = 0; i < length; i++)
   {
      retval = UART_TX(uart, data+i);
      if( !retval ) break;
   }
   return retval;
}
#endif
