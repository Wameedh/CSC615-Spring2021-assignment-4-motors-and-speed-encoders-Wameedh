/**************************************************************
* Class: CSC-615-01 Spring 2020
* Name: Wameedh Mohammed Ali
* Student ID: 920678405
* Github ID: wameedh
* Project: Assignment 4 - Motors & Speed Encoder
*
* File: motorControler.h
*
* Description: This is the header file for motorControler.c file that was imported from assignment 3.
**************************************************************/

#ifndef MOTORCONTROLER_H_
#define MOTORCONTROLER_H_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>



#define HIGH    1
#define LOW     0

#define MODE1   0x00

//GPIO config
#define PWMA       0
#define AIN1       1
#define AIN2       2

#define LED0_ON_L           0x06
#define LED0_ON_H           0x07
#define LED0_OFF_L          0x08
#define LED0_OFF_H          0x09

#define UBYTE   uint8_t
#define UWORD   uint16_t
#define PRESCALE    0xFE


typedef enum {
    FORWARD  = 1,
    BACKWARD  ,
} DIR;


void Motor_Stop();
void PCA9685_SetPWM(UBYTE channel, UWORD on, UWORD off);
void PCA9685_SetPWMFreq(UWORD freq);
void runTheMotor(DIR dir, UWORD speed);
void writeI2C(UBYTE reg, UBYTE value);
UBYTE readI2C(UBYTE reg);

#endif //MOTORCONTROLER_H_