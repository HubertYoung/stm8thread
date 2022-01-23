#ifndef EEPROM_INNER_H
#define EEPROM_INNER_H

#include "stm8s.h"

#define EEPROM_BASE_ADDR 0x4000
#define EEPROM_SIZE 0x07FF


#define PWM_CYCLE_ADD   0x0010 // pwm 占空比地址
#define PWM_PERIOD_ADD  0x0020 // pwm 占空比地址


union data32_unit_mod
{

    uint32_t data32;
    uint8_t buf[4];
};

typedef union data32_unit_mod data32_unit_mod_t;

union data16_unit_mod
{

    uint16_t data16;
    uint8_t buf[2];
};
typedef union data16_unit_mod data16_unit_mod_t;

/**
    外部函数声明
*/
void eeprom_init(void);
uint8_t eeprom_read_byte(uint16_t addr);
void eeprom_write_byte(uint16_t addr, uint8_t data);

void eeprom_write_data32(uint16_t offset, uint32_t data);
void eeprom_write_data16(uint16_t offset, uint16_t data);
void eeprom_write_data8(uint16_t offset, uint8_t data);

uint32_t eeprom_read_data32(uint16_t offset);
uint16_t eeprom_read_data16(uint16_t offset);
uint8_t eeprom_read_data8(uint16_t offset);

void eeprom_area_clear(void);

#endif
