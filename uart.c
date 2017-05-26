#ifndef UART_C
#define UART_C

#include <iostm8s003f3.h>          // STM Processor type
#include <io_macros.h>
#include <string.h> // Memcpy
#include <stdio.h> // sprintf
#include "typedefs.h"
#include "uart.h"


#define UART_LF		   0x0A
#define UART_CR		   0x0D
#define UART_ESCAPE	   0x7D
#define UART_TILDA	   0x7E



void InitialiseUART(void);
void UARTPrintF(byte *message, byte len);
void UART_RECEIVE_Churn (void);




//byte uart2_RX_data[UART2_RX_BUFF_SIZE]= {0}; ///< The receive buffer
//byte uart2_RX_data[4]= {0,0,0,0}; ///< The receive buffer
byte uart2_RX_in = 0; /// Number of bytes written into
byte uart2_RX_out = 0; 
byte uart2_RX_parsing = 0; ////What weve parsed
void Uart_buffer_reset (byte t);

//  Setup the UART to run at 115200 baud, no parity, one stop bit, 8 data bits.
//
//  Important: This relies upon the systemk clock being set to run at 16 MHz.

/////////////////////////////////////////////////////////////////////////////////////

 void InitialiseUART()
{       
    unsigned char tmp = UART1_SR;
    tmp = UART1_DR;
    //  Reset the UART registers to the reset values.
    UART1_CR1 = 0;
    UART1_CR2 = 0;
    UART1_CR4 = 0;
    UART1_CR3 = 0;
    UART1_CR5 = 0;
    UART1_GTR = 0;
    UART1_PSCR = 0;
    //  Set up the port to 14400,n,8,2.
    UART1_CR1_M    = 0;         //  8 Data bits.
    UART1_CR1_PCEN = 0;         //  Disable parity.
    UART1_CR3      = 0x20;      //  2 stop bits     
    UART1_BRR2     = 0x07;      //  Set the baud rate registers to 14400
    UART1_BRR1     = 0x45;      //  based upon a 16 MHz system clock.
    //  Disable the transmitter and receiver.
    UART1_CR2_TEN   = 0;        //  Disable transmit.
    UART1_CR2_REN   = 0;        //  Disable receive.
    //  Set the clock polarity, clock phase and last bit clock pulse.
    UART1_CR3_CPOL  = 0;
    UART1_CR3_CPHA  = 0;
    UART1_CR3_LBCL  = 0;
    //  Set the Receive Interrupt RM0016 p358,362
    UART1_CR2_TIEN  = 0;     // Transmitter interrupt enable
    UART1_CR2_TCIEN = 0;     // Transmission complete interrupt enable
    UART1_CR2_RIEN  = 1;     //  Receiver interrupt enable
    UART1_CR2_ILIEN = 0;     //  IDLE Line interrupt enable

    //  Turn on the UART transmit, receive and the UART clock.
    UART1_CR2_TEN    = 1;
    UART1_CR2_REN    = 1;
    UART1_CR1_PIEN   = 0;
    UART1_CR4_LBDIEN = 0;
    }

/////////////////////////////////////////////////////////////////////////////////////////


//  Send the message in the string to UART1.
void UARTPrintF(byte *message, byte len)
{  
    byte i;
    for ( i = 0 ; i < len ; i++ ) // For all the data
    {
        while ( UART1_SR_TXE == 0 ); // Wait for the shift register to be clear
        UART1_DR = message[i];
    }
    //while ( UART1_SR_TXE == 0 ); // Wait for the shift register to be clear
    //UART1_DR = '\n';             // Trailing Line Feed character [ \n ]
    
}

void UART_RECEIVE_Churn (void)
{
  
  
}

void Uart_buffer_reset (byte t)
{
  byte i;
  uart2_RX_in = 0;
  for (i=0; i<200; i++)
  {
    uart2_RX_data[i] = 0;
  }
}

#endif