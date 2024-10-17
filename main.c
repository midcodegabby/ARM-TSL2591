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
	int i = 0;
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
	i2c2_init();

	for (i = 0; i < 32; i++);

	i2c2_target_init();
	//nvic_priority();

	while(1);

	return 0;
}

