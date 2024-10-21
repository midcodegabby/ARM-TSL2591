/*
 * gpio.c
 *
 *  Author: Gabriel Rodgers
 *  Date: 10/13/2024
 *  Purpose: configure GPIOs for I2C2, button input, and LED2
 */

//include files
#include <stdint.h>
#include "gpio.h"

//definitions and register mapping
#define GPIOA 0x48000000
#define GPIOB 0x48000400
#define GPIOC 0x48000800
//#define GPIOF 0x48001400

#define GPIOA_MODER (*((volatile uint32_t *) GPIOA))
#define GPIOA_OTYPER (*((volatile uint32_t *) (GPIOA + 0x04)))
#define GPIOA_ODR (*((volatile uint32_t *) (GPIOA + 0x14)))

#define GPIOB_MODER (*((volatile uint32_t *) GPIOB))
#define GPIOB_OTYPER (*((volatile uint32_t *) (GPIOB + 0x4)))
#define GPIOB_OSPEEDR (*((volatile uint32_t *) (GPIOB + 0x8)))
#define GPIOB_PUPDR (*((volatile uint32_t *) (GPIOB + 0xC)))
#define GPIOB_IDR (*((volatile uint32_t *) (GPIOB + 0x10)))
#define GPIOB_AFRH (*((volatile uint32_t *) (GPIOB + 0x24)))

#define GPIOC_MODER (*((volatile uint32_t *) GPIOC))
#define GPIOC_PUPDR (*((volatile uint32_t *) (GPIOC + 0x0C)))
#define GPIOC_IDR (*((volatile uint32_t *) (GPIOC + 0x10)))

/*
#define GPIOF_MODER (*((volatile uint32_t *) GPIOF))
#define GPIOF_OTYPER (*((volatile uint32_t *) (GPIOF + 0x4)))
#define GPIOF_OSPEEDR (*((volatile uint32_t *) (GPIOF + 0x8)))
#define GPIOF_PUPDR (*((volatile uint32_t *) (GPIOF + 0xC)))
#define GPIOF_IDR (*((volatile uint32_t *) (GPIOF + 0x10)))
#define GPIOF_AFRL (*((volatile uint32_t *) (GPIOF + 0x20)))
*/
/*
//initialize PF0 to be I2C2_SDA, PF1 to be I2C2_SCL
void gpio_i2c2_init(void) {
	GPIOF_AFRL |= (1 << 2); //AF4 for PF0
	GPIOF_AFRL |= (1 << 6); //AF4 for PF1

	GPIOF_OSPEEDR |= (0x3 | (0x3 << 2)); //set very high speed for PF0/PF1

	//NEW:
	GPIOF_PUPDR |= (1 | (1 << 2)); //pull ups

	GPIOF_OTYPER |= (0x3); //open drain for PF0 and PF1
	
	GPIOF_MODER &= ~(1); //alt function for PF0 
	GPIOF_MODER &= ~(1 << 2); //alt function for PF1

}
*/

void gpio_i2c2_init(void) {
	GPIOB_AFRH |= (0x4 << 20); //AF4 for PB13
	GPIOB_AFRH |= (0x4 << 24); //AF4 for PF14

	GPIOB_OSPEEDR |= ((0x3 << 26) | (0x3 << 28)); //set very high speed for PB13/PB14

	//NEW:
	GPIOB_PUPDR |= ((1 << 26) | (1 << 28)); //pull ups for PB13/PB14

	GPIOB_OTYPER |= (0x3 << 13); //open drain for PB13/PB14
	
	GPIOB_MODER &= ~(1 << 26); //alt function for PB13 
	GPIOB_MODER &= ~(1 << 28); //alt function for PB14
}

//function to return a 1 if the I2C2 bus is idle and 0 otherwise
uint8_t gpio_i2c2_bus_poll(void) {
	if ((GPIOB_IDR & 0x3) == 0x3) return 1;
	else return 0;
}

void gpio_button_init(void) {
	//set button pin (B1, connected to PC13) to be general purpose input mode
	GPIOC_MODER &= ~(0x3 << 26);

	//set input to pull up R
	GPIOC_PUPDR &= ~(0x3 << 26);
	GPIOC_PUPDR |= (1 << 26);
}

uint8_t gpio_button_poll(uint8_t button) {
	//check input data register and assign it to the button parameter
	button = ((GPIOC_IDR >> 13) & 1);

	//should probably do some debouncing here

	return button;
}

void gpio_led_init(void) {
	//set LED pin (GPIOA pin 5) to be general purpose output mode
	GPIOA_MODER |= (1 << 10);
	GPIOA_MODER &= ~(1 << 11);

	GPIOA_OTYPER &= ~(1 << 5); // set PA5 to be push-pull
}

void gpio_led_on(void) {
	GPIOA_ODR |= (1 << 5); 
}

void gpio_led_off(void) {
	GPIOA_ODR &= ~(1 << 5); 
}

void gpio_led_toggle(void) {
	GPIOA_ODR ^= (1 << 5); 
}

