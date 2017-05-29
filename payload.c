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
void device_enrolling_message(byte* uart2_RX_data_ptr );
void continous_feedback_message(byte* _imei);
void handshaking_message(byte t);

//Messages from server (server ACKS)
void server_response (void);        // Server response after receiving ENROLLING MESSAGE from device
void handshaking_message_answer (void);

byte print_enr_buff[50];

void device_enrolling_message(byte* uart2_RX_data_ptr )
{
  byte leng,i;
  byte _enr_buff1[15] = "##,imei:";
  byte _enr_buff2[10] = ",A;";
  
  strcat(print_enr_buff,_enr_buff1);
  strcat(print_enr_buff,uart2_RX_data);
  strcat(print_enr_buff,_enr_buff2);
  
  leng = sprintf(src, print_enr_buff);
  UARTPrintF(src,leng);
}






#endif