/**************************************************************
* Class: CSC-615-01 Spring 2020
* Name: Wameedh Mohammed Ali
* Student ID: 920678405
* Github ID: wameedh
* Project: Assignment 4 - Motors & Speed Encoder
*
* File: motorControler.h
*
* Description: This file was imported from assignment 3. It controles the motor.
* The motor should stop, go forward, backward,
* as well as speed control (Pulse Width Modulation).
*
**************************************************************/

#include "motorController.h"

void runTheMotor(DIR dir, UWORD speed)
{
    if(speed > 100) // max speed is 100
        speed = 100;
        PCA9685_SetPWM(PWMA, 0, speed * (4096 / 100) - 1);
        if(dir == FORWARD) {
            PCA9685_SetPWM(AIN1,0, 4095);
            PCA9685_SetPWM(AIN2,0, 0);
           
        } else {
            PCA9685_SetPWM(AIN1,0, 0);
            PCA9685_SetPWM(AIN2,0, 4095);
        }
}

void writeI2C(UBYTE reg, UBYTE value){
    if(wiringPiI2CWriteReg8(fd, (int)reg, (int)value) != 0){
            // failed
            printf("wiringPiI2CWriteReg8() has failed!\n");
        }
}

UBYTE readI2C(UBYTE reg){
    return  wiringPiI2CReadReg8(fd, reg);
}

void Motor_Stop()
{
    PCA9685_SetPWM(PWMA, 0, 0 * (4096 / 100) - 1);
}

/**
 * Some of the code and logic in the Below function was sourced from
 * https://www.waveshare.com/wiki/File:Motor_Driver_HAT_Code.7z
 * Set the PWM output.
 * @param channel: 16 output channels.  //(0 ~ 15)
 * @param on: 12-bit register will hold avalue for the ON time.  //(0 ~ 4095)
 * @param off: 12-bit register will hold the value for the OFF time.  //(0 ~ 4095)
 *
 * @For more information, please see page 15 - page 19 of the datasheet.
 * Example:
 * PCA9685_SetPWM(0, 0, 4095);
 */
void PCA9685_SetPWM(UBYTE channel, UWORD on, UWORD off)
{
    writeI2C(LED0_ON_L + 4*channel, on & 0xFF);
    writeI2C(LED0_ON_H + 4*channel, on >> 8);
    writeI2C(LED0_OFF_L + 4*channel, off & 0xFF);
    writeI2C(LED0_OFF_H + 4*channel, off >> 8);
}

/**
 * Some of the code and logic in the Below function was sourced from
 * https://www.waveshare.com/wiki/File:Motor_Driver_HAT_Code.7z
 * Set the frequency (PWM_PRESCALE) and restart.
 * 
 * For the PCA9685, Each channel output has its own 12-bit 
 * resolution (4096 steps) fixed frequency individual PWM 
 * controller that operates at a programmable frequency 
 * from a typical of 40 Hz to 1000 Hz with a duty cycle 
 * that is adjustable from 0 % to 100 % 
 * 
 * @param freq: Output frequency.  //40 ~ 1000
 *
 * Example:
 * PCA9685_SetPWMFreq(50);
 */
void PCA9685_SetPWMFreq(UWORD freq)
{
    freq *= 0.9;  // Correct for overshoot in the frequency setting (see issue #11).
    double prescaleval = 25000000.0;
    prescaleval /= 4096.0;
    prescaleval /= freq;
    prescaleval -= 1;

    UBYTE prescale = floor(prescaleval + 0.5);

    UBYTE oldmode = readI2C(MODE1);
    UBYTE newmode = (oldmode & 0x7F) | 0x10; // sleep
    writeI2C(MODE1, newmode);
    writeI2C(PRESCALE, prescale);
    writeI2C(MODE1, oldmode);
    delay(5);
    writeI2C(MODE1, oldmode | 0x80);  //  This sets the MODE1 register to turn on auto increment.
}
