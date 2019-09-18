/*------------------------------------------------------------*-
  Stepper Motor Controller - header file
  ARDUINO MICRO
  (c) Minh-An Dao 2019
  version 1.30 - 18/09/2019
 --------------------------------------------------------------
 * This code use the TMC2208 as the driver for the stepper.
 * Other Driver (A4988, DRV8825) should work fine too.
 * You just need to calibrate the MIN_DELAY(us) and MAX_DELAY(us)
 * in Micro_Stepper.cpp to fit those driver.
 -------------------------------------------------------------- */
#ifndef __MICRO_STEPPER_H
#define __MICRO_STEPPER_H
#include <Arduino.h>
#include <TMCStepper.h>
#include <TimerOne.h>
//#include "config.h"

//#define HOLD_TORQUE //Use this option if you want to make the rotor hold the torque even if it stands still


// ------ Public constants ------------------------------------
#define CW        HIGH
#define CCW       LOW
// ------ Public function prototypes --------------------------
/**
Initialize the Stepper motor
*/
void stepper_init();
/**
Rountine to put in the loop for the stepper motor
*/
void stepper_routine();
/**
Change the stepper motor speed, put in the parameter your speed (in percentage)
*/
void stepper_changeSpeed(int);
/**
Return the stepper motor speed (0 to 100%)
*/
int stepper_readSpeed();
/**
Make the stepper motor faster (in percentage)
*/
void stepper_faster(int);
/**
Make the stepper motor slower (in percentage)
*/
void stepper_slower(int);
/**
Define the stepper motor direction (CW or CCW)
*/
void stepper_dir(bool);
/**
Make the stepper motor runs
*/
void stepper_run();
/**
Make the stepper motor stop
*/
void stepper_stop();
/**
Turn the stepper in numbers of rotation that you want
*/
void stepper_turn(float);
// ------ Public variable -------------------------------------

#endif //__MICRO_STEPPER_H
