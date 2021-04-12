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
#include "motorControler.h"
// #include "assignment4.h"


// Pin number declarations. We're using the Broadcom chip pin numbers.
const int buttonPin = 17; // PWM LED - Broadcom pin 17, P1 pin 11
int fd;

int main() {

  if(wiringPiSetupGpio() == -1) { 
        exit(0); // setup wiringPi failed.
    }

    pinMode(buttonPin, INPUT);
    //Get the fd
    fd = wiringPiI2CSetup(0x40);
    // write 0 to the fd
    writeI2C(MODE1, 0x00);
    // set the freq 
    PCA9685_SetPWMFreq(100);

    printf("Press Button to start...\n");
    while (digitalRead(buttonPin) == LOW)
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
    delay(5000); // five seconds delay
    printf("Motor started slowing down!\n");
    for (size_t i = 100; i >= 15 ; i--)
    {
        // gradually decrease speed to 15%
        runTheMotor(FORWARD, i);
    }
    delay(1000); // one second delay
    Motor_Stop();
    printf("Motor has stoped fully!\n");
    delay(2000); // two seconds delay
    printf("Motor started running backward!\n");
     for (size_t i = 0; i <= 100 ; i++)
    {
        // gradually increase speed to max
        runTheMotor(BACKWARD, i);
    }
    delay(5000); delay(5000); // five seconds delay
    Motor_Stop();
    printf("Motor has stoped!\n");
    printf("End of program!\n");
    return 0;
}
