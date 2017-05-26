#ifndef LED_H
#define LED_H

/*     Defines          */

#define LIGHT_OFF  0x00
#define LIGHT_LOW  0x02
#define LIGHT_MED  0x04
#define LIGHT_HIGH  0x06

/*
#define HI 0x03          // DCycle Low setting CCRH/L FOR 85mA
#define LO 0x30
*/

/*
#define HI 0x04          // DCycle High setting CCRH/L FOR 185mA
#define LO 0xFF
*/

/*
#define HI2 0x19     // Frequency
#define LO2 0xFF
*/

/*    Functions          */

extern void light_off(void);
extern void light_low(void);
extern void light_med(void);
extern void light_high(void);
//extern void state_machine(byte light_setting_state );
extern void state_machine(void);
extern void state_sequencer(void);
extern void power_on_init(void);
extern void set_duty_cycle (byte hi, byte lo);



/*     Global Variables          */

byte Light_Setting;


#endif


