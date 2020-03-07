/* SLASS RCU Joystick Driver
 * searchlight-1.0.1-slass
 * Author: Jon Wilder
 * Project Manager: Kevin Johnson - APR Aviation
 * Created Date: March 3rd, 2020
 * Use rights retained by Fresno Police Department and APR Aviation
 * (C) Copyright 2020 SohCahToa Embedded Solutions. All rights reserved.
 */ 


#ifndef JOYSTICK_H_
#define JOYSTICK_H_





#endif /* JOYSTICK_H_ */

#define _X_AXIS 1
#define _Y_AXIS 0

/* Null Range */
#define _X_NULL_TOP 0		// joystick X axis null zone upper limit
#define _X_NULL_BOT 1		// joystick X axis null zone lower limit
#define _Y_NULL_TOP 2		// joystick Y axis null zone upper limit
#define _Y_NULL_BOT 3		// joystick Y axis null zone lower limit

/* Global Variables & Arrays */
extern uint16_t position[2];	// joystick position data array
extern uint16_t null[4];

void setNull(void);

void getJoystick(void);

/* End of file */