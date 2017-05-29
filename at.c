#ifndef AT_C
#define AT_C

#include <iostm8s003f3.h>          // STM Processor type
#include <io_macros.h>
#include "globals.h"
#include "typedefs.h"
#include "led.h"
#include "pins.h"
#include "uart.h"
#include "flash.h"
#include "gps.h"
#include "at.h"
#include <intrinsics.h>         // Interrupts Inlines
#include <string.h> // Memcpy
#include <stdio.h> // sprintf



void gps_AT_SET_POWER_MODE (byte power_mode_state);   // default - power down,
void gps_AT_SET_AUTONOMY_MODE (void); // GPS module needs to be reset before it can start acquiring location information- Issue this after GPS pwr ON
void gps_AT_SET_FULL_FUNCTIONALITY (void); //Full GSM functionality - issue after setting autonomy mode
void gps_AT_GET_LOCATION_INFO (void);      // Longitued,latitude, time


void gsm_AT_SETCMGF(void);       // Operate in TEXT mode while reading and sending SMS - Issue this first the power on GPS
void gsm_AT_SETCNMI(void);
void gsm_AT_READ_SMS (void); 
void gsm_AT_GPRS_SHUTDOWN (void); //Shutdown GPRS beffore attempting to connect (optional)
void gsm_AT_STARTUP_SINGLE_IP_CONNECTION (void); // 
void gsm_AT_IP_INITIAL_MODE (void); //module should reply with STATE: IP INITIAL, if not then you need to restart with the whole process again. 
void gsm_AT_SETCGDCONT( void );      //Set GPRS PDP context - SAFARICOM APN (find it out????)
void gsm_AT_CGACT( void );            // Activate PDP context 
void gsm_AT_CGATT (void );        //Attach to the GPRS Service
void gsm_AT_GET_IMEI (void);      // Get Imei
void gsm_AT_CONNECT_TO_SERVER (void); // TCP Connection to server (traccar IP and PORT) ,,,, 
void gsm_AT_SETAPN_START_TASK(void);
void gsm_AT_BRINGUP_WIRELESS_CONNECTION_GPRS(void);
void gsm_AT_GET_IP(void);
void gsm_AT_SEND_TCP_PACKET (void);
void gsm_AT_CLOSE_TCP_CONNECTION(void);



byte gsm_IMEI[20]; ///< The IMEI of the modem
byte gsm_ICCID[25]; ///< The ICCID of the SIM
byte gsm_IP[16];
//extern byte gsm_retry;
byte retry_count; 
byte sms_delay;
byte gsm_FailureCount; ///< The current GSM transmission failure count

byte gsm_state;
byte gps_state;
byte gsm_tick_send;
byte gsm_timeout = 0;
byte response_flags = RESP_NONE;



/***************************************************************//**
* @fn gsm_AT_INIT()
* @brief Send the AT Command to set factory defaults
*******************************************************************/
void gsm_AT_INIT( void ) 
{
    gsm_state = STATE_INIT; 
    memcpy(src, "AT&F0\r\n", 7); //Factory Defaults
    UARTPrintF(src, 7);  //post to Modem
    response_flags = RESP_OK;
    gsm_tick_send = AT_SEND_TIMEOUT; // Start a Modem not responding timeout
    // gsm_tick_send = 0;
    
}

/***************************************************************//**
* @fn gps_AT_SET_POWER_MODE (byte power_mode_state)
* @brief Send the AT Command to select GPS power  mode
*******************************************************************/
void gps_AT_SET_POWER_MODE( byte power_mode_state ) 
{
  if (power_mode_state == ON)
  {
    global_power_mode_state = power_mode_state;
    gsm_state = STATE_GPS_POWER_ON; 
    memcpy(src, "AT+CGPSPWR=1\r\n", 14); //Factory Defaults
    UARTPrintF(src, 14);  //post to Modem
    response_flags = RESP_OK;
    gsm_tick_send = AT_SEND_TIMEOUT; // Start a Modem not responding timeout
    gsm_timeout = GSM_TIMEOUT; //
  }
  else if (power_mode_state == OFF)
  {
    global_power_mode_state = power_mode_state;
    gsm_state = STATE_GPS_POWER_OFF; 
    memcpy(src, "AT+CGPSPWR=0\r\n", 14); //Factory Defaults
    UARTPrintF(src, 14);  //post to Modem
    gsm_timeout = GSM_TIMEOUT; // 
    gsm_tick_send = AT_SEND_TIMEOUT;
    
  }
    
}
void gps_AT_SET_AUTONOMY_MODE (void)
{
    gsm_state = STATE_GPS_AUTONOMY_MODE; 
    memcpy(src, "AT+CGPSRST=1\r\n", 14); //GPS Reset
    UARTPrintF(src, 14);  //post to Modem
    response_flags = RESP_OK;
    gsm_tick_send = AT_SEND_TIMEOUT; // Start a Modem not responding timeout
    gsm_timeout = GSM_TIMEOUT; //
  
}

void gps_AT_SET_FULL_FUNCTIONALITY (void)
{
    gsm_state = STATE_GPS_FULL_FUNCTIONALITY; 
    memcpy(src, "AT+CFUN=1\r\n", 11); //GPS Reset
    UARTPrintF(src, 11);  //post to Modem
    response_flags = RESP_OK;
    gsm_tick_send = AT_SEND_TIMEOUT; // Start a Modem not responding timeout
    gsm_timeout = GSM_TIMEOUT; //
  
}

void gps_AT_GET_LOCATION_INFO (void)
{
    gsm_state = STATE_GPS_LOCATION_INFOR; 
    memcpy(src, "AT+CGPSINF=32\r\n", 15); //GPS Reset
    UARTPrintF(src, 15);  //post to Modem
    gsm_tick_send = AT_SEND_TIMEOUT; // Start a Modem not responding timeout
    gsm_timeout = GSM_TIMEOUT; //
   
}

void gsm_AT_SETCMGF(void)
{
    //smsInProgress = FALSE;
    //smsWhitelisted = FALSE;
    gsm_state = STATE_CMGF;
    // Enable SMS message format TEXT
    memcpy(src, "AT+CMGF=1\r\n", 11); //GPS Reset
    UARTPrintF(src, 11);  //post to Modem
    gsm_tick_send = AT_SEND_TIMEOUT; // Start a Modem not responding timeout
    gsm_timeout = GSM_TIMEOUT; //
  
}

/*-----------------------------------------------------------------------------------------*/
/*! @fn         gsm_AT_SETCNMI()
*   @param      None. 
*   @return     None.
*   @brief      Send the AT Command to enable unsolicited SMS message notification
*               messages will arrive with the prefix +CMT
*
*               Version     Date        Description
*--------------------------------------------------------------------------------------------
*   @version    1           07/03/2016  Initial version.
*------------------------------------------------------------------------------------------*/
void gsm_AT_SETCNMI( void  )
{
    gsm_state = STATE_CNMI;
    // Request Automatic SMS notiticaition with +CMT: message
    memcpy(src, "AT+CNMI=3,2\r\n", 13); //
    UARTPrintF(src, 13);  //post to Modem
    gsm_tick_send = AT_SEND_TIMEOUT;                        // Start a Modem not responding timeout
    gsm_timeout = GSM_TIMEOUT; //
}

void gsm_AT_READ_SMS (void)
{
    gsm_state = STATE_READ_SMS;
    memcpy(src, "AT+CMGR=3\r\n", 11); // Content of 3rd message in the inbox
    UARTPrintF(src, 11);  //post to Modem
    gsm_tick_send = AT_SEND_TIMEOUT;                        // Start a Modem not responding timeout
    gsm_timeout = GSM_TIMEOUT; //
  
}

void gsm_AT_GPRS_SHUTDOWN (void)
{
    gsm_state = STATE_GPRS_SHUTDOWN;
    memcpy(src, "AT+CIPSHUT\r\n", 12); // 
    UARTPrintF(src, 12);  //post to Modem
    response_flags = RESP_SHUT_OK;
    gsm_tick_send = AT_SEND_TIMEOUT;                        // Start a Modem not responding timeout
    gsm_timeout = GSM_TIMEOUT; //
  
}
void gsm_AT_STARTUP_SINGLE_IP_CONNECTION (void)
{
    gsm_state = STATE_STARTUP_SINGLE_IP_CONNECTION;
    memcpy(src, "AT+CIPMUX=0\r\n", 13); // 
    UARTPrintF(src, 13);  //post to Modem
    response_flags = RESP_OK;
    gsm_tick_send = AT_SEND_TIMEOUT;                        // Start a Modem not responding timeout
    gsm_timeout = GSM_TIMEOUT; //
  
}

void gsm_AT_IP_INITIAL_MODE (void)
{
    ip_initial_mode_retry++;
    gsm_state = STATE_IP_INITIAL_MODE;
    memcpy(src, "AT+CIPSTATUS\r\n", 14); // 
    UARTPrintF(src, 14);  //post to Modem
    gsm_tick_send = AT_SEND_TIMEOUT;                        // Start a Modem not responding timeout
    gsm_timeout = GSM_TIMEOUT; //
  
}
void gsm_AT_SETCGDCONT( void )
{
byte i;
byte current_APN[] = "safaricom.com";            //TO BE DONE!!!!!!!!!!!!! Find out Saf APN
    gsm_state = STATE_SETCGDCONT;
    i = sprintf( src, "AT+CGDCONT=1,\"IP\",\"safaricom.com\",\"0.0.0.0\"\r\n"); // Set the context
    UARTPrintF( src, i ); // Write the AT command 
    gsm_tick_send = AT_SEND_TIMEOUT; // Start a Modem not responding timeout
    gsm_timeout = GSM_TIMEOUT; //
  
}

/***************************************************************//**
* @fn gsm_AT_CGATT()
* @brief Send the AT Command to see if the GPRS session is active
*******************************************************************/
void gsm_AT_CGATT( void )
{
byte i;
    gsm_state = STATE_SETCGATT; // Move on to next state
    i = sprintf(src, "AT+CGATT=1\r\n"); 
    UARTPrintF( src, i ); // Write the AT command 
    gsm_tick_send = AT_SEND_TIMEOUT; // Start a Modem not responding timeout
    gsm_timeout = GSM_TIMEOUT; //
}

void gsm_AT_GET_IMEI (void)
{
    gsm_state = STATE_GETIMEI; // Move on to next state
    memcpy( src,"AT+GSN\r\n", 8 ); // Request IMEI
    UARTPrintF( src, 8 ); // Write the AT command - 
    gsm_tick_send = AT_SEND_TIMEOUT; // Start a Modem not responding timeout
    gsm_timeout = GSM_TIMEOUT; //
  
}
void gsm_AT_CGACT( void )            // Activate PDP context
{
    gsm_state = STATE_SETCGACT; // Move on to next state
    memcpy( src,"AT+CGACT=1,1\r\n", 13 ); // Request IMEI
    UARTPrintF( src, 13 ); // Write the AT command - 
    gsm_tick_send = AT_SEND_TIMEOUT; // Start a Modem not responding timeout
    gsm_timeout = GSM_TIMEOUT; //
}

void gsm_AT_CONNECT_TO_SERVER (void)              //TCP Connection?
{
  byte i;
    gsm_state = STATE_CONNECT_TO_SERVER; // Move on to next state
    i = sprintf(src, "AT+CIPSTART=\"TCP\",\"192.168.44.144\",\"111\"\r\n");           //traccar.org ip and port numbers
    UARTPrintF( src, i ); // Write the AT command - 
    gsm_tick_send = AT_SEND_TIMEOUT; // Start a Modem not responding timeout
    gsm_timeout = GSM_TIMEOUT; //
}

void gsm_AT_SETAPN_START_TASK (void)
{
   gsm_state = STATE_SETAPN_START_TASK; // Move on to next state
   memcpy( src,"AT+CSTT\r\n", 9 ); // 
   UARTPrintF( src, 9 ); // Write the AT command - 
   gsm_tick_send = AT_SEND_TIMEOUT; // Start a Modem not responding timeout
   gsm_timeout = GSM_TIMEOUT; //
  
}

void gsm_AT_BRINGUP_WIRELESS_CONNECTION_GPRS (void)
{
   gsm_state = STATE_BRINGUP_WIRELESS_CONNECTION_GPRS; // Move on to next state
   memcpy( src,"AT+CIICR\r\n", 10 ); // 
   UARTPrintF( src, 10 ); // Write the AT command - 
   gsm_tick_send = AT_SEND_TIMEOUT; // Start a Modem not responding timeout
   gsm_timeout = GSM_TIMEOUT; //
}

void gsm_AT_GET_IP(void)
{
   gsm_state = STATE_GET_IP; // Move on to next state
   memcpy( src,"AT+CIFSR\r\n", 10 ); // 
   UARTPrintF( src, 10 ); // Write the AT command - 
   gsm_tick_send = AT_SEND_TIMEOUT; // Start a Modem not responding timeout
   gsm_timeout = GSM_TIMEOUT; //
}

void gsm_AT_SEND_TCP_PACKET (void)
{
  byte i=0;
  byte tx_buffer[100];
   gsm_state = STATE_SEND_TCP_PACKET; // Move on to next state
   memcpy( src,"AT+CIPSEND=23\r\n", 14 ); // 
   UARTPrintF( src, 14 ); // Write the AT command - 
   
   
   i = sprintf( tx_buffer,">THIS IS DATA TO SERVER" ); // TO BE DONE!!!!!!!!!!!!!!!
   UARTPrintF( tx_buffer, i); 
   
   gsm_tick_send = AT_SEND_TIMEOUT; // Start a Modem not responding timeout
   //gsm_timeout = GSM_TIMEOUT; //
   gsm_timeout = 360; // This maytake a longer time         // TO BE DONE
  
}

void gsm_AT_CLOSE_TCP_CONNECTION(void)
{
   gsm_state = STATE_CLOSE_TCP_CONNECTION; // Move on to next state
   memcpy( src,"AT+CIPCLOSE\r\n", 13 ); // 
   UARTPrintF( src, 13 ); // Write the AT command - 
   gsm_tick_send = AT_SEND_TIMEOUT; // Start a Modem not responding timeout
   gsm_timeout = GSM_TIMEOUT; //
   
  
}


#endif