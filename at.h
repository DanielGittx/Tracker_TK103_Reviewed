#ifndef AT_H
#define AT_H

#include "typedefs.h"

#define AT_SEND_TIMEOUT                 5     // Seconds - This should be enough time to allow processing finish
#define GSM_TIMEOUT                    20     // 

extern void gsm_AT_INIT( void );
extern void gps_AT_SET_POWER_MODE (byte power_mode_state);   // default - power down,
extern void gps_AT_SET_AUTONOMY_MODE (void); // GPS module needs to be reset before it can start acquiring location information- Issue this after GPS pwr ON
extern void gps_AT_SET_FULL_FUNCTIONALITY (void); //Full GSM functionality - issue after setting autonomy mode
extern void gps_AT_GET_LOCATION_INFO (void);      // Longitued,latitude, time


extern void gsm_AT_SETCMGF(void);       // Operate in TEXT mode while reading and sending SMS - Issue this first the power on GPS
extern void gsm_AT_SETCNMI(void);
extern void gsm_AT_READ_SMS (void); 
extern void gsm_AT_GPRS_SHUTDOWN (void); //Shutdown GPRS beffore attempting to connect (optional)
extern void gsm_AT_STARTUP_SINGLE_IP_CONNECTION (void); // 
extern void gsm_AT_IP_INITIAL_MODE (void); //module should reply with STATE: IP INITIAL, if not then you need to restart with the whole process again. 
extern void gsm_AT_SETCGDCONT( void );      //Set GPRS PDP context - SAFARICOM APN (find it out????)
extern void gsm_AT_CGACT( void );            // Activate PDP context 
extern void gsm_AT_CGATT (void );        //Attach to the GPRS Service
extern void gsm_AT_GET_IMEI (void);      // Get Imei
extern void gsm_AT_CONNECT_TO_SERVER (void); // TCP Connection to server (traccar IP and PORT) ,,,, 
extern void gsm_AT_SETAPN_START_TASK(void);
extern void gsm_AT_BRINGUP_WIRELESS_CONNECTION_GPRS(void);
extern void gsm_AT_GET_IP(void);
extern void gsm_AT_SEND_TCP_PACKET (void);
extern void gsm_AT_CLOSE_TCP_CONNECTION(void);

// GPRS/GSM/GPS
enum 
{ 
  STATE_INIT,        //                 0
  STATE_GPS_POWER_ON,
  STATE_GPS_POWER_OFF,
  STATE_GPS_AUTONOMY_MODE,
  STATE_GPS_LOCATION_INFOR,
  STATE_GPS_FULL_FUNCTIONALITY,
  STATE_GPRS_SHUTDOWN,
  STATE_CMGF,
  STATE_CNMI,
  STATE_READ_SMS,
  STATE_STARTUP_SINGLE_IP_CONNECTION,
  STATE_IP_INITIAL_MODE,
  STATE_SETCGDCONT,           //        12
  STATE_SETCGACT,
  STATE_SETCGATT,
  STATE_GETIMEI,
  STATE_CONNECT_TO_SERVER,
  STATE_SETAPN_START_TASK,
  STATE_BRINGUP_WIRELESS_CONNECTION_GPRS,
  STATE_GET_IP,
  STATE_SEND_TCP_PACKET,
  STATE_CLOSE_TCP_CONNECTION,


};

enum
{ 
  RESP_NONE,
  RESP_OK,
  RESP_ERROR,
  RESP_IMEI,
  RESP_NMEA_DATA,
  RESP_SMS,
  RESP_CONNECT_OK,
  RESP_CONNECT_FAIL,
  RESP_SHUT_OK,
  RESP_CLOSE_OK,
};


extern byte gsm_IMEI[20]; ///< The IMEI of the modem
extern byte gsm_ICCID[25]; ///< The ICCID of the SIM
extern byte gsm_IP[16];
//extern byte gsm_retry;
extern byte retry_count; 
extern byte sms_delay;
extern byte gsm_FailureCount; ///< The current GSM transmission failure count


extern byte gsm_state;
extern byte gps_state;
extern byte gsm_tick_send;
extern byte gsm_timeout;
extern byte response_flags;









#endif