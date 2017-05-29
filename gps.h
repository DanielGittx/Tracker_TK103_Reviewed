#ifndef GPS_H
#define GPS_H

#include "typedefs.h"
#include "at.h"


#define GPS_INFO_ID     "$GPGGA"


extern byte at_parse (byte t);
extern void gsm_gps_response_matching (byte parse_flags );
extern void gsm_gps_turnoff( void );
extern void gsm_gps_turnon( void );
extern void gsm_tick (void);
extern void TK103_protocol (void);
extern void gsm_sequencer (void);

extern byte ip_initial_mode_retry;
extern byte global_power_mode_state;






#endif