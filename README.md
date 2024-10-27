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
- Accuracy is high (90% - 95% accuracy compared to Photone iPhone app measurements)

Next steps:
- Switch from ~5s busy loop polling to interrupt polling

Possible future implementations:
- Function to reset I2C bus from a deadlocked state

Youtube video link (final test; 90% - 95% accuracy):
https://www.youtube.com/watch?v=m42UTxCefyY&t=5s

Youtube video link (first test; 30%-50% accuracy):
https://www.youtube.com/shorts/LOIbflBZAfY
