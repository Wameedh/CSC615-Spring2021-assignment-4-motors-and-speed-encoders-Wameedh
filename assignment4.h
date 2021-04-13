/**************************************************************
* Class: CSC-615-01 Spring 2020
* Name: Wameedh Mohammed Ali
* Student ID: 920678405
* Github ID: wameedh
* Project: Assignment 4 - Motors & Speed Encoder
*
* File: assignment4.h
*
* Description: This is the header file for assignment4.c
**************************************************************/

#ifndef MOTORCONTROLER_H_
#define MOTORCONTROLER_H_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <pthread.h>


#define PI                    3.1415    //the value of pi
#define NUMBER_OF_MOTORS      2         //This defines the number of motors being used.
#define PULSES_PER_ROTATION   20        //The number of holes in the wheel encoder. The speed sensor reads each hole as a pulse.
#define TIME_TO_MEASURE       1.50  
#define ENCODER_DIAMETER      2.575     //in centimeters.

#define HIGH    1
#define LOW     0

#define MODE1   0x00

//GPIO config
// Pin number declarations. We're using the Broadcom chip pin numbers.
#define BUTTON_PIN              17              // Broadcom pin# 6 GPIO17, P1 pin 11
#define SPEED_SENSOR_PIN        27              //pin 5(GPIO 27)
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


int speedSensorThreadFlag = 1; 
int fd;
volatile size_t i; // used in two for loops that would increase and decrease the speed of the motor gradually.

void Motor_Stop();
void PCA9685_SetPWM(UBYTE channel, UWORD on, UWORD off);
void PCA9685_SetPWMFreq(UWORD freq);
void runTheMotor(DIR dir, UWORD speed);
void writeI2C(UBYTE reg, UBYTE value);
UBYTE readI2C(UBYTE reg);
int pulseTiming(double time);
double calculateAngularSpeed(int totalPulses, double time);
double calculateSpeed(double diameter, double angularSpeed);
void *runSpeedSensor(void *ptr);

#endif //MOTORCONTROLER_H_