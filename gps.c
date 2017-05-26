#ifndef GPS_C
#define GPS_C


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




void at_parse (byte t);
void gsm_gps_response_matching (void);
void gsm_gps_turnoff( void );
void gsm_gps_turnon( void );
void gsm_tick (void);
void TK103_protocol (void);




byte ip_initial_mode_retry = 0;
byte global_power_mode_state =0;





void at_parse (byte t)
{
  byte parsed = 0, tail = 0, n,p,q,r,s,u,v,w,x,y,z;
  byte NMEA_ID [] = "$GPGGA,";      //First 7 bytes of NMEA Sentence.
  byte buff[200];
    //byte gps_info;
 
    if (memcmp (uart2_RX_data,"OK", 2) == 0)
    {
          response_flags = RESP_OK; // Await this response
      
       //UARTPrintF (uart2_RX_data, strlen(uart2_RX_data));
       Uart_buffer_reset(2);
    }
    else if (memcmp (uart2_RX_data,"ERROR",5)== 0)
    {
       //UARTPrintF (uart2_RX_data, strlen(uart2_RX_data));
       Uart_buffer_reset(2);
    }
    else if (memcmp (uart2_RX_data,"+COPS: ",7)== 0)
    {
             UARTPrintF (uart2_RX_data, strlen(uart2_RX_data));
       Uart_buffer_reset(2);
    }
    else if (memcmp (uart2_RX_data,"+CREG: ",7)== 0)
    {
             UARTPrintF (uart2_RX_data, strlen(uart2_RX_data));
       Uart_buffer_reset(2);
    }
    else if (memcmp (uart2_RX_data,"+CGATT: ",8)== 0)
    {
             UARTPrintF (uart2_RX_data, strlen(uart2_RX_data));
       Uart_buffer_reset(2);
    }
    else if (memcmp (uart2_RX_data,"STATE: IP INITIAL",17)== 0)
    {
             UARTPrintF (uart2_RX_data, strlen(uart2_RX_data));
       Uart_buffer_reset(2);
    }
    else if (memcmp (uart2_RX_data,"CONNECT OK",10)== 0)
    {
             UARTPrintF (uart2_RX_data, strlen(uart2_RX_data));
       Uart_buffer_reset(2);
    }
    else if (memcmp (uart2_RX_data,"CONNECT FAIL",12)== 0)
    {
             UARTPrintF (uart2_RX_data, strlen(uart2_RX_data));
       Uart_buffer_reset(2);
    }
    else if (memcmp (uart2_RX_data,"+CME ERROR",10)== 0)
    {
             UARTPrintF (uart2_RX_data, strlen(uart2_RX_data));
       Uart_buffer_reset(2);
    }
    else if (memcmp (uart2_RX_data,"ALREADY CONNECT",15)== 0)
    {
             UARTPrintF (uart2_RX_data, strlen(uart2_RX_data));
       Uart_buffer_reset(2);
    }
    else if (memcmp (uart2_RX_data,"SEND OK",7)== 0)
    {
             UARTPrintF (uart2_RX_data, strlen(uart2_RX_data));
       Uart_buffer_reset(2);
    }
    else if (memcmp (uart2_RX_data,"CLOSE OK",7)== 0)
    {
             UARTPrintF (uart2_RX_data, strlen(uart2_RX_data));
       Uart_buffer_reset(2);
    }
    else if (memcmp (uart2_RX_data,"SHUT OK",7)== 0)
    {
      response_flags = RESP_SHUT_OK;
      //UARTPrintF (uart2_RX_data, strlen(uart2_RX_data));
      Uart_buffer_reset(2);
    }    
    else if (memcmp (uart2_RX_data,"+SGACT: ",8)== 0)          // for IP Address processing ????????? confirm response format 
    {
             UARTPrintF (uart2_RX_data, strlen(uart2_RX_data));
       Uart_buffer_reset(2);
    }
    else if (memcmp (uart2_RX_data,"+CMT: ",6)== 0)            // for SMS Processing
    {
             UARTPrintF (uart2_RX_data, strlen(uart2_RX_data));
       Uart_buffer_reset(2);
    }
    else if (memcmp (uart2_RX_data,NMEA_ID, 7) == 0)           //This is a good packet we can go ahead and parse it
    {
      
         // Uart_buffer_reset(2);
         // UARTPrintF ("RIGHT_ID\n", 9);
         
          //memset(uart2_RX_data,'0',strlen(uart2_RX_data));
          
          //Time 
          
          
          for (n=0; n<6;n++)
          {
            length = sprintf (src,"%c",uart2_RX_data[7+n]);
            //UARTPrintF(src, length);
          }
                  
          // Latitude 
          
          for (p=0; p<12;p++)
          {
            length = sprintf (src,"%d",uart2_RX_data[17+p]);
           // UARTPrintF(src, length);
          }
          
           //Latitude North or South     
          
          switch (uart2_RX_data[30])             
          {
          
          case 'N':
            length = sprintf (src,"%d",uart2_RX_data[30]);
            //UARTPrintF(src, length);
            break;
          case 'S':
            length = sprintf (src,"%d",uart2_RX_data[30]);
            //UARTPrintF(src, length);
            break;
          //default:
            
          }
          
          // Longitude
          for (q=0; q<13;q++)             //12 bytes
          {
            length = sprintf (src,"%d",uart2_RX_data[32+q]);
            //UARTPrintF(src, length);
          }
          
          // Longidude orientation (East or West) and Quality Indication
         switch (uart2_RX_data[46])             
          {
          case 'E':
            length = sprintf (src,"%d",uart2_RX_data[46]);
            //UARTPrintF(src, length);
            break;
          case 'W':
            length = sprintf (src,"%d",uart2_RX_data[46]);
            //UARTPrintF(src, length);
            break;
            
         
          //default:
            
          }
         
          switch (uart2_RX_data[48])
          {
            
          case '1': //Uncorrected Coordinate
           // length = sprintf (src,"%d",uart2_RX_data[48]);
           // UARTPrintF(src, length);
              break;
          case '2':    //Diffrentially correct coordinate
           // length = sprintf (src,"%d",uart2_RX_data[48]);
           // UARTPrintF(src, length);
              break;
          case '3':    // :)
           // length = sprintf (src,"%d",uart2_RX_data[48]);
           // UARTPrintF(src, length);
              break;
          case '4':   // RTK Fix coordinate
           // length = sprintf (src,"%d",uart2_RX_data[48]);
           // UARTPrintF(src, length);
              break;
          case '5':   // RTK Float (decimeter precision)
            //length = sprintf (src,"%d",uart2_RX_data[48]);
            //UARTPrintF(src, length);
              break;
            
          }
          
          // Number of satelites (To be done!)
          for (r=0; r<2; r++)
          {
          // length = sprintf (src,"%d",uart2_RX_data[50+r]);
          // UARTPrintF(src, length);
          }
          
          //Horizontal Dilution of precision (HDOP)
          for (s=0; s<4; s++)
          {
          // length = sprintf (src,"%d",uart2_RX_data[53+s]);
          // UARTPrintF(src, length);
          }
          
          // Altitude of the antenna
                   
          for (u=0; u<7; u++)
          {
          // length = sprintf (src,"%d",uart2_RX_data[58+u]);
          // UARTPrintF(src, length);
          }
          
          //This info may not be very necessary;no need to test it (TBD)
         /* 
          for (v=0; v<24; v++)
          {
             length = sprintf (src,"%d",uart2_RX_data[65+v]);
             UARTPrintF(src, length);
          }
          */
        
    Uart_buffer_reset(2);        
    response_flags = RESP_NMEA_DATA;         
    }
    else if ( ( uart2_RX_data[0] >= 0x30 ) && ( uart2_RX_data[0] <= 0x39 ) )    // IMEI message should start with a number.       
    {
      if (strlen (uart2_RX_data) != 14)                      //IMEI is 14-bytes; this number is surely not IMEI!
      {
          Uart_buffer_reset(2);
      }
      else      // IMEI trapped ; WE NEED IMEI TO BUILD ENROLLING MESSAGE
      {
       
        
        if (strlen(print_enr_buff) != 25)                  // We are not sure what we have in enrolling message buffer    ;(
            {
                print_enr_buff_reset();                         // So Reset stuff! 
                device_enrolling_message(uart2_RX_data);        // build up enrolling message afresh
            }
         else
            {
                length = sprintf(src, print_enr_buff);         //we already have full message 
                UARTPrintF(src,length);              
            }
          Uart_buffer_reset(2);
          response_flags = RESP_IMEI;
      }
      
    }
   else
    {
          Uart_buffer_reset(2);                  // TRASH AREA, JUST DISCARD IT :(
    }   
    
}
void gsm_tick (void)
{
  
  if (gsm_timeout > 0)
  {
    gsm_timeout--;             //Keep running
  }
  else
  {
    gsm_gps_turnoff(); //MODEM TURN OFF (SW/HW)
    
  }
 
    if ( gsm_tick_send > 0 )
    {
        gsm_tick_send--;
        if ( gsm_tick_send == 0 )
        {   
          switch (gsm_state)
    	    {
            case STATE_INIT:
              gsm_AT_INIT();
              break;
            case STATE_GPS_POWER_ON:
                if (global_power_mode_state == ON)
                 {
                    gps_AT_SET_POWER_MODE (ON);
                 }
                else 
                {
                  gps_AT_SET_POWER_MODE (OFF);
                }
              break;
            case STATE_GPS_AUTONOMY_MODE:
              gps_AT_SET_AUTONOMY_MODE ();
              break;
            case STATE_GPS_FULL_FUNCTIONALITY:
              gps_AT_SET_FULL_FUNCTIONALITY();
              break;
            case STATE_GPS_LOCATION_INFOR:
              gps_AT_GET_LOCATION_INFO ();
              break;
            case STATE_CMGF:
              gsm_AT_SETCMGF();
              break;
            case STATE_CNMI:
              gsm_AT_SETCNMI();
              break;
            case STATE_GPRS_SHUTDOWN:
              gsm_AT_GPRS_SHUTDOWN ();  
              break;
            case STATE_STARTUP_SINGLE_IP_CONNECTION:
              gsm_AT_STARTUP_SINGLE_IP_CONNECTION ();
              break;
            case STATE_IP_INITIAL_MODE:
              gsm_AT_IP_INITIAL_MODE ();
              break;
            case STATE_SETCGDCONT:
              gsm_AT_SETCGDCONT();
              break;
            case STATE_SETCGATT:
              gsm_AT_CGATT();
              break;
            case STATE_GETIMEI:
              gsm_AT_GET_IMEI();
              break;
            case STATE_SETCGACT:
              gsm_AT_CGACT();
              break;
            case STATE_CONNECT_TO_SERVER:
              gsm_AT_CONNECT_TO_SERVER();
              break;
            case STATE_SETAPN_START_TASK:
              gsm_AT_SETAPN_START_TASK();
              break;
            case STATE_BRINGUP_WIRELESS_CONNECTION_GPRS:
              gsm_AT_BRINGUP_WIRELESS_CONNECTION_GPRS();
              break;
            case STATE_GET_IP:
              gsm_AT_GET_IP();
              break;
            case STATE_SEND_TCP_PACKET:
              gsm_AT_SEND_TCP_PACKET();
              break;
            case STATE_CLOSE_TCP_CONNECTION:
              gsm_AT_CLOSE_TCP_CONNECTION();
              break;
            default:
                
            }
            
        }
        else           // Timed out
        {
              //gsm_gps_turnoff( );         // HW turn off
        }
    }
  /*
       length = sprintf (src,"gsm_state: %d",gsm_state);
       UARTPrintF(src, length);
  */
}

void gsm_gps_response_matching (void)
{
  byte response_state = 0;
   
  switch (gsm_state)
  {
    
  case STATE_INIT:
    if (response_flags == RESP_OK)
       gps_AT_SET_POWER_MODE(ON);
       break;
       
  case STATE_GPS_POWER_ON:
    if (response_flags == RESP_OK)
     gps_AT_SET_AUTONOMY_MODE();
    break;
        
  case STATE_GPS_AUTONOMY_MODE:
    if (response_flags == RESP_OK)
      gps_AT_SET_FULL_FUNCTIONALITY();
    break;
    
  case STATE_GPS_FULL_FUNCTIONALITY:
    if (response_flags == RESP_OK)
      gsm_AT_GPRS_SHUTDOWN ();
    break;
    
  case STATE_GPRS_SHUTDOWN:
    if (response_flags == RESP_SHUT_OK)
      gsm_AT_STARTUP_SINGLE_IP_CONNECTION ();
       
    break;
    
  case STATE_STARTUP_SINGLE_IP_CONNECTION:
    if (response_flags == RESP_OK)
         gsm_AT_IP_INITIAL_MODE();
         //ip_initial_mode_retry++;             //Incremented inside funtion
      break;
      
  case STATE_IP_INITIAL_MODE:
    if (response_flags == RESP_OK)
      gsm_AT_SETCGDCONT();
    break;
     
  case STATE_SETCGDCONT:
    if (response_flags == RESP_OK)
      gsm_AT_CGACT();
    break;
    
  case STATE_SETCGACT:
    if (response_flags == RESP_OK)
      gsm_AT_CGATT();
    break;
      
  case STATE_SETCGATT:
    if (response_flags == RESP_OK)
      gsm_AT_IP_INITIAL_MODE ();
     if(ip_initial_mode_retry == 2)
     {
       gsm_AT_SETAPN_START_TASK();
     }
    break;
   
    /*
  case STATE_IP_INITIAL_MODE:
      if (response_state == RESP_OK)
      gsm_AT_SETAPN_START_TASK();
    break;
    
    */
    
  case STATE_SETAPN_START_TASK:
     if (response_flags == RESP_OK)
       gsm_AT_IP_INITIAL_MODE ();
       if(ip_initial_mode_retry == 3)
       {
          gsm_AT_BRINGUP_WIRELESS_CONNECTION_GPRS ();
          ip_initial_mode_retry = 0;
       }
     break;
       
  case STATE_BRINGUP_WIRELESS_CONNECTION_GPRS:
    if (response_flags == RESP_OK)
      gsm_AT_GET_IP();
    break;
    
  case STATE_GET_IP:
     if (response_flags == RESP_OK)
       gps_AT_GET_LOCATION_INFO ();
     break;
     
  case STATE_GPS_LOCATION_INFOR:
    if (response_flags == RESP_NMEA_DATA)
      gsm_AT_GET_IMEI();
    break;
    
  case STATE_GETIMEI:
    if (response_flags == RESP_IMEI)
      gsm_AT_CONNECT_TO_SERVER();
    break;
    
  case STATE_CONNECT_TO_SERVER:
    if (response_flags == RESP_CONNECT_OK | RESP_CONNECT_FAIL)
      gsm_AT_SEND_TCP_PACKET ();
    break;
    
  case STATE_SEND_TCP_PACKET:
    if (response_flags == RESP_OK)
      gsm_AT_CLOSE_TCP_CONNECTION();
    break;
    
  case STATE_CLOSE_TCP_CONNECTION:
    if (response_flags == RESP_OK)
        gps_AT_SET_POWER_MODE(OFF);
        // MODEM HW IS STILL POWERED!
    break;
    default:   
    
  }
  
}


void gsm_gps_turnon( void )         // HW turn on
{
    gsm_state = STATE_INIT;
}

void gsm_gps_turnoff( void )         // HW turn off
{
  
  
}

void TK103_protocol (void)
{
  
  
  
  
}



#endif