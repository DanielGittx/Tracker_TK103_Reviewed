#ifndef MAIN_C
#define MAIN_C

//#include <iostm8l101f3.h>
#include <iostm8s003f3.h>          // STM Processor type
#include <io_macros.h>
#include "globals.h"
#include "typedefs.h"
#include "led.h"
#include "pins.h"
#include "timers.h"
#include "payload.h"
#include "uart.h"
#include "flash.h"
#include "gps.h"
#include <intrinsics.h>         // Interrupts Inlines
#include <string.h> // Memcpy
#include <stdio.h> // sprintf

#define PULSE_OFFSET 5           // We ignore this pulses as they are not genuine

void option_bytes_config (void);
void clock_config (void);
void CCR_interrupt_priority_config(void);                   // Leave default setting i.e same priority for all interrupts
void flags_reset (void);
void timer4config (void);
void timer2config(void);
void timer1config(void);
void init_timer2 (void);
void init_timer1 (void);
void security_check (void);
void set_duty_cycle (byte hi, byte lo);
//void gps_parse (byte* uart_Received_data);


//word AFR0_EN 0x4803
byte count = 0;
volatile word one_second_count = 0;
//volatile byte one_second_flag = 0;
volatile byte ten_seconds_count = 0;
//volatile byte ten_seconds_flag = 0;
byte security_pulses_count = 0;

word d = 0;
byte security_check_counter = 0;
byte uart2_RX_data[200]; ///< The receive buffer
byte src[200];
byte length = 0;



//byte uart2_RX_out = 0; // Start empty
//byte uart2_RX_parsing = 0; // Start empty




/*
#pragma location = 0x004803
__root const char AFR0 = 0x01;
*/

//__no_init volatile char AFR0;


#pragma vector = TIM1_OVR_UIF_vector
__interrupt void TIM1_UPD_OVF_IRQHandler(void)
{
         //PD_ODR_ODR5 ^= TRUE;
      one_second_count++;
      if ( one_second_count == 244)   //Every 1 SEC - 
          { 
            flag.noise_mask = 1;      // Mask button press noise on the COMM_INT Pin
            flag.one_second = 1;
            one_second_count = 0x0000;
          }
   TIM1_SR1_UIF = 0; 
  
}



#pragma vector = TIM2_OVR_UIF_vector
__interrupt void TIM2_UPD_OVF_IRQHandler(void)
{
 /*   
      one_second_count++;
      if ( one_second_count == 0xC350)   //Every 1 SEC - 0XC350 = 50000 Cycles
          { 
            //DR_EN  ^= TRUE; 
            //flag.one_second = 1;
            one_second_count = 0x0000;
          }
 */
    TIM2_SR1_UIF = 0;
  
}

#pragma vector = EXTI2_vector          //SECURITY PULSES          
__interrupt void EXTI_PORTC_IRQHandler(void)
{ 
  if (flag.noise_mask == 1)     // Avoid noise (pulses) generated by button press. Ensure we only record 1 pulse per second
    {
         security_pulses_count++;
         
      if (security_pulses_count == 5)
          {
              //PD_ODR_ODR5 ^= TRUE;
              //flag.security_pulse = 1;
              //security_pulses_count = 0;
          }
      flag.noise_mask = 0;
    }
}


#pragma vector = 8
__interrupt void EXTI_PORTD_IRQHandler(void)           
{  
  POWER_EN = TRUE;            // Switch on powre
  set_duty_cycle (0,0);
  
}

#pragma vector = UART1_R_RXNE_vector           // UART RECEIVE Handler
__interrupt void UART1_IRQHandler(void)        // UART1_DR Register (Data Register) must be read to avoid multiple concurrent interrupts!!!!     
{  
  byte n=0;
 
   
      if (UART1_SR_RXNE != 0)     // There is data in buffer
      {
             uart2_RX_data[uart2_RX_in] = UART1_DR;
                 uart2_RX_in++;         
                if (uart2_RX_in == 200 )                //Wrap buffer at 200
                    uart2_RX_in = 0;
                                   
              if (UART1_SR_RXNE == 0)                         //Transmission completed
              {           
                 // uart2_RX_in = 0;
                  security_pulses_count = 1;
               
              }
     
      }
      
    
}


void main (void)
{
  __disable_interrupt();
  
  //flash_control_register();       // ERROR! Address out of range issue
  //option_bytes_config();           
  
  /*
  int * AFR0_EN = (int*)0x004803;      // Alternate function to enable TMR2_CH1 for PC5 
  *AFR0_EN |= 0x0001;
  */
  
  clock_config();
  //CCR_interrupt_priority_config();
  flags_reset();
  init_timer2();
  init_timer1();
  timer2config();
  timer1config();
  InitialiseUART();
  pins_init();                    // Init MCU
  
  EXTI_CR1 = 0x90;               // Falling Edge for button and Rising Edge for security pulses

 //timer4config ();

 __enable_interrupt();
 

  //set_duty_cycle (0,0);
    
  
  gsm_AT_INIT();
  //length = sprintf (src, "RESET");
  //UARTPrintF( src, length);
  
 // SetDefaultValues();
  
  
  forever_loop 
  {
    
    __wait_for_interrupt();
     //state_sequencer();
     //state_machine();
     //strcpy
     //strlen
     //memcmp
     //atoi /itoa
     
    // gsm_gps_response_matching(2);
   
     ///////gsm_churn(); 
    
    
   if (flag.one_second == 1)  
   {  
     byte leng;
      //  leng = sprintf(src, print_enr_buff);
      //  UARTPrintF(src, leng);
     
      byte enrolling_msg = 0;
      enrolling_msg++;
      if (enrolling_msg == 1)
      {
       // leng = sprintf(src, "TESTING");
       // UARTPrintF(src,leng);
      }
    
        gsm_tick();
       // gsm_gps_response_matching(2);
      
  if ( security_pulses_count == 1)              //
     {
       //at_parse(20);
       
       gsm_sequencer();
       security_pulses_count =0;
     }
      
                    
     flag.one_second =0;           
    }
  
  
  }
  
}


void clock_config (void)
{
   CLK_ICKR = 0;               //  Reset the Internal Clock Register.
   CLK_ICKR_HSIEN = 1;         //  Enable the HSI.
   CLK_ECKR = 0;               //  Disable the external clock.
   while (CLK_ICKR_HSIRDY == 0);  //  Wait for the HSI to be ready for use.
   CLK_CKDIVR = 0;             //  Ensure the clocks are running at full speed. 16Mhz
   CLK_PCKENR1 = 0xff;            //  Enable all peripheral clocks.
   CLK_PCKENR2 = 0xff;
   CLK_CCOR = 0;               // Turn off CCO.
   CLK_HSITRIMR = 0;           //  Turn off any HSIU trimming.
   CLK_SWIMCCR = 0;             //  Set SWIM to run at clock / 2.
   CLK_SWR = 0xE1;                 //  Use HSI as the clock source.
   CLK_SWCR = 0;                //  Reset the clock switch control register.
   CLK_SWCR_SWEN = 1;           //  Enable switching.
   while (CLK_SWCR_SWBSY != 0);    //  Pause while the clock switch is busy.
   
  
}


/*
Enable write access to option bytes
Word program operation enabled
Block erase operation enabled
Fast block program operation disabled
Standard block programming operation enabled (automatically first erasing)

*/
void option_bytes_config (void)
{
  //byte option_byte [6] = {0x00,0x00,0x01,0x00,0x00,0x00};
  
//  FLASH_CR2 = 0x80;        
//  FLASH_NCR2 = 0x7F;
  
  FLASH_CR2_OPT = 1;            //write Access to option bytes enabled
  FLASH_NCR2_NOPT = 0;
  
  char *address = (char *) 0x4803;        //  Option byte base address.
  //for (byte i=0; i<3;i++)
  *address = (char) 0x01;            // Set AFR0 as 1 (Alternate function
  
 //*(unsigned char*)0x4800 = 0x00;  
 //*(unsigned char*)0x4801 = 0x00; 
 //*(unsigned char*)0x4802 = 0x00; 
 //*(unsigned char*)0x4803 = 0x01; 
 //*(unsigned char*)0x4804 = 0x00; 
 //*(unsigned char*)0x4805 = 0x00; 
  
}

void CCR_interrupt_priority_config (void)
{
  ITC_SPR3 = 0xff;         // SW priority disabled; allow external interrupt for Button
  ITC_SPR5 = 0xff;               
  
  
}
void timer4config (void)
{
   
    TIM4_CR1 = 0xff;
  //TIM4_CR1 = 00000101;

}
void init_timer2 (void)
{
  TIM2_CR1 = 0;
  TIM2_IER = 0;
  TIM2_SR2 = 0;
  TIM2_CCER1 = 0;
  TIM2_CCER2 = 0;
  TIM2_CCER1 = 0;
  TIM2_CCER2 = 0;
  TIM2_CCMR1 = 0;
  TIM2_CCMR2 = 0;
  TIM2_CCMR3 = 0;
  TIM2_CNTRH = 0;
  TIM2_CNTRL = 0;
  TIM2_PSCR = 0;
  TIM2_ARRH = 0;
  TIM2_ARRL = 0;
  TIM2_CCR1H = 0;
  TIM2_CCR1L = 0;
  TIM2_CCR2H = 0;
  TIM2_CCR2L = 0;
  TIM2_CCR3H = 0;
  TIM2_CCR3L = 0;
  TIM2_SR1 = 0;
}

void init_timer1 (void)
{
  TIM1_CR1 = 0;
  TIM1_IER = 0;
  TIM1_SR2 = 0;
  TIM1_CCER1 = 0;
  TIM1_CCER2 = 0;
  TIM1_CCER1 = 0;
  TIM1_CCER2 = 0;
  TIM1_CCMR1 = 0;
  TIM1_CCMR2 = 0;
  TIM1_CCMR3 = 0;
  TIM1_CNTRH = 0;
  TIM1_CNTRL = 0;
  TIM1_PSCRH = 0;
  TIM1_ARRH = 0;
  TIM1_ARRL = 0;
  TIM1_CCR1H = 0;
  TIM1_CCR1L = 0;
  TIM1_CCR2H = 0;
  TIM1_CCR2L = 0;
  TIM1_CCR3H = 0;
  TIM1_CCR3L = 0;
  TIM1_SR1 = 0;
}




/*
//  Setup Timer 2 to generate a 20 Hz interrupt based upon a 16 MHz timer.
void  timer2config(void)
{
 //TIM2_PSCR = 0x03;        // Prescalar = 8
 TIM2_PSCR = 0x02;        // Prescalar = 4
 //TIM2_ARRH = 0xc3;
 TIM2_ARRL = 0x50;        // Low byte = 50000
  TIM2_IER_UIE = 1;        // Enable update Interrupts
 TIM2_CR1_CEN = 1;        // Enable timer
}
*/

void  timer2config(void)
{
   
    TIM2_PSCR = 0x00;             //  Prescaler = 1.  
    
    TIM2_ARRH = 0x15;      //  High byte of.     - - PERIOD--FREQUENCY
    TIM2_ARRL = 0xff;       //  Low byte of .
    
    
    /*
    TIM2_ARRH = 0x0a;      //  High byte of.     - - PERIOD--FREQUENCY
    TIM2_ARRL = 0x00;       //  Low byte of .
    */
    
    //TIM2_CCR1H = 0x03;      //  High byte    - - DUTY CYCLE
    //TIM2_CCR1L = 0x30;      //  Low byte    
    TIM2_CCER1_CC1P = 0;    //  Active HIGH
    TIM2_CCER1_CC1E = 1;    //  Enable compare mode for channel 1
    TIM2_CCMR1_OC1M = 6;    //  PWM Mode 1 - active if counter < CCR1, inactive otherwise. (0b0110)
    TIM2_IER_UIE = 1;       //  Enable the update interrupts.
    TIM2_CR1_CEN = 1;       //  Finally enable the timer.

}

void  timer1config(void)
{
     TIM1_PSCRH = 0x00;        // Prescalar = 1
     TIM1_ARRH = 0xff;
     TIM1_ARRL = 0xff;        // Low byte
     TIM1_IER_UIE = 1;        // Enable update Interrupts
     TIM1_CR1_CEN = 1;        // Enable timer
  
}



void set_duty_cycle (byte hi, byte lo)
{
    TIM2_CCR1H = (hi&hi);      //  High byte    - - DUTY CYCLE
    TIM2_CCR1L = (lo&lo);      //  Low byte  
}


void flags_reset(void)
{
  TIM1_SR1_UIF = 0;
  flag.button_press = 0;
  flag.ten_secs_elapse = 0;
  flag.one_second =0 ;
  flag.security_pulse = 0; 
  flag.noise_mask = 0;        
  
}

void clear_RX_buffer(void) 
{

  for (word i=0; i< uart2_RX_in; i++)
  {
    //uart2_RX_data[i] = NULL;
  }
}



#endif