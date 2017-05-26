#ifndef PINS_C
#define PINS_C

#include <iostm8s003f3.h>          // Processor
#include <io_macros.h>
#include "globals.h"
#include "typedefs.h"
#include "led.h"
#include "pins.h"
#include <intrinsics.h>         // Interrupts


void pins_init (void);


/*-----------------------------------------------------------------------------------------*/
/*  @fn         pins_init ()
*   @param      none. 
*   @return     None
*   @brief      Configure pins, Control, direction
*------------------------------------------------------------------------------------------*/
void pins_init (void)
{
  /*  port  */
 
  
     /* BUTTON  */
   
    
    PD_DDR_bit.DDR2 = 0;           // Input
    PD_CR1_bit.C12 =  1;           // Floating input
    PD_CR2_bit.C22 =  1;           // External interrupt enabled
    
     /*   PWR_EN - Enable 3V Reg  */
   PC_DDR_bit.DDR6 = 1;           // Output
   PC_CR1_bit.C16 =  1;           // Enable pull-up
   PC_CR2_bit.C26 =  0;           // External interrupt Disabled
   
    /*   DR_EN - PWM PIN */
   PC_DDR_bit.DDR5 = 1;           // Output
   PC_CR1_bit.C15 =  1;           // Enable pull-up
   //PC_CR1_bit.C15 =  0;
   PC_CR2_bit.C25 =  1;           // High Speed
   
    /*   CURR_CTRL - Lamp States  */
   PC_DDR_bit.DDR4 = 1;           // Output
   PC_CR1_bit.C14 =  1;           // Enable pull-up
   PC_CR2_bit.C24 =  0;           // External interrupt Disabled
   
    /*   COMM_INT - Security pulses  */
   PC_DDR_bit.DDR3 = 0;           // Input
   PC_CR1_bit.C13 =  0;           // FLOATING INPUT
   PC_CR2_bit.C23 =  1;           // External interrupt disabled    || USE THE OTHER CONFIG MODE
   
  /* port D*/
   
        /*   USART TX */
   PD_DDR_bit.DDR5 = 1;           // Output
   PD_CR1_bit.C15 =  1;           // Enable pull-up
   PD_CR2_bit.C25 =  0;           // External interrupt Disabled
   
       /*   USART RX */
   PD_DDR_bit.DDR6 = 0;           // INput
   PD_CR1_bit.C16 =  1;           // Enable pull-up
   PD_CR2_bit.C26 =  0;           // External interrupt Enabled
  
   //   UNUSED //
   
     /* port A*/
     PA_DDR = 0xFF;                 // Output
     PA_CR1 = 0x01;                 // Pull up enabled
     PA_CR2 = 0x00;                 // Exte Int disabled
     /* port B*/
     PB_DDR = 0xFF;                 // Output
     PB_CR1 = 0x01;                 // Pull up enabled
     PB_CR2 = 0x00;                 // Exte Int disabled
}





#endif