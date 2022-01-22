#include "eeprom.h"
#include "stm8s.h"

void eeprom_init(void)
{
  FLASH_DeInit();
  FLASH_Unlock(FLASH_MEMTYPE_DATA); //EEPROM
  FLASH_SetProgrammingTime(FLASH_PROGRAMTIME_STANDARD);
}

inline uint8_t eeprom_read_byte(uint16_t addr)
{
  return FLASH_ReadByte(addr);
}

inline void eeprom_write_byte(uint16_t addr, uint8_t data)
{
  FLASH_Unlock(FLASH_MEMTYPE_DATA);
  FLASH_ProgramByte(addr, data);
  FLASH_WaitForLastOperation(FLASH_MEMTYPE_DATA);
  FLASH_Lock(FLASH_MEMTYPE_DATA);
}

void eeprom_write_data32(uint16_t offset, uint32_t data)
{

  data32_unit_mod_t tmp;
  tmp.data32 = data;
  if (offset + 4 > EEPROM_SIZE)
  {
    //error
    return;
  }
  eeprom_write_byte(EEPROM_BASE_ADDR + offset, tmp.buf[0]);
  eeprom_write_byte(EEPROM_BASE_ADDR + offset + 1, tmp.buf[1]);
  eeprom_write_byte(EEPROM_BASE_ADDR + offset + 2, tmp.buf[2]);
  eeprom_write_byte(EEPROM_BASE_ADDR + offset + 3, tmp.buf[3]);
}

void eeprom_write_data16(uint16_t offset, uint16_t data)
{

  data16_unit_mod_t tmp;
  tmp.data16 = data;
  if (offset + 2 > EEPROM_SIZE)
  {
    //error
    return;
  }
  eeprom_write_byte(EEPROM_BASE_ADDR + offset, tmp.buf[0]);
  eeprom_write_byte(EEPROM_BASE_ADDR + offset + 1, tmp.buf[1]);
}

void eeprom_write_data8(uint16_t offset, uint8_t data)
{

  if (offset + 1 > EEPROM_SIZE)
  {
    //error
    return;
  }
  eeprom_write_byte(EEPROM_BASE_ADDR + offset, data);
}

uint32_t eeprom_read_data32(uint16_t offset)
{

  data32_unit_mod_t tmp;
  if (offset + 4 > EEPROM_SIZE)
  {
    //error
    return 0xEEEEEEEE;
  }

  tmp.buf[0] = eeprom_read_byte(EEPROM_BASE_ADDR + offset);
  tmp.buf[1] = eeprom_read_byte(EEPROM_BASE_ADDR + offset + 1);
  tmp.buf[2] = eeprom_read_byte(EEPROM_BASE_ADDR + offset + 2);
  tmp.buf[3] = eeprom_read_byte(EEPROM_BASE_ADDR + offset + 3);

  return tmp.data32;
}

uint16_t eeprom_read_data16(uint16_t offset)
{

  data16_unit_mod_t tmp;
  if (offset + 2 > EEPROM_SIZE)
  {
    //error
    return 0xEEEE;
  }
  tmp.buf[0] = eeprom_read_byte(EEPROM_BASE_ADDR + offset);
  tmp.buf[1] = eeprom_read_byte(EEPROM_BASE_ADDR + offset + 1);

  return tmp.data16;
}

uint8_t eeprom_read_data8(uint16_t offset)
{

  if (offset + 1 > EEPROM_SIZE)
  {
    //error
    return 0xEE;
  }

  return eeprom_read_byte(EEPROM_BASE_ADDR + offset);
}

void eeprom_area_clear(void)
{

  uint16_t index = 0;
  for (; index < EEPROM_SIZE; index += 2)
  {
    eeprom_write_data16(index, 0xFFFF);
  }
}
