/*
 * This program encodes data in packets and sends across UART  
 * 
 * Author: Syed Asad Munir (syedasadmunir@gmail.com)
 * Date: May 25, 2020
 */

#ifndef __AVR_ATmega168PA__
	#define __AVR_ATmega168PA__
#endif

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "uart.h"
#include "lm75.h"
#include "twi.h"

#define FRAME_BOUNDARY 0x7E
#define BOUNDARY_ESCAPE_CHARACTER 0X7A
#define CRC_POLY 0x31

uint8_t gen_crc(uint8_t *data, uint8_t len)
{
	uint8_t crc = 0xff;
	uint8_t i,j;

	for (i = 0; i < len; i++) {
		crc ^= data[i];
		for (j = 0; j < 8; j++) {
			if((crc & 0x80) != 0)
				crc = (uint8_t)((crc << 1) ^ CRC_POLY);
			else
				crc <<= 1;
			
		}
	}
	return crc;
}

int main(void)
{       
	uint8_t data[2];
	uint8_t tempdata[3];
    initUSART();
	twi_setup(TWBR_VAL);
         
	while (1) {
		get_temperature(data);

                /* todo: if data is 0x7e, need to escape it */
                tempdata[0] = 0x23;
		tempdata[1] = data[0];
		tempdata[2] = data[1];
		transmitByte(FRAME_BOUNDARY);
		for (uint8_t a = 0; a < sizeof(tempdata); a++) {
			transmitByte(tempdata[a]);
		}
		transmitByte(gen_crc(tempdata,sizeof(tempdata)));
		transmitByte(FRAME_BOUNDARY);
		
                /* at least one 0xff should be transmitted after the frame end */
                transmitByte(0xff);
		transmitByte(0xff);
		_delay_ms(2000);
	}
}
