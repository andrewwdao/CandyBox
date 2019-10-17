/*------------------------------------------------------------*-
  Stepper Motor Controller - header file
  RASPBERRY PI 3B+
  (c) Minh-An Dao 2019
  version 1.00 - 24/09/2019
 --------------------------------------------------------------
 * This code use the TMC2208 as the driver for the stepper.
 * Other Driver (A4988, DRV8825) should work fine too.
 * You just need to calibrate the MIN_DELAY(us) and MAX_DELAY(us)
 * in Rasp_Stepper.cpp to fit those driver.
 -------------------------------------------------------------- */
#ifndef __RASP_STEPPER_H
#define __RASP_STEPPER_H
#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>     /* atof */

//#define HOLD_TORQUE //Use this option if you want to make the rotor hold the torque even if it stands still


// ------ Public constants ------------------------------------
#define DEFAULT_ROT   3
#define DEFAULT_SPEED 90  //%
#define DEFAULT_DIR   CW
// ------ Public function prototypes --------------------------
/**
Rountine to put in the loop for the stepper motor
*/
void stepper_run(float,unsigned int);

// ------ Public variable -------------------------------------

#endif //__RASP_STEPPER_H
