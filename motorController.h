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
#include <pthread.h>


#define PI                    3.14159265358979323846  //the value of pi
#define NUMBER_OF_MOTORS      2                       //This defines the number of motors being used.
#define PULSES_PER_ROTATION   20                      //The number of holes in the wheel encoder. The speed sensor reads each hole as a pulse.
#define TIME_TO_MEASURE       1.0  
#define ENCODER_DIAMETER      2.575                     //in centimeters.

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

// Pin number declarations. We're using the Broadcom chip pin numbers.
#define BUTTON_PIN              17              // Broadcom pin# 6 GPIO17, P1 pin 11
#define SPEED_SENSOR_PIN        27              //pin 5(GPIO 27)


int speedSensorThreadFlag = 1;
int fd;
volatile double power = 0.0;

void Motor_Stop();
void PCA9685_SetPWM(UBYTE channel, UWORD on, UWORD off);
void PCA9685_SetPWMFreq(UWORD freq);
void runTheMotor(DIR dir, UWORD speed);
void writeI2C(UBYTE reg, UBYTE value);
UBYTE readI2C(UBYTE reg);

//reads the pulses that the speed sensor sees. It returns the pulses that were counted.
int readPulses(double time);

//calculates and returns angular speed using formula omega = (2*PI*NumberOfPulses) / (Time * PulsesPerRotation). Speed in radians per second.
double calculateAngularSpeed(int totalPulses, double time);

//calculates and returns linear speed using formula v = ωr. Speed in meters per second.
double calculateSpeed(double diameter, double angularSpeed);

//Function to be used in threads to display the speed read by the speed sensor.
void *useSpeedSensor(void *ptr);

#endif //MOTORCONTROLER_H_