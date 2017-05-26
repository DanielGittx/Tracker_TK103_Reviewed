
#ifndef TYPEDEFS_H
#define TYPEDEFS_H


/*-----------------------------------------------------------------------------------------*/
/*  typedefs  
*------------------------------------------------------------------------------------------*/

typedef unsigned char   byte;           // 8-bit
typedef unsigned int    word;           // 16-bit
typedef unsigned long   dword;          // 32-bit


#define NULL_POINTER (byte*)0x0000

/*-----------------------------------------------------------------------------------------*/
/*  Unions  
*------------------------------------------------------------------------------------------*/
typedef union _BYTE
{
    byte _byte;
    struct
    {
        unsigned b0:1;
        unsigned b1:1;
        unsigned b2:1;
        unsigned b3:1;
        unsigned b4:1;
        unsigned b5:1;
        unsigned b6:1;
        unsigned b7:1;
    };
} BYTE;

typedef union _WORD
{
    word _word;
    struct
    {
        byte byte0;
        byte byte1;
    };
    struct
    {
        BYTE Byte0;
        BYTE Byte1;
    };
    struct
    {
        BYTE LowB;
        BYTE HighB;
    };
    struct
    {
        byte v[2];
    };
} WORD;


struct {
  unsigned char one_second: 1;
  unsigned char button_press:    1;
  unsigned char security_pulse:  1;
  unsigned char ten_secs_elapse: 1;
  unsigned char noise_mask:1;
  
  
} flag;

/*-----------------------------------------------------------------------------------------*/
/*  Enums
*------------------------------------------------------------------------------------------*/

enum { FALSE, TRUE };
enum { LOW, HIGH };
enum { OFF, ON };
enum { FAIL, OK };


   char str1[15];
   char str2[15];
   int ret;


 /*   
    
    if ( Light_Setting == LIGHT_LOW)
     {
       light_low();
     }
    
     if ( Light_Setting == LIGHT_HIGH)
     {
       light_high();
     }
    if ( Light_Setting == LIGHT_OFF)
     {
       light_off();
     }
*/

#endif

