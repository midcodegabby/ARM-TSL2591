/*
 * i2c.h
 *
 *  Author: Gabriel Rodgers
 *  Date: 10/13/2024
 */

#ifndef INC_I2C_H_
#define INC_I2C_H_

void i2c2_init(void);
void i2c2_target_init(void);
void i2c2_write_read(uint32_t volatile *read_buffer);
uint8_t i2c2_check_bus(uint32_t count);
void i2c2_resolve_deadlock(void);
void I2C2_EV_IRQHandler(void);

#endif /* INC_I2C_H_ */
