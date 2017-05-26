#ifndef GLOBALS_C
#define GLOBALS_C

//Global Defines

#define forever_loop      while(1)
#define POWER_EN          PC_ODR_ODR6        // Switch on 3V Regulator
#define DR_EN             PC_ODR_ODR5        // Switch on/off LEDS
#define CURR_CTRL         PD_ODR_ODR4         



// Global Functions



// Global variables


/*

//integers
//64bit //unsigned long long 0 to 18,446,744,073,709,551,615
//32bit //unsigned long int 0 to 4,294,967,295
//16bit //unsigned int 0 to 65535
//8bit  //unsigned char 0 to 255

*/

/*
----------------------------
PWM CHEAT SHEET
----------------------------

TIMx_ARRH/L  -- Adjust PWM Frequency
TIMx_CCPR/L  -- Adjust Duty Cycle

DUTY_CYCLE =  (TIMx_CCPR/L divided by TIMx_ARRH/L ) * 100

HIGH BYTE - Big Change in value
LOW BYTE - Small/Gradual Change in value

PWM Freq - 2.830kHz
Duty_Cycle:- Low Setting  - 13%
           - High Setting - 20%

*/
/*
    if ( memcmppgm2ram( &buffer[DATA], (const rom far void *)"TEST", 4 ) == 0 )
    

  //  byte *ch = message;
    //byte i;
   
    while (*ch)
    {
        UART1_DR = (unsigned char) *ch;     //  Put the next character into the data transmission register.
        while (UART1_SR_TXE == 0);          //  Wait for transmission to complete.
        ch++;                               //  Grab the next character.
    }
    */





#endif
