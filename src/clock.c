/* 
 * Author: Gabriel Rodgers
 * Date: 10/13/2024
 * Purpose: Configure clocks for system and peripherals
*/

#include <stdint.h>
#include <stdio.h>

#include "clock.h"

//define registers
#define RCC 0x40021000
#define FLASH 0x40022000

#define RCC_CR (*((volatile uint32_t *) (RCC))) 
#define RCC_CFGR (*((volatile uint32_t *) (RCC + 0x08)))
#define RCC_AHB1ENR (*((volatile uint32_t *) (RCC + 0x48)))
#define RCC_AHB2ENR (*((volatile uint32_t *) (RCC + 0x4C)))
#define RCC_APB1ENR (*((volatile uint32_t *) (RCC + 0x58)))
#define RCC_APB2ENR (*((volatile uint32_t *) (RCC + 0x60)))
#define RCC_CCIPR (*((volatile uint32_t *) (RCC + 0x88)))
									
#define FLASH_ACR (*((volatile uint32_t *) (FLASH)))		

//remove these
uint32_t SYS_CLK = 48;
uint32_t AHB_CLK = 8;
uint32_t APB1_CLK = 8;

//initialize sysclk and flash wait state
void sysclk_init(void) {
	FLASH_ACR |= (1 << 1); //2 wait states

	//select MSI range from RCC_CR register
	RCC_CR |= (1 << 3);
	
	//set MSI range to 48 MHz
	RCC_CR &= ~(0xF << 4);
	RCC_CR |= (0xB << 4);
}

//enable HSI16 clock
void hsi_init(void) {
	RCC_CR |= (1 << 9); //enable HSI16 for Stop modes
	RCC_CR |= (1 << 8); //enable HSI16
	
	//loop until HSI16 is ready
	while (((RCC_CR >> 10) & 1) != 1);
}


//initialize peripheral clocks: GPIOA/C/F, SYSCFG, I2C
void peripheral_clk_init(void) {
	RCC_AHB2ENR |= (1 << 0); //GPIOA enable
	RCC_AHB2ENR |= (1 << 1); //GPIOB enable
	RCC_AHB2ENR |= (1 << 2); //GPIOC enable
	//RCC_AHB2ENR |= (1 << 5); //GPIOF enable
	RCC_APB2ENR |= (1 << 0); //SYSCFG enable

	//Set I2C2CLK to HSI16
	RCC_CCIPR |= (1 << 15);

	RCC_APB1ENR |= (1 << 22); //I2C2 enable

}
