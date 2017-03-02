/**
 *   @file       setup.c
 */

#include <stdio.h>
#include <string.h>
#include "stm32f10x.h"
#include "..\setup\setup.h"
#include "core_cm3.h"
//#include "lib\stm32l1xx_flash.h"

//unsigned char SaveSetupFlag;
//unsigned char LoadSetupFlag;

/* Private variables ---------------------------------------------------------*/
//  __IO FLASH_Status FLASHStatus = FLASH_COMPLETE;


// struct sSetupControl SetupControl;
// struct sSetup Setup;

/*
--- FUNKCJE PRYWATNE ---------------------------------------------------------------
void DefaultSettings(void)
{
  if( *(unsigned int *)(DATA_EEPROM_START_ADDR) != 0x4B49 )
  {
    SaveDefaultSetup(); 
    WriteIntToEEPROM(DATA_EEPROM_START_ADDR, 0x4B49);
  }
  
  LoadSetupStruct();
  
  Sep.EmcCMXCntErr = (unsigned int *)(DATA_EEPROM_ERROR_ADDR); 
  
  Prepare_NoiseFrame_Address(Sep.Setup.Address);
 
  #ifdef DEBUG_SETUP_C
    printf("Ustawienia domyślne -> standardowe\n\r");
  #endif
}


void WriteIntToEEPROM(unsigned int Address, unsigned int Data)
{
  ctl_global_interrupts_disable();
  
  DATA_EEPROM_Unlock();
  FLASH_ClearFlag(FLASH_FLAG_EOP|FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR | FLASH_FLAG_SIZERR | FLASH_FLAG_OPTVERR | FLASH_FLAG_OPTVERRUSR);
  
  do
  {
    if(Data)
      FLASHStatus = DATA_EEPROM_ProgramWord( Address, Data);
    else
      FLASHStatus = DATA_EEPROM_EraseWord( Address );
  
  }while(FLASHStatus != FLASH_COMPLETE);
  
  DATA_EEPROM_Lock();

  ctl_global_interrupts_enable();
}


void SaveDefaultSetup(void)
{
  WriteIntToEEPROM(DATA_EEPROM_COEFF_GTZ,  COEFF_FREQ_600Hz);
  WriteIntToEEPROM(DATA_EEPROM_CMX_VAD,    POWER_OF_SPEECH);
  WriteIntToEEPROM(DATA_EEPROM_SEP_LANG,   LANGUAGE_PL);
  WriteIntToEEPROM(DATA_EEPROM_CMX_VOL_EX, 7);  /// MAX to 7
  WriteIntToEEPROM(DATA_EEPROM_CMX_VOL_EC, 7);  /// MAX to 7
  WriteIntToEEPROM(DATA_EEPROM_ERROR_ADDR, 0);//WriteIntToEEPROM(Sep.EmcCMXCntErr, 0); 
}

void SaveSetup(void)
{
  WriteIntToEEPROM(DATA_EEPROM_COEFF_GTZ,  Sep.Setup.Local.Coeff.GRTZ_600Hz);
  WriteIntToEEPROM(DATA_EEPROM_CMX_VAD,    Sep.Setup.Local.Coeff.VAD_Speech);
  WriteIntToEEPROM(DATA_EEPROM_SEP_LANG,   Sep.Setup.Local.Language);
  WriteIntToEEPROM(DATA_EEPROM_CMX_VOL_EX, Sep.Setup.Local.Volume.OutputVoice);  /// MAX to 7
  WriteIntToEEPROM(DATA_EEPROM_CMX_VOL_EC, Sep.Setup.Local.Volume.InputVoice);  /// MAX to 7
}

void LoadSetupStruct(void)
{
  Sep.Setup.Local.Coeff.GRTZ_600Hz   = *(unsigned int *)(DATA_EEPROM_COEFF_GTZ);
  Sep.Setup.Local.Coeff.VAD_Speech   = *(unsigned int *)(DATA_EEPROM_CMX_VAD);
  Sep.Setup.Local.Language           = *(unsigned int *)(DATA_EEPROM_SEP_LANG);
  Sep.Setup.Local.Volume.InputVoice  = *(unsigned int *)(DATA_EEPROM_CMX_VOL_EC);  /// Regulacja wzmocnienia sygnału EC/94
  Sep.Setup.Local.Volume.OutputVoice = *(unsigned int *)(DATA_EEPROM_CMX_VOL_EX);  /// Regulacja wzmocnienia sygnału EX
}


 
//You can directly read from the corresponding address, e.g.
//#define SRC_ADDR 0x08080000
//
//memcpy(&dest, (void *) SRC_ADDR, sizeof(dest));
*/


/*--------------------------------------------------------------------------------------------*/




