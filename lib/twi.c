#include <avr/io.h>
#include <util/twi.h>
#include "twi.h"
#include "uart.h"

void twi_setup(uint8_t twbr_val)
{
        TWBR = twbr_val;
}

uint8_t twi_start()
{
        TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
        while (!(TWCR & (1 << TWINT)));
        if ((TWSR & 0xF8) != TW_START)
                return 0;
        return 1;
}

uint8_t twi_send_write_address(uint8_t address)
{
        TWDR = address;
        TWCR = (1 << TWINT) | (1 << TWEN);
        while (!(TWCR & (1 << TWINT)))
                ;
        if ((TWSR & 0xF8) != TW_MT_SLA_ACK)
        {
                return 0;
        }
        return 1;
}

uint8_t twi_send_read_address(uint8_t address)
{
        TWDR = address;
        TWCR = (1 << TWINT) | (1 << TWEN);
        while (!(TWCR & (1 << TWINT)))
                ;
        if ((TWSR & 0xF8) != TW_MR_SLA_ACK)
        {
                return 0;
        }
        return 1;
}

uint8_t twi_send_data(uint8_t data)
{
        TWDR = data;
        TWCR = (1 << TWINT) | (1 << TWEN);
        while (!(TWCR & (1 << TWINT)))
                ;
        if ((TWSR & 0xF8) != TW_MT_DATA_ACK)
        {
                return 0;
        }
        return 1;
}

uint8_t twi_restart()
{
        TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
        while (!(TWCR & (1 << TWINT)))
                ;
        if ((TWSR & 0xF8) != TW_REP_START)
        {
                return 0;
        }
        return 1;
}

uint8_t twi_send_ack()
{
        TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
        while (!(TWCR & (1 << TWINT)))
                ;
        return TWDR;
}

uint8_t twi_send_nack()
{
        TWCR = (1 << TWINT) | (1 << TWEN);
        while (!(TWCR & (1 << TWINT)))
                ;
        return TWDR;
}

void twi_stop()
{
        TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
}
