#ifndef lm75_h
#define lm75_h

#include <stdint.h>

#define LM75_ADDRESS_W               0b10010000
#define LM75_ADDRESS_R               0b10010001
#define LM75_TEMP_REGISTER           0b00000000
#define LM75_CONFIG_REGISTER         0b00000001
#define LM75_THYST_REGISTER          0b00000010
#define LM75_TOS_REGISTER            0b00000011

/*
 * Return 2 bytes of temperature reading
 */
void get_temperature(uint8_t *data);

#endif