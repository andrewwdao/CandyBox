/*------------------------------------------------------------*-
  Stepper Motor Controller - header file
  RASPBERRY PI 3B+
  (c) Minh-An Dao 2019
  version 1.00 - 24/09/2019
 --------------------------------------------------------------
 * This code use the TMC2208 as the driver for the stepper.
 * Other Driver (A4988, DRV8825) should work fine too.
 * You just need to calibrate the MIN_DELAY(us) and MAX_DELAY(us)
 * in Micro_Stepper.cpp to fit those driver.
 -------------------------------------------------------------- */
#ifndef  __RASP_STEPPER_CPP 
#define  __RASP_STEPPER_CPP
#include "Rasp_Stepper.h"

// ------ Private constants -----------------------------------
#define EN_PIN           0 // Enable - GPIO_GEN_0 - GPIO17 - PIN 11
#define DIR_PIN          1 // Direction - GPIO_GEN_1 - GPIO18 - PIN 12
#define STEP_PIN         2 // Step - GPIO_GEN_2 - GPIO27 - PIN 13

#define MIN_SPEED     0   //%
#define MAX_SPEED     100 //%
#define MIN_DELAY 95
#define MAX_DELAY 5000

#define PULSE_PER_ROT 3200

#define HIGH      1
#define LOW       0
#define CW        HIGH
#define CCW       LOW
#define true      1
#define false     0
// ------ Private function prototypes -------------------------
/**
Initialize the Stepper motor
*/
void stepper_init();
/**
Map function declaration
*/
long map(long x, long in_min, long in_max, long out_min, long out_max);
// ------ Private variables -----------------------------------
volatile unsigned int  stepperSpeed; //already converted value
volatile unsigned int  numOfPulse;
volatile float         numOfRot;
// ------ PUBLIC variable definitions -------------------------

//--------------------------------------------------------------
// FUNCTION DEFINITIONS
//--------------------------------------------------------------
//--------------------------------------------------------------
// FUNCTION DEFINITIONS
//--------------------------------------------------------------
void stepper_init() {
  if(wiringPiSetup() == -1){ //when initialize wiring failed,print message to the screen
		printf("setup wiringPi failed !");
		return; 
	}//end if
	pinMode(EN_PIN, OUTPUT);
  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  digitalWrite(EN_PIN,LOW); //always turn on driver --> holding torque
  digitalWrite(DIR_PIN, DEFAULT_DIR);
  numOfPulse = 0;
}//end stepper_init
//--------------------------------
void stepper_run(float rot,unsigned int speed) {
  stepper_init();
  numOfRot = rot;             //default =0, if  >0, motor will rotate
  stepperSpeed = map(speed,MIN_SPEED,MAX_SPEED,MAX_DELAY,MIN_DELAY);
  
  while (++numOfPulse<PULSE_PER_ROT*numOfRot) {
    //printf("Num of rotation left: %f",(numOfRot-(float)numOfPulse/PULSE_PER_ROT));
    digitalWrite(STEP_PIN, !digitalRead(STEP_PIN));
    delayMicroseconds(stepperSpeed);
  }//end while
  #ifndef HOLD_TORQUE
    digitalWrite(EN_PIN, HIGH);// disable driver in hardware
  #endif
  return;
} //end stepper_run
//--------------------------------
long map(long x, long in_min, long in_max, long out_min, long out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}//end map
//--------------------------------
#endif //__RASP_STEPPER_CPP
