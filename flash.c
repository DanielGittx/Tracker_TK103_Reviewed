#ifndef FLASH_C
#define FLASH_C


#include <iostm8s003f3.h>          // STM Processor type
#include <io_macros.h>
#include "flash.h"



void SetDefaultValues(void);
void flash_control_register(void);



//  Data to write into the EEPROM.
unsigned int _pulseLength[] = { 2000U, 27830U, 400U, 1580U, 400U, 3580U, 400U };
unsigned char _onOrOff[] =    {   1,      0,     1,     0,    1,     0,    1 };
char numberOfValues = 7;


/*
 
  Write access to option byte disabled
  word program operation disabled
  Block erase operation enabled
  Fast block program operation disabled
  standard block programming operation enabled

*/

void flash_control_register(void)
{
    FLASH_CR2 = 0x21;        
    FLASH_NCR2 = 0xde;
    
}


//----------------------------------------
//  Write the default values into EEPROM.

void SetDefaultValues(void)
{
//  Check if the EEPROM is write-protected.  If it is then unlock the EEPROM.

if (FLASH_IAPSR_DUL == 0)
{
FLASH_DUKR = 0xae;
FLASH_DUKR = 0x56;
}
//  Write the data to the EEPROM.

char *address = (char *) 0x9000;        //  EEPROM base address.
*address++ = (char) numberOfValues;
for (int index = 0; index < numberOfValues; index++)
{
*address++ = (char) (_pulseLength[index] & 0xff);
*address++ = (char) ((_pulseLength[index] >> 8) & 0xff);
*address++ = _onOrOff[index];
}
//  Now write protect the EEPROM.

FLASH_IAPSR_DUL = 0;
}





#endif