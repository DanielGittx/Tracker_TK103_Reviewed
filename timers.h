#ifndef TIMERS_C
#define TIMERS_C

#include "timers.h"


#define F_CPU 11059200
#define delay_us(usecs) __delay_cycles((F_CPU/1000000)  *(usecs))

void delay_ms(unsigned char millisecs);
void delay_s(unsigned char secs);
void delay_mins(unsigned char minutes);
void __delay_cycles(long word);


void delay_ms(unsigned char millisecs)
{
    while(millisecs--)
    {
      delay_us(1000);
    }
}
void delay_s(unsigned char secs)
{
    while(secs--)
    {
      __delay_cycles(F_CPU);
    }
}
void delay_mins(unsigned char minutes)
{
    while(minutes--)
    {
      __delay_cycles(60*F_CPU);
    }
}







#endif
