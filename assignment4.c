/**************************************************************
* Class: CSC-615-01 Spring 2020
* Name: Wameedh Mohammed Ali
* Student ID: 920678405
* Github ID: wameedh
* Project: Assignment 4 - Motors & Speed Encoder
*
* File: assignment4.c
*
* Description: 
*
**************************************************************/
#include "motorController.h"


int main(void) {
     pthread_t sensorThread_id;

  if(wiringPiSetupGpio() == -1) { 
        exit(0); // setup wiringPi failed.
    }

    pinMode(BUTTON_PIN, INPUT);
    //Get the fd
    fd = wiringPiI2CSetup(0x40);
    // write 0 to the fd
    writeI2C(MODE1, 0x00);
    // set the freq 
    PCA9685_SetPWMFreq(100);

    pthread_create(&sensorThread_id, NULL, useSpeedSensor,NULL);

    printf("Press Button to start...\n");
    while (digitalRead(BUTTON_PIN) == LOW)
    {
    //  The program is listening for the button to be pressed,
    //  then it would exit this while loop and execute the rest of the instructions.
    }

        /**
         * forward for about 5 seconds, then slow down to 15% gradually, then
         * stop the motor for at least one second, then start the motor slowly and
         * gradually increase speed to max but in reverse
        */
    printf("Motor running forword!\n");
    runTheMotor(FORWARD, 100); // run the motor forward at max speed
    printOutInfo()
    delay(5000); // five seconds delay
    printf("Motor started slowing down to 15%\n");
    for (size_t i = 100; i >= 15 ; i--)
    {
        // gradually decrease speed to 15%
        runTheMotor(FORWARD, i);
        printOutInfo()
    }
    delay(3000); // three second delay
    Motor_Stop();
    printf("Motor has stoped fully!\n");
    printOutInfo()
    delay(2000); // two seconds delay
    printf("Motor started running backward and gradually increase speed to max\n");
     for (size_t i = 0; i <= 100 ; i++)
    {
        // gradually increase speed to max
        runTheMotor(BACKWARD, i);
        printOutInfo()
    }
    delay(5000); // five seconds delay
    Motor_Stop();
    
    speedSensorThreadFlag = 0;
    pthread_join( sensorThread_id, NULL);

    printf("Motor has stoped!\n");
    printf("End of program!\n");
    printOutInfo()

    return 0;
}


void runTheMotor(DIR dir, UWORD speed)
{
    if(speed > 100) // max speed is 100
        speed = 100;
        power = speed * (4096 / 100) - 1;
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


void *useSpeedSensor(void *ptr) {
    while (speedSensorThreadFlag)
    {
        // delay(1000); // print out the spped every one second
        double aSpeed = calculateAngularSpeed(readPulses(TIME_TO_MEASURE), TIME_TO_MEASURE);
        double speed = calculateSpeed(ENCODER_DIAMETER, aSpeed);
        // printf("Power (PWM) being applied is: %f\n", power);
        // printf("The angular speed is: %f rad/sec\n", aSpeed);
        // printf("The linear speed is: %f cm/sec\n\n", speed);
    }
    return NULL;
}

double calculateAngularSpeed(int totalPulses, double time) {
    return (2.0 * PI * totalPulses) / (time * PULSES_PER_ROTATION);
}

double calculateSpeed(double diameter, double angularSpeed) {
    return (diameter / 2) * angularSpeed;
}

int readPulses(double time) {
    int count = 0;
    double start = millis();
    double end = start + (1000 * time);
    // printf("start: %f, end: %f\n", start, end);

    while (end > millis()) {
        if(digitalRead(SPEED_SENSOR_PIN)) {
            count++;
            while(digitalRead(SPEED_SENSOR_PIN)){}
        }
    }
    // printf("Total count: %d\n", count);

    return count;
}

void printOutInfo(){
    printf("Power (PWM) being applied is: %f\n", power);
    printf("The angular speed is: %f rad/sec\n", aSpeed);
    printf("The linear speed is: %f cm/sec\n\n", speed);
}