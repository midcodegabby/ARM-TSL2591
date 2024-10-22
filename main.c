/*
 * main.c
 *
 *  Author: Gabriel Rodgers
 *  Date: 10/13/2024
 *  Purpose: Implement STM332-TSL2591 communication logic
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "clock.h"
#include "gpio.h"
#include "i2c.h"
#include "nvic.h"
#include "exti.h"
#include "tsl2591_functions.h"

//semihosting init function:
extern void initialize_monitor_handles(void);

int main(void) {
	//initialize typedef values
	again_t _AGAIN; //ALS gain
	atime_t _ATIME; //integration time

	//edit gain and integration times typedefs
	_AGAIN = again_low; //low is for high light levels, maximum is for lowest light levels
	_ATIME = atime_100ms;

	//TSL2591 registers 
	uint32_t en_reg = 0xA0; //(7-5) COMMAND reg normal operation, (4-0) reg 0x00 
	//uint32_t id_reg = 0xB2; //(4-0) reg 0x12 
	uint32_t data_reg = 0xB4; //((4-0) reg 0x14

	uint32_t raw_data = 0; //var holds raw tsl2591 data
	float lux_data = 0; //var holds tsl2591 data in units of lux

	int i = 0; //index var

	//TSL2591 init message: (15-13) COMMAND reg normal operation, (12-8) reg 0x00, (7-0) data 0x03
	uint32_t init_msg = (en_reg << 8) | (0x03);
	
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
	
	//change ALS gain and integration time
	tsl2591_write_settings(_AGAIN, _ATIME);

	//check device id
	//i2c2_write_read(1, &id_reg, &raw_data);
	//printf("TSL2591 Raw Data: %lx\n", raw_data);
	
	while(1) {
		//poll every 5 seconds: maybe use timer in the future
		for (i = 0; i < 24000000; i++); 

		i2c2_write_read(4, &data_reg, &raw_data); //collect data from TSL2591
		lux_data = rawdata_to_lux(raw_data, _AGAIN, _ATIME); //convert raw data to lux

		printf("LUX = %.2f\n", lux_data); //print out light data
	}

	return 0;
}

