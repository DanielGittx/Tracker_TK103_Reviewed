#ifndef LED_C
#define LED_C
   

#include <iostm8s003f3.h>          // Processor
#include <io_macros.h>
#include "globals.h"
#include "typedefs.h"
#include "led.h"
#include "pins.h"
#include <intrinsics.h>         // Interrupts
  
void light_off(void);
void light_low(void);
void light_med(void);
void light_high(void);
//void state_machine(byte light_setting_state );
void state_machine(void );
void state_sequencer(void);
void power_on_init(void);


/*-----------------------------------------------------------------------------------------*/
/*  @fn         state_machine()
*   @param      none. 
*   @return     None
*   @brief      Implements state functions
*------------------------------------------------------------------------------------------*/

//void state_machine(byte light_setting_state )
void state_machine(void )
{
  //Light_Setting = light_setting_state;
  
	switch (Light_Setting)
	{
		case LIGHT_OFF:

             light_off();
  			break;
		case LIGHT_LOW:
                           power_on_init();
			  //light_low();
                        
			break;
		case LIGHT_MED:

			light_med();

			break;
            
                case LIGHT_HIGH:

			light_high();
                        
               		break;
		default:
                      light_off();
      
	}
}

/*-----------------------------------------------------------------------------------------*/
/*  @fn         state_sequencer()
*   @param      none. 
*   @return     None
*   @brief      Move/sequence along states
*------------------------------------------------------------------------------------------*/

void state_sequencer(void)
{      
	switch (Light_Setting)
	{

		case LIGHT_OFF:
            
     		Light_Setting = LIGHT_LOW;	  //
            
			break;
		case LIGHT_LOW:

			Light_Setting = LIGHT_MED;    // 

			break;
		case LIGHT_MED:

			Light_Setting = LIGHT_HIGH;    

			break;
               case LIGHT_HIGH:

			Light_Setting = LIGHT_OFF;    

			break;

     	    default:
  
			Light_Setting = LIGHT_LOW;
		}
}

/*-----------------------------------------------------------------------------------------*/
/*  @fn         light_off()
*   @param      none. 
*   @return     None
*   @brief      Put off LEDs
*               Switch off POWER-3v Reg,PWM Off
*               Put system to sleep  
*------------------------------------------------------------------------------------------*/

void light_off(void )                       // 0 mA
{
   DR_EN  = FALSE;  
   POWER_EN = FALSE;
}

/*-----------------------------------------------------------------------------------------*/
/*  @fn         power_on_init()
*   @param      none. 
*   @return     None
*   @brief      Switch on POWER-3v Reg,PWM    
*               Put on LEDS, state Light low
*                
*                 
*------------------------------------------------------------------------------------------*/

void power_on_init()
{
   DR_EN  = TRUE;  
   POWER_EN = TRUE;
   light_low ();
}


/*-----------------------------------------------------------------------------------------*/
/*  @fn         light_low()
*   @param      none. 
*   @return     None
*   @brief      Set Duty cycle for low state
*               
*                 
*------------------------------------------------------------------------------------------*/
void light_low(void )
{
      //DR_EN  = TRUE;
        PC_ODR_ODR4 = TRUE; 
         set_duty_cycle (0x03, 0xc0);         // 2.830kHz withOUT R18 - 85mA
      //  set_duty_cycle (0x02, 0xe0);         // 2.830kHz with R18
     //  set_duty_cycle (0x03, 0x30);       //2.4kHz
     //set_duty_cycle (0x01, 0xff);       //12.43kHz No hissing but dutycycleun adjustable!!!
             
}

/*-----------------------------------------------------------------------------------------*/
/*  @fn         light_med()
*   @param      none. 
*   @return     None
*   @brief      Set Duty cycle for Med state
*               
*                 
*------------------------------------------------------------------------------------------*/
void light_med(void)
{ 
    //DR_EN  = TRUE;
      //PC_ODR_ODR4 = TRUE; 
      //set_duty_cycle (0xA0, 0x00);   //2.830kHz
     // set_duty_cycle (0x04, 0xff);  // 2kHz
    //Light_Setting = LIGHT_HIGH;
}

/*-----------------------------------------------------------------------------------------*/
/*  @fn         light_hig()
*   @param      none. 
*   @return     None
*   @brief      Set Duty cycle for High state
*               
*                 
*------------------------------------------------------------------------------------------*/

void light_high(void )				
{
    //DR_EN  = TRUE;
       PC_ODR_ODR4 = TRUE; 
       set_duty_cycle (0x06, 0x07);   //2.830kHz WITHOUT R18 - 185mA
      //set_duty_cycle (0x04, 0xff);           /// 2.45kHz
    //Light_Setting = LIGHT_HIGH;
}


#endif
