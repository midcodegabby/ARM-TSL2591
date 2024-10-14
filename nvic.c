/*
 * nvic.c
 *
 *  Author: Gabriel Rodgers
 *  Date: 10/13/2024
 *  Purpose: To control interrupts through the NVIC;
 *  this file only effects interrupts 40 (Button), 33 (I2C2_EV), 34 (I2C2_ER)
 */


#include <stdint.h>
#include "nvic.h"

#define NVIC_ISER 0xE000E100
#define NVIC_ICER 0xE000E180
#define NVIC_IPR 0xE000E400

#define NVIC_ISER1 (*((volatile uint32_t *) (NVIC_ISER + 0x04)))
#define NVIC_ICER1 (*((volatile uint32_t *) (NVIC_ICER + 0x04)))
#define NVIC_IPR10 (*((volatile uint32_t *) (NVIC_IPR + 0x28)))
#define NVIC_IPR13 (*((volatile uint32_t *) (NVIC_IPR + 0x34)))

//enable NVIC
void nvic_enable(void) {
        NVIC_ISER1 |= (1 << 8); //enable interrupt 40 (user button)
}

//disable NVIC
void nvic_disable(void) {
        NVIC_ICER1 |= (1 << 8); //disable interrupt 40 (user button)
}

//change priorities of nvic
void nvic_priority(void) {
	//change priority of button interrupt (40) to lowest
	NVIC_IPR10 |= (0xF << 4); 

}

