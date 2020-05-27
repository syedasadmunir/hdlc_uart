#ifndef twi_h
#define twi_h

#define F_SCL 100000UL /* SCL frequency */
#define PRESCALER 1
#define TWBR_VAL ((((F_CPU / F_SCL) / PRESCALER) - 16 ) / 2)

void twi_setup(uint8_t twbr_val);
uint8_t twi_start();
uint8_t twi_send_write_address(uint8_t address);
uint8_t twi_send_read_address(uint8_t address);
uint8_t twi_send_data(uint8_t data);
uint8_t twi_restart();
uint8_t twi_send_ack();
uint8_t twi_send_nack();
void twi_stop();

#endif