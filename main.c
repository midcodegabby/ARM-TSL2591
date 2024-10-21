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
	
	//TSL2591 init message: (7-5) COMMAND reg normal operation, (4-0) reg 0x00, (7-0) data 0x03
	uint32_t init_msg = 0xA003;

	//TSL2591 registers 
	uint32_t id_reg = 0xB2; //(7-5) COMMAND reg normal operation, (4-0) reg 0x12 
	uint32_t data_reg = 0xB4; //(7-5) COMMANd reg normal operation, (4-0) reg 0x14 

	//var to hold TSL2591 raw data
	uint32_t raw_data = 0;

	//index var
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
	nvic_priority();
	i2c2_init();
	
	//initialization message to the TSL2591
	i2c2_write(2, &init_msg);

	//check device id
	//raw_data = 0; //reset to 0 before every call
	//i2c2_write_read(1, &id_reg, &raw_data);
	
	while(1) {
		//poll every 10 seconds: LOL use timer in the future
		for (i = 0; i < 48000000; i++); 

		raw_data = 0; //reset to 0 before every call
		i2c2_write_read(4, &data_reg, &raw_data);

		printf("TSL2591 Raw Data: %lu\n", raw_data);
	}

	return 0;
}

