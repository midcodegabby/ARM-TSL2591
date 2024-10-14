/*
 * i2c.c
 *
 *  Author: Gabriel Rodgers
 *  Date: 10/13/2024
 *  Purpose: configure I2C2 and implement read and write functions
 */

//include files
#include <stdint.h>

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
	uint8_t bus_state = 0xFF; //initialize an I2C bus state variable
				  
	//enable interrupts here... if I want any

	//set up timing for 16MHz I2CCLK and 100KHz transmission frequency (from ST Reference Manual)
	I2C2_TIMINGR |= (0x3 << 28); //PRESC
	I2C2_TIMINGR |= (0x4 << 20); //SCLDEL
	I2C2_TIMINGR |= (0x2 << 16); //SDADEL
	I2C2_TIMINGR |= (0xF << 8); //SCLH
	I2C2_TIMINGR |= 0x13; //SCLL

	I2C2_CR2 |= (1 << 25); //set autoend
	I2C2_CR2 |= (1 << 16); //NBYTES = 1 (send 1 byte before automatic stop condition)
	I2C2_CR2 |= (TSL2591_ADDRESS << 1); //set target address

	//wait for transmit register to be empty	
	while ((I2C_ISR & 1) != 1);

	I2C2_TXDR = TSL2591_Init_Message; //load the init message into the transmission register

	I2C2_CR1 |= 1; //enable I2C2 Peripheral

	//loop until the I2C2 bus is idle - MIGHT NEED TO ONLY CHECK SDA?
	while (bus_state != 1) bus_state = gpio_i2c2_bus_poll();

	I2C2_CR2 |= (1 << 13); //send start condition
	
	//loop until the Stop condition is sent
	while (!(I2C2_ISR >> 5) & 1); 

	//clear the Stop flag
	I2C2_ICR |= (1 << 5);

	//now we need to send the initialization settings to the TSL2591's registers
	
	


	
	

}
