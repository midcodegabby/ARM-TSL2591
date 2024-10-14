/*
 * gpio.h
 *
 *  Author: Gabriel Rodgers
 *  Date: 10/13/2024
 */

#ifndef INC_GPIO_H_
#define INC_GPIO_H_

void gpio_i2c2_init(void);
uint8_t gpio_i2c2_bus_poll(void);
void gpio_button_init(void);
uint8_t gpio_button_poll(uint8_t);
void gpio_led_init(void);
void gpio_led_on(void);
void gpio_led_off(void);
void gpio_led_toggle(void);


#endif /* INC_GPIO_H_ */
