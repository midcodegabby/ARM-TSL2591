/*
 * exti.h
 *
 *  Author: Gabriel Rodgers
 *  Date: 10/13/2024
 */

#ifndef INC_EXTI_H_
#define INC_EXTI_H_

void exti_init(void);
void exti_enable(void);
void exti_disable(void);
void EXTI15_10_IRQHandler(void);

#endif 
