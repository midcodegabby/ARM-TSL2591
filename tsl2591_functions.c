/*
 * tsl2591_functions.c
 *
 *  Author: Gabriel Rodgers
 *  Date: 10/21/2024
 *  Purpose: Implement functions to operate on raw tsl2591 data
 */

//include files
#include <stdint.h>
#include <stdlib.h>

#include "tsl2591_functions.h"

//var definitions
#define TSL2591_LUX_DF 408.0F
#define TSL2591_LUX_C_CH0 1.64F 
#define TSL2591_LUX_C_CH1 0.59F


//return the light data in units of LUX
float rawdata_to_lux(uint32_t raw_data) {
	//raw_data is big-endian: high 2 bytes are CH1, low 2 bytes are CH0
	uint16_t ch0 = raw_data & 0xFFFF;
	uint16_t ch1 = (raw_data >> 16) & 0xFFFF;

	//gain and time floats
	float atime = 100.0F;
	float again = 1.0F;

	float cpl = (atime * again) / TSL2591_LUX_DF;

	//lux calculation
	float lux_data = (((float)ch0 - (float)ch1)) * (1.0F - ((float)ch1 / (float)ch0)) / cpl;

	return lux_data;
}
