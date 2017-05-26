#ifndef TIMERS_H
#define TIMERS_H

#include <iostm8s003f3.h>          // STM Processor type
#include <io_macros.h>
#include "globals.h"
#include "typedefs.h"
#include "led.h"
#include "pins.h"
#include "timers.h"
#include <intrinsics.h>         // Interrupts Inlines


extern void delay_ms(unsigned char millisecs);
extern void delay_s(unsigned char secs);
extern void delay_mins(unsigned char minutes);
extern void __delay_cycles(long word);



#endif












