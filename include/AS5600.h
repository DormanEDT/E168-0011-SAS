
/*
 * AS5600.h
 *
 *  Created on: 10-Jun-2022
 *      Author: Admin
 */

#ifndef AS5600_H_
#define AS5600_H_


#define M_AS5600  			(0x36)	/* AS5600 - Magnetic Angle sensor Device Address*/
#define M_AS5600_STATUS  	(0x0B)	/* AS5600 - Magnetic Angle sensor Status register address*/
#define M_AS5600_RA_MSB  	(0x0C)  /* AS5600 - Magnetic Angle sensor raw angle MSB register address*/
#define M_AS5600_RA_LSB 	(0x0D)  /* AS5600 - Magnetic Angle sensor raw angle LSB register address*/

#define M_AS5600_STATUS_MD  (0x20)  /* AS5600 - Magnetic Angle sensor status magnet detect */


// Fo the MUX PCA9540BDP

#define M_PCA9540B   (0x70) // (0xE0)
#define M_PCA9540B_MUX_CHANNEL0   (0x04)
#define M_PCA9540B_MUX_CHANNEL1   (0x05)

#endif //AS5600_H_
