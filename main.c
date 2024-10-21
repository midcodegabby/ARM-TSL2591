/*
 * main.c
 *
 *  Author: Gabriel Rodgers
 *  Date: 10/13/2024
 *  Purpose: Implement STM332-TSL2591 communication logic
 */

#include <stdint.h>
#include <stdio.h>

#include "clock.h"
#include "gpio.h"
#include "i2c.h"
#include "nvic.h"
#include "exti.h"
#include "systick.h"

//semihosting init function:
extern void initialize_monitor_handles(void);

//globals

int main(void) {
	
	//TSL2591 init messages
	uint32_t init_msg1 = 0xA0;
	uint32_t init_msg2 = 0x0003;

	//uint32_t volatile value = 0;
	//initialize clocks
	sysclk_init();
	hsi_init();
	peripheral_clk_init();

	//initialize gpios
	gpio_button_init();
	gpio_led_init();
	gpio_i2c2_init();

	//initialize button interrupts and i2c 
	exti_init();
	exti_enable();
	nvic_enable();
	nvic_priority();
	i2c2_init();

	i2c2_write(1, &init_msg1);
	i2c2_write(2, &init_msg2);
	
	while(1) {

	}

	return 0;
}

