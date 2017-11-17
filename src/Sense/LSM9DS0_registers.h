/*
 * LSM9DS0_registers.h
 *
 *  Created on: 04.11.2017
 *      Author: Hermann
 */

#ifndef LSM9DS0_REGISTERS_H_
#define LSM9DS0_REGISTERS_H_


#define LSM9DS0_XM_ID 					0b01001001
#define LSM9DS0_G_ID 					0b11010100

#define READ							0x80
#define INCREMENT						0x40

// Gyro register
#define LSM9DS0_WHO_AM_I_G				0x0F
#define LSM9DS0_CTRL_REG1_G 			0x20
#define LSM9DS0_CTRL_REG2_G 			0x21
#define LSM9DS0_CTRL_REG3_G 			0x22
#define LSM9DS0_CTRL_REG4_G 			0x23
#define LSM9DS0_CTRL_REG5_G 			0x24
#define LSM9DS0_OUT_X_L_G				0x28
#define LSM9DS0_OUT_X_H_G				0x29
#define LSM9DS0_OUT_Y_L_G				0x2A
#define LSM9DS0_OUT_Y_H_G				0x2B
#define LSM9DS0_OUT_Z_L_G				0x2C
#define LSM9DS0_OUT_Z_H_G				0x2D

// Accelerometer and Magnetometer register
#define LSM9DS0_OUT_TEMP_L_XM			0x05
#define LSM9DS0_OUT_TEMP_H_XM			0x06
#define LSM9DS0_STATUS_REG_M			0x07
#define LSM9DS0_OUT_X_L_M				0x08
#define LSM9DS0_OUT_X_H_M				0x09
#define LSM9DS0_OUT_Y_L_M				0x0A
#define LSM9DS0_OUT_Y_H_M				0x0B
#define LSM9DS0_OUT_Z_L_M				0x0C
#define LSM9DS0_OUT_Z_H_M				0x0D
#define LSM9DS0_WHO_AM_I_XM 			0x0F
#define LSM9DS0_INT_CTRL_REG_M		  	0x12
#define LSM9DS0_INT_SRC_REG_M			0x13
#define LSM9DS0_CTRL_REG1_XM			0x20
#define LSM9DS0_CTRL_REG2_XM			0x21
#define LSM9DS0_CTRL_REG3_XM			0x22
#define LSM9DS0_CTRL_REG4_XM			0x23
#define LSM9DS0_CTRL_REG5_XM			0x24
#define LSM9DS0_CTRL_REG6_XM			0x25
#define LSM9DS0_CTRL_REG7_XM			0x26
#define LSM9DS0_OUT_X_L_A				0x28
#define LSM9DS0_OUT_X_H_A				0x29
#define LSM9DS0_OUT_Y_L_A				0x2A
#define LSM9DS0_OUT_Y_H_A				0x2B
#define LSM9DS0_OUT_Z_L_A				0x2C
#define LSM9DS0_OUT_Z_H_A				0x2D

//Acclerometer ODR
#define LSM9DS0_XM_ODR_100HZ			0x67
#define LSM9DS0_XM_ODR_200HZ			0x77
#define LSM9DS0_XM_ODR_400HZ			0x87
#define LSM9DS0_XM_ODR_800HZ			0x97
#define LSM9DS0_XM_ODR_1600HZ			0xA7

//Gyroscope ODR
#define LSM9DS0_G_ODR_95HZ				0x0F
#define LSM9DS0_G_ODR_190HZ				0x4F

// Acceleration LSB
#define LSM9DS0_ACCEL_MG_LSB_2G 		0.061F
#define LSM9DS0_ACCEL_MG_LSB_4G 		0.122F
#define LSM9DS0_ACCEL_MG_LSB_6G 		0.183F
#define LSM9DS0_ACCEL_MG_LSB_8G 		0.244F

// Magnetic Field Strength gauss range
#define LSM9DS0_MAG_MGAUSS_2GAUSS 		0.08F
#define LSM9DS0_MAG_MGAUSS_4GAUSS 		0.16F
#define LSM9DS0_MAG_MGAUSS_8GAUSS 		0.32F
#define LSM9DS0_MAG_MGAUSS_12GAUSS 		0.48F

// Angular Rate LSB
// Angular Rate: dps per LSB
#define LSM9DS0_GYRO_DPS_DIGIT_245DPS 	0.00875F
#define LSM9DS0_GYRO_DPS_DIGIT_500DPS 	0.01750F
#define LSM9DS0_GYRO_DPS_DIGIT_2000DPS 	0.07000F

// Temperature LSB
#define LSM9DS0_TEMP_LSB 				8


#endif /* LSM9DS0_REGISTERS_H_ */
