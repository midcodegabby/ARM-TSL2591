This project is a from-scratch implementation of a TLS2591 light sensor driver. 
My main goal for this project was to learn about the I2C communication protocol by
implementing it from scratch between an STM32 Cortex-M4 mcu and a TSL2591 light sensor.

Things learned:
- I2C communication protocol: implementation, use, and electrical characteristics
- Configuring clocks and gpio for I2C communication
- Enabling and using the FPU in ARM Cortex-M4 microcontrollers
- Debugging I2C via Logic Analyzer

Status:
- In-Progress
- Operational
- Accuracy is low (30% to 50% off compared to Photone iPhone app measurements)

Next steps:
- Function to change gain and integration time
- Switch from ~5s busy loop polling to interrupt polling
- Improve accuracy using different gains and integration times

Possible future implementations:
- Function to reset I2C bus from a deadlocked state

Youtube video link (testing):
https://www.youtube.com/shorts/LOIbflBZAfY
