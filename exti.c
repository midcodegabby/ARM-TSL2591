/*
 * exti.c
 *
 *  Author: Gabriel Rodgers
 *  Date: 10/13/2024
 *  Purpose: to enable interrupts and handle some IRQs
 */

#include <stdint.h>
#include <stdio.h>

#include "exti.h"
#include "clock.h"
#include "nvic.h"
#include "gpio.h"
#include "systick.h"
#include "main.h"

#define EXTI 0x40010400
#define SYSCFG 0x40010000

#define EXTI_IMR1 (*((volatile uint32_t *) EXTI))
#define EXTI_EMR1 (*((volatile uint32_t *) (EXTI + 0x04)))
#define EXTI_PR1 (*((volatile uint32_t *) (EXTI + 0x14)))
#define EXTI_RTSR1 (*((volatile uint32_t *) (EXTI + 0x08)))
#define EXTI_FTSR1 (*((volatile uint32_t *) (EXTI + 0x0C)))
#define SYSCFG_EXTICR4 (*((volatile uint32_t *) (SYSCFG + 0x14)))

//enable interrupts
void exti_init(void) {

        //enable falling edge trigger for line 13
        EXTI_FTSR1 |= (1 << 13);

        //disable rising edge trigger for line 13
        EXTI_RTSR1 &= ~(1 << 13);

        //choose PC13 to be the event trigger for EXTI13
        SYSCFG_EXTICR4 |= (1 << 5);
}

void exti_enable(void) {
        //enable interrupts for line 13, corresponding to the user button
        EXTI_IMR1 |= (1 << 13);
}

void exti_disable(void) {
        //disable interrupts for line 13, corresponding to the user button
        EXTI_IMR1 &= ~(1 << 13);
}

//IRQ handler for button push interrupt; this either starts the game or changes the hand
void EXTI15_10_IRQHandler(void) {
	//disable interrupts
        nvic_disable(); 
	
	gpio_led_toggle();

        //clear any pending interrupts and re-enable interrupts
        EXTI_PR1 |= (1 << 13);
	nvic_enable();
}

