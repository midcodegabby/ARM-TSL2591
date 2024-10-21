/*
 * i2c.c
 *
 *  Author: Gabriel Rodgers
 *  Date: 10/13/2024
 *  Purpose: configure I2C2 and implement read and write functions
 */

//include files
#include <stdint.h>
#include <stdio.h>
#include <inttypes.h>

#include "i2c.h"
#include "gpio.h"
#include "nvic.h"

//definitions and register mapping (for STM32)
#define I2C2 0x40005800

//initialization/control registers
#define I2C2_CR1 (*((volatile uint32_t *) (I2C2)))
#define I2C2_CR2 (*((volatile uint32_t *) (I2C2 + 0x04)))
#define I2C2_TIMINGR (*((volatile uint32_t *) (I2C2 + 0x10)))
#define I2C2_TIMINGR (*((volatile uint32_t *) (I2C2 + 0x10)))
#define I2C2_TIMEOUTR (*((volatile uint32_t *) (I2C2 + 0x14)))
#define I2C2_ISR (*((volatile uint32_t *) (I2C2 + 0x18)))
#define I2C2_ICR (*((volatile uint32_t *) (I2C2 + 0x1C)))

//data registers
#define I2C2_RXDR (*((volatile uint32_t *) (I2C2 + 0x24)))
#define I2C2_TXDR (*((volatile uint32_t *) (I2C2 + 0x28)))


//define some TSL2591 Registers (and address) (light sensor)
#define TSL2591_ADDRESS 0x29
#define TSL2591_ENABLE 0x01
#define TSL2591_ALS_DATA 0x14

#define TSL2591_Init_Message 0b10100000
#define TSL2591_Settings_Message0 0x00
#define TSL2591_Settings_Message1 0x03

//initialize I2C2 registers 
//I2C2 Settings: 100KHz SCL Frequency, 7-bit addressing mode
void i2c2_init(void) {
	I2C2_CR1 &= ~1; //disable I2C2 Peripheral

	//enable interrupts here... if I want any
	I2C2_CR1 |= (1 << 4); //enable NACK interrupt

	//set up timing for 16MHz I2CCLK and 100KHz transmission frequency (from ST Reference Manual)
	I2C2_TIMINGR |= (0x3 << 28); //PRESC
	I2C2_TIMINGR |= (0x4 << 20); //SCLDEL
	I2C2_TIMINGR |= (0x2 << 16); //SDADEL
	I2C2_TIMINGR |= (0xF << 8); //SCLH
	I2C2_TIMINGR |= 0x13; //SCLL
	
	I2C2_CR2 |= (TSL2591_ADDRESS << 1); //set target address

	I2C2_CR1 |= 1; //enable I2C2 Peripheral
}

//initialize communications with the target device and initialize the target device's registers
void i2c2_target_init() {
	I2C2_CR2 |= (1 << 25); //set autoend
	I2C2_CR2 |= (0x2 << 16); //NBYTES = 2 (send 2 bytes before automatic stop condition)

	//loop until bus is idle
	//int count = 0;
	//uint8_t timeout = 0xFF;
/*	while (1) {
	       	timeout = i2c2_check_bus(count);
		if (timeout == 1) break;
	}
*/

	//while ((I2C2_ISR >> 15) & 1); //wait for previous stop condition
	//printf("Bus is idle\n");
	
	I2C2_CR2 |= (1 << 13); //send start condition
	
	//I don't think I need to include address?
	while (!(I2C2_ISR >> 1) & 1); //wait for TXIS bit set to put data in I2C_TXDR
	I2C2_TXDR = ((TSL2591_ADDRESS >> 1) + 0); //Transmission register <- Address + W

	while (!(I2C2_ISR >> 1) & 1); //loop until TXIS bit is set and we can put data in I2C2_TXDR
	I2C2_TXDR = TSL2591_Init_Message; //load the init message into the transmission register

	while (!(I2C2_ISR >> 5) & 1); //wait for STOP condition
	I2C2_ICR |= (1 << 5); //clear Stop flag

	//restart but this time change the registers on the TSL2591
	I2C2_CR2 |= (0x3 << 16); //NBYTES = 3 (send 3 bytes before automatic stop condition)

	//printf("We completed the first transmission\n");

	//while ((I2C2_ISR >> 15) & 1); //wait for previous stop condition
	I2C2_CR2 |= (1 << 13); //send start condition

	//printf("We sent the start condition\n");

	while (!(I2C2_ISR >> 1) & 1); //loop until TXIS bit is set and we can put data in I2C2_TXDR
	I2C2_TXDR = ((TSL2591_ADDRESS >> 1) + 0); //Transmission register <- Address + W
	
	//printf("We sent address\n");

	while (!(I2C2_ISR >> 1) & 1); //loop until TXIS bit is set and we can put data in I2C2_TXDR
	I2C2_TXDR = TSL2591_Settings_Message0; 

	while (!(I2C2_ISR >> 1) & 1); //loop until TXIS bit is set and we can put data in I2C2_TXDR
	I2C2_TXDR = TSL2591_Settings_Message1; 

	while (!(I2C2_ISR >> 5) & 1);  //wait for STOP condition
	I2C2_ICR |= (1 << 5); //clear Stop flag

	//printf("At end of i2c2_target_init()\n");
}

//Write to a register in the target device then read and store 4 bytes to the read_buffer
void i2c2_write_read(uint32_t volatile *read_buffer) {
	//uint8_t i; //index declaration	
	
	//do the write stuff
	I2C2_CR2 &= ~(1 << 25); //clear autoend
	I2C2_CR2 |= (0x2 << 16); //NBYTES = 2 (send 2 bytes before automatic stop condition)

	I2C2_CR2 |= (1 << 13); //send start condition
	
	//I don't think I need to include address?
	while (!(I2C2_ISR >> 1) & 1); //wait for TXIS bit set to put data in I2C_TXDR
	I2C2_TXDR = ((TSL2591_ADDRESS >> 1) + 0); //Transmission register <- Address + W

	while (!(I2C2_ISR >> 1) & 1); //loop until TXIS bit is set and we can put data in I2C2_TXDR
	I2C2_TXDR = 0xFF; 
	
	while (!(I2C2_ISR >> 6) & 1); //wait for NBYTES to be transferred
	I2C2_ICR |= (1 << 14); //send stop condition

	while (!(I2C2_ISR >> 5) & 1); //wait for STOP condition
	I2C2_ICR |= (1 << 5); //clear Stop flag

/*
	//do the read stuff
	for (i = 0; i < 4; i++) {
		while (!((I2C2_ISR >> 2) & 1)); //loop until RXNE bit is set

		//transfer data from register to read_buffer in big-endian format
		*read_buffer = (I2C2_RXDR >> (8*i)); 
	}
*/
	
}	

//Use timers to check if the bus goes idle; returns 1 if idle and 0 if not idle
uint8_t i2c2_check_bus(int count) {
	I2C2_TIMEOUTR |= (1 << 12); //set TIDLE
	I2C2_TIMEOUTR |= (0xC3); //25 ms = 0xC3
	I2C2_TIMEOUTR |= (1 << 15); //enable timeout timer
	
	for (count = 0; count < 1500000; count++); //loop for about 30 ms 
	 
	if ((I2C2_ISR >> 12) & 1) {
		I2C2_ICR &= ~(1 << 12); //clear timout flag
		I2C2_TIMEOUTR &= ~(1 << 15); //disable timout timer
		return 1; //bus went idle
	}
			
	I2C2_TIMEOUTR &= ~(1 << 15); //disable timout timer
	return 0; //bus did not go idle
}

//Function to resolve I2C deadlocks
void i2c2_resolve_deadlock(void) {
	
}


//IRQ handler for I2C2 event interrupts; for now for NACKs
void I2C2_EV_IRQHandler(void) {
	nvic_disable();
	printf("NACK Detected!!!\n");
	nvic_enable();
}

