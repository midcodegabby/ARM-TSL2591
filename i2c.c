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

#include "i2c.h"
#include "gpio.h"

//definitions and register mapping (for STM32)
#define I2C2 0x48005800

//initialization/control registers
#define I2C2_CR1 (*((volatile uint32_t *) (I2C2)))
#define I2C2_CR2 (*((volatile uint32_t *) (I2C2 + 0x04)))
#define I2C2_TIMINGR (*((volatile uint32_t *) (I2C2 + 0x10)))
#define I2C2_TIMINGR (*((volatile uint32_t *) (I2C2 + 0x10)))
#define I2C2_TIMEOUTR (*((volatile uint32_t *) (I2C2 + 0x14))) //probably not used
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
	//enable NACK interrupt

	//set up timing for 16MHz I2CCLK and 100KHz transmission frequency (from ST Reference Manual)
	I2C2_TIMINGR |= (0x3 << 28); //PRESC
	I2C2_TIMINGR |= (0x4 << 20); //SCLDEL
	I2C2_TIMINGR |= (0x2 << 16); //SDADEL
	I2C2_TIMINGR |= (0xF << 8); //SCLH
	I2C2_TIMINGR |= 0x13; //SCLL

	printf("here\n");	

	I2C2_CR2 |= (TSL2591_ADDRESS << 1); //set target address

	I2C2_CR1 |= 1; //enable I2C2 Peripheral
}

//initialize communications with the target device and initialize the target device's registers
void i2c2_target_init() {
	uint8_t bus_state = 0xFF; //initialize an I2C bus state variable
	
	I2C2_CR2 |= (1 << 25); //set autoend
	I2C2_CR2 |= (0x2 << 16); //NBYTES = 2 (send 2 bytes before automatic stop condition)

	while (bus_state != 1) bus_state = gpio_i2c2_bus_poll(); //loop until bus is idle
	while ((I2C2_ISR & 1) != 1); //loop until transmission register is empty

	I2C2_TXDR = ((TSL2591_ADDRESS >> 1) + 0); //load the address into the transmission register
	I2C2_CR2 |= (1 << 13); //send start condition

	while ((I2C2_ISR & 1) != 1); //loop until the transmission register is empty

	I2C2_TXDR = TSL2591_Init_Message; //load the init message into the transmission register
	
	//loop until the Stop condition is sent
	while (!(I2C2_ISR >> 5) & 1); 

	I2C2_ICR |= (1 << 5); //clear Stop flag
			     

	//restart but this time change the registers on the TSL2591
	I2C2_CR2 |= (0x3 << 16); //NBYTES = 3 (send 3 bytes before automatic stop condition)

	while (bus_state != 1) bus_state = gpio_i2c2_bus_poll(); //loop until bus is idle
	while ((I2C2_ISR & 1) != 1); //loop until transmission register is empty

	I2C2_TXDR = ((TSL2591_ADDRESS >> 1) + 0); //load the address into the transmission register
	I2C2_CR2 |= (1 << 13); //send start condition

	while ((I2C2_ISR & 1) != 1); //loop until the transmission register is empty

	I2C2_TXDR = TSL2591_Settings_Message0; 
	while ((I2C2_ISR & 1) != 1); //loop until the transmission register is empty

	I2C2_TXDR = TSL2591_Settings_Message1; 
	while ((I2C2_ISR & 1) != 1); //loop until the transmission register is empty
	
	//loop until the Stop condition is sent
	while (!(I2C2_ISR >> 5) & 1); 

	I2C2_ICR |= (1 << 5); //clear Stop flag
	
}

//Write to a register in the target device then read and store 4 bytes to the read_buffer
void i2c2_write_read(uint32_t volatile *read_buffer) {
	uint8_t i; //index declaration	
	
	//do the write stuff
	
	//do the read stuff
	for (i = 0; i < 4; i++) {
		while (!((I2C2_ISR >> 2) & 1)); //loop until RXNE bit is set

		//transfer data from register to read_buffer in big-endian format
		*read_buffer = (I2C2_RXDR >> (8*i)); 
	}

	//

		

}	
