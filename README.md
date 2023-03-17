## **Universal Searchlights SLASS RCU Firmware**
## **searchlight-1.0.1-slass**
**Author: Jon Wilder**<br />
**Project Created on February 24th, 2020**<br />

---

## **Disclaimer**
Copyright of this code and all other associated source code modules (hereinafter referred to as "source code") is retained by 
SohCahToa Embedded Solutions. SohCahToa Embedded Solutions retains the sole right to copy, use, and distribute this project, 
all associated source code, and any and all production files generated herein by the source code.

Use rights of the production files are solely retained by APR Aviation and Fresno Police Department. APR Aviation and Fresno 
Police Department retain the right to use and distribute all production files generated by the source code, but not the source 
code itself.

---

## **Hardware Information**

**Processor Type:** Microchip Atmel ATtiny26<br />

**PWM Outputs -**<br />
**Pin Assignment (DOWN):** PB1/OC1A<br />
**Pin Assignment (LEFT):** PB2<br />
**Pin Assignment (RIGHT):** PB3/OC1B<br />
**Pin Assignment (UP):** PB6<br />
**PWM Module Assignment (Y Axis):** OC1A<br />
**PWM Module Assignment (X Axis):** OC1B<br />
**Max Duty Cycle:** 39.02% Measured<br />
**PWM Frequency:** 20.3Hz Measured<br />
**PWM Period:** 49.2mS Measured<br />

**ADC Inputs (Joystick) -**<br />
**Pin Assignment (Y AXIS):** PA0/ADC0<br />
**Pin Assignment (X Axis):** PA1/ADC1<br />

**On Board LED -**<br />
**Pin Assignment:** PB5<br />

---

## **Release Notes**

**v1.0.1** - This is the first version of 3rd party SLASS RCU software.

---

## **PWM Information**

The ATtiny26 processor has two 8-bit PWM generators with shared period/frequency via Timer/Counter 1. Each generator uses 
separate compare registers for independent duty cycle control. OC1A provides the Y axis control signaling while OC1B provides 
the X axis control signaling.

In hardware, OC1A output is only available on pin PB1 while OC1B output is only available on pin PB3. These pins provide the 
DOWN and RIGHT movement signals respectively. Thus, it is necessary to synthesize the UP and LEFT movement signals onto two 
additional GPIO pins. As the controller board is designed, it was found to utilize GPIO pins PB2 and PB6 to provide the LEFT 
and UP signals respectively. The Timer 1 compare match interrupts, in conjunction with the Timer 1 overflow interrupt, provide 
the LEFT and UP signals on GPIO pins PB2 and PB6 respectively.

When the joystick is moved into the null zone, OC1A and OC1B hardware outputs are switched off via the COM1x1/COM1x0 bits in 
the Timer Counter1 Control Register TCCR1A. This hands control of PB1 and PB3 off to the port latch. As the PORTB register 
control bits for PB1 and PB3 are initialized low at boot time and remain in this state throughout code execution, this forces 
pins PB1 and PB3 low as soon as the OC1A/OC1B outputs are disconnected from PB1 and PB3. As the joystick is now in the null
range, the timer 1 overflow interrupt will no longer switch PB2 and PB6 high, leaving the compare match interrupts to pull 
them low automatically.

When the joystick is moved left and/or up, the Timer 1 Compare Match and Overflow interrupts assert control of pins PB2 and 
PB6 respectively. The compare match drives PB2/PB6 low while the overflow interrupt drives PB2/PB6 high. This matches the 
non-inverting phasing of the OC1A/OC1B pins.

When the joystick is moved down and/or right, pins PB2 and PB6 are forced low by the compare match interrupt while the timer 
1 overflow interrupt will not return them high as the joystick movement will be outside of the allowable range to act upon
PB2 and PB6. OC1A and OC1B outputs are again enabled via the COM1x1/COM1x2 bits in the Timer Counter Control Register TCCR1A. 
The PWM hardware assumes control and outputs the PWM signal on OC1A/OC1B. As the joystick position will no longer be in the 
up or the left range, the interrupts still run, but do not act upon PB2 and PB6 until the joystick position re-enters the up 
and/or left range.

As the original SLASS RCU was found to be limited to a maximum 39% duty cycle, this has been duplicated in this firmware 
release as well.

Upon movement of the pressure joystick in any direction, the associated direction output pins output a 20Hz PWM signal with 
a duty cycle ranging from 0-39%. These signals are fed from the SLASS RCU to the SLASS ECU. The SLASS ECU uses this information 
to drive the azimuth and elevation servos of the searchlight with variable speed and direction.

---

## **Joystick Information**

The function getJoystick() uses the Analog/Digital converter driver to sample both the X and Y axes of the joystick. The 
function performs 10 samples of each axis position at 1mS intervals, then averages the samples. The sample averages are 
stored in the global array "position", which is read by the main function as well as the Timer 1 compare match and Timer 
1 overflow interrupts.

Function setNull() runs at device startup. This function samples the joystick X and Y positions at boot time, then stores
the samples into a null array, which is read by the joystick position processing code and used in calculating the requested
PWM duty cycle. Each axis has an upper and a lower null limit. This allows the unit to always find the null position regard-
less of wear and tear on the pressure joystick.

---

**(C) Copyright 2020 SohCahToa Embedded Solutions. All rights reserved.**
