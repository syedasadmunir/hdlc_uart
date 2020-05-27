#include "lm75.h"
#include "twi.h"

void get_temperature(uint8_t *data)
{
    twi_start();
    twi_send_write_address(LM75_ADDRESS_W);
    twi_send_data(LM75_TEMP_REGISTER);
    twi_restart();
    twi_send_read_address(LM75_ADDRESS_R);
    data[0] = twi_send_ack();
    data[1] = twi_send_nack();
    twi_stop();
}