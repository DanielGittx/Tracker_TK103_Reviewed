#ifndef PAYLOAD_C
#define PAYLOAD_C

#include "gps.h"
#include "uart.h"
#include <iostm8s003f3.h>          // STM Processor type
#include <io_macros.h>
#include "globals.h"
#include "typedefs.h"
#include "led.h"
#include "pins.h"
//#include "timers.h"
#include "uart.h"
#include "flash.h"
#include "gps.h"
#include "payload.h"
#include <intrinsics.h>         // Interrupts Inlines
#include <string.h> // Memcpy
#include <stdio.h> // sprintf

// Messages to server //
byte* device_enrolling_message(void);
void continous_feedback_message(byte t);
void handshaking_message(byte t);

//Messages from server (server ACKS)
void server_response (void);        // Server response after receiving ENROLLING MESSAGE from device
void handshaking_message_answer (void);



byte* device_enrolling_message(void)
{
  byte i;
  byte enrolling_msg[] = "##,imei:";
    
  for (i=0; i<14; i++)
  {
    enrolling_msg[9+i] = uart2_RX_data[i];                // Store IMEI
  }
  
  return enrolling_msg;
}






#endif