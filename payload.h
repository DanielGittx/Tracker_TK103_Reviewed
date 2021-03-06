#ifndef PAYLOAD_H
#define PAYLOAD_H

#include "typedefs.h"


//TCP - TK103 Implementation
#define HEAD             0x28                            // symbol char '(' 
#define DEVICE_ID       (dword)(012345678912345)         // Default Device ID(imei) - 15 bytes: 
//#define COMMAND      
//#define PAYLOAD                                        // Should be less than 1k - 1024 bytes
#define TAIL            0x29                             // symbol char ')'    


extern byte print_enr_buff[50];

// Messages to server //
extern void device_enrolling_message(byte* uart2_RX_data_ptr );
extern void continous_feedback_message(byte* _imei);                            // 
extern void handshaking_message(byte t);

//Messages from server (server ACKS)
extern void server_response (void);        // Server response after receiving ENROLLING MESSAGE from device
extern void handshaking_message_answer (void);



#endif