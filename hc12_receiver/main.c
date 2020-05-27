/*
 * This program implements an SDLC like frame structure to receive temerature
 * data from an LM75a sensor and transmits it to the serial interface
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
#include <stdbool.h>
#include <stdint.h>

#include "uart.h"
#include "ftoa.h"

#define DEBUG_MODE false

/* CRC related definitions */
#define FRAME_BOUNDARY 0x7E
#define BOUNDARY_ESCAPE_CHARACTER 0X7A
#define CRC_POLY 0x31

volatile bool frame_started = false;
volatile bool escape_flag = false;
volatile uint8_t frame_index = 0;
volatile uint8_t frame[25];
volatile bool frame_ready = false;

void cpy_array(uint8_t *source_array, uint8_t *dest_array, uint8_t length)
{
	for (uint8_t i = 0; i < length; i++) {
		dest_array[i] = source_array[i];
	}
}

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
	DDRB |= 0xff;
	uint8_t local_frame[25];
	uint8_t frame_length;
	uint16_t full_data;
	initUSART();
	usart_enable_interrupt();
	sei();
	while (1)
	{
		if (frame_ready)
		{
			#if DEBUG_MODE
				printString("array ready");
			#endif
			frame_length = frame_index;
			frame_index = 0;
			
			#if DEBUG_MODE
				printString("Frame length is :");
				printHexByte(frame_length);
				printString("\r\n");
			#endif

			cpy_array(frame,local_frame,frame_length);
			
			frame_ready = false;
			uint8_t crc;
			uint8_t msg_crc;
			crc = gen_crc(local_frame,frame_length-1);
			msg_crc = local_frame[frame_length-1];
			
			#if DEBUG_MODE
				printString("message crc is :");
				printHexByte(msg_crc);
				printString("\r\n");
			#endif

			#if DEBUG_MODE
				printString("calculated crc is :");
				printHexByte(crc);
				printString("\r\n");
			#endif

			if (crc == msg_crc) {
				switch(local_frame[0]){
					case 0x23:
					
					full_data = (uint16_t)(local_frame[1] << 8) | (uint16_t)(local_frame[2] & 0xff);

					float temp;
					temp = (float)(full_data >> 5)*0.125;

					char buff[20];
					ftoa(temp,buff,3);
					printString(buff);
					printString("\r\n");
					break;
				
					default:
					printString("*");
				}
			} else
			{
				transmitByte(0x2d);
			}
		}
	};
	return 0;
}

ISR(USART_RX_vect)
{
	uint8_t data;
	data = UDR0;
	//cli();
	
	#if DEBUG_MODE
		printString("Data received: ");
		printHexByte(data);
		printString("\r\n");
	#endif

	if (escape_flag) {
		
		#if DEBUG_MODE
			printString("escape flag found set\r\n");
		#endif

		if (frame_started) {
			if (data == BOUNDARY_ESCAPE_CHARACTER) {
				
				#if DEBUG_MODE
					printString("escape character detected\r\n");
				#endif

				escape_flag = false;
				frame[frame_index++] = 0x7E;
				return;
			}
			
			#if DEBUG_MODE
				printString("end of frame detected\r\n");
			#endif

			frame_ready = true;
			frame_started = false;
			escape_flag = false;
			return;
		} // WHAT IF ANOTHER 7E COMES IN 
		if (data != FRAME_BOUNDARY) {
			
			#if DEBUG_MODE
				printString("starting frame\r\n");
			#endif

			frame_started = true;
			frame_index = 0;
			escape_flag = false;
		}
	}

	//printString("no escapte flag\r\n");

	if (data == FRAME_BOUNDARY) {
		
		#if DEBUG_MODE
			printString("frame boundary found. Index is: ");
			printHexByte(frame_index);
			printString("\r\n");
		#endif

		escape_flag = true;
		return;
	}

	if (frame_started){
		
		#if DEBUG_MODE
			printString("data found which is: ");
			printHexByte(data);
			printString("\r\n");
		#endif

		frame[frame_index++] = data;
		#if DEBUG_MODE
			printString(". Index is: ");
			printHexByte(frame_index);
			printString("\r\n");
		#endif
	}
	//sei();	
}
