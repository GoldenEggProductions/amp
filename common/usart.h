// vim: set foldlevel=0 foldmethod=syntax spell:
#ifndef _USART_H
#define _USART_H

#include <avr/io.h>
#include <util/setbaud.h>
#include <avr/interrupt.h>
#include <string.h>

#define USART_BUFFER_SIZE 128
#define USART_BUFFER_MASK ( USART_BUFFER_SIZE - 1 )

static uint8_t USART_RX0_Buffer[USART_BUFFER_SIZE];
static uint8_t USART_RX1_Buffer[USART_BUFFER_SIZE];
static uint8_t USART_TX0_Buffer[USART_BUFFER_SIZE];
static uint8_t USART_TX1_Buffer[USART_BUFFER_SIZE];

static volatile uint8_t USART_RX0_Head;
static volatile uint8_t USART_RX0_Tail;
static volatile uint8_t USART_RX1_Head;
static volatile uint8_t USART_RX1_Tail;

static volatile uint8_t USART_TX0_Head;
static volatile uint8_t USART_TX0_Tail;
static volatile uint8_t USART_TX1_Head;
static volatile uint8_t USART_TX1_Tail;

// Initialize USART specified by usart
void USART_Init(uint8_t usart)
{
   switch(usart)
   {
      case SER_485_USART:
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

         USART_RX0_Head = 0;
         USART_TX0_Head = 0;
         USART_RX0_Tail = 0;
         USART_TX0_Tail = 0;
         break;

      case SER_USB_USART:
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

         USART_RX1_Head = 0;
         USART_TX1_Head = 0;
         USART_RX1_Tail = 0;
         USART_TX1_Tail = 0;
         break;

      default:
         break;
   }
   sei();
}

// USART receive interrupt
ISR(USART0_RX_vect)
{
   uint8_t temp = 0;

   uint8_t data = UDR0;
   temp = (USART_RX0_Head + 1) & USART_BUFFER_MASK;
   USART_RX0_Head = temp;

   if( temp == USART_RX0_Tail )
   {
      // buffer overflow
   }

   USART_RX0_Buffer[USART_RX0_Head] = data;
}

// USART transmit interrupt
ISR(USART1_RX_vect)
{
   uint8_t temp = 0;

   uint8_t data = UDR1;
   temp = (USART_RX1_Head + 1) & USART_BUFFER_MASK;
   USART_RX1_Head = temp;

   if( temp == USART_RX1_Tail )
   {
      // buffer overflow
   }

   USART_RX1_Buffer[USART_RX1_Head] = data;
}

ISR(USART0_UDRE_vect)
{
   uint8_t temp = 0;

   if( USART_TX0_Head != USART_TX0_Tail )
   {
      temp = ( USART_TX0_Tail + 1 ) & USART_BUFFER_MASK;
      USART_TX0_Tail = temp;

      UDR0 = USART_TX0_Buffer[temp];
   }
   else
   {
      CLEARBIT(UCSR0B, UDRIE0);
   }
}

ISR(USART1_UDRE_vect)
{
   uint8_t temp = 0;

   if( USART_TX1_Head != USART_TX1_Tail )
   {
      temp = (USART_TX1_Tail + 1) & USART_BUFFER_MASK;
      USART_TX1_Tail = temp;

      UDR1 = USART_TX1_Buffer[temp];
   }
   else
   {
      CLEARBIT(UCSR1B, UDRIE1);
   }
}

// USART receive
int8_t USART_RX(uint8_t usart, char *data)
{
   uint8_t temp;
   switch(usart)
   {
      case SER_485_USART:
         if( USART_RX0_Head == USART_RX0_Tail ) return 0;
         temp = ( USART_RX0_Tail + 1 ) & USART_BUFFER_MASK;
         USART_RX0_Tail = temp;
         memcpy(data, &USART_RX0_Buffer[temp], 1);
         return 1;
         break;
      case SER_USB_USART:
         if( USART_RX1_Head == USART_RX1_Tail ) return 0;
         temp = ( USART_RX1_Tail + 1 ) & USART_BUFFER_MASK;
         USART_RX1_Tail = temp;
         memcpy(data, &USART_RX1_Buffer[temp], 1);
         return 1;
         break;
   }
   return -1;
}

// USART transmit
int8_t USART_TX(uint8_t usart, char *data)
{
   uint8_t temp;

   switch(usart)
   {
      case SER_485_USART:
         temp = ( USART_TX0_Head + 1 ) & USART_BUFFER_MASK;
         if( temp == USART_TX0_Tail ) return -1;

         USART_TX0_Buffer[temp] = *data;
         USART_TX0_Head = temp;

         SETBIT(UCSR0B, UDRIE0);
         break;
      case SER_USB_USART:
         temp = ( USART_TX1_Head + 1 ) & USART_BUFFER_MASK;
         if( temp == USART_TX1_Tail ) return -1;

         USART_TX1_Buffer[temp] = *data;
         USART_TX1_Head = temp;

         SETBIT(UCSR1B, UDRIE1);
         break;
   }
   return 1;
}

// USART transmit string
int8_t USART_TX_S(uint8_t usart, char *data, uint8_t length)
{
   uint8_t retval = 0;
   uint8_t i;

   for(i = 0; i < length; i++)
   {
      retval = USART_TX(usart, data+i);
      if( !retval ) break;
   }
   return retval;
}
#endif
