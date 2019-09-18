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
#ifndef  __MICRO_STEPPER_CPP 
#define  __MICRO_STEPPER_CPP
#include "Micro_Stepper.h"

// ------ Private constants -----------------------------------
#define EN_PIN           2 // Enable
#define DIR_PIN          4 // Direction
#define STEP_PIN         3 // Step
#define DEFAULT_SPEED 70  //%
#define MIN_SPEED     0   //%
#define MAX_SPEED     100 //%
#define MIN_DELAY 80
#define MAX_DELAY 5000

#define PULSE_PER_ROT 3200
// ------ Private function prototypes -------------------------

// ------ Private variables -----------------------------------
uint16_t  stepperSpeed; //already converted value
bool      stepperIsRunning;
bool      stepperDIR;
uint32_t  lastMicros;
uint32_t  numOfPulse;
float     numOfRot;
// ------ PUBLIC variable definitions -------------------------

//--------------------------------------------------------------
// FUNCTION DEFINITIONS
//--------------------------------------------------------------
void stepper_init() 
{
  stepperSpeed = map(DEFAULT_SPEED,MIN_SPEED,MAX_SPEED,MAX_DELAY,MIN_DELAY);
  stepperIsRunning = false;
  stepperDIR = CW;
	pinMode(EN_PIN, OUTPUT);
  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  #ifdef HOLD_TORQUE
    digitalWrite(EN_PIN,LOW); //always turn on driver --> holding torque
  #else
    digitalWrite(EN_PIN, HIGH);// Disable driver in hardware, only turn on when "Go!" or "F|x" command is called
  #endif
  numOfPulse = 0;
  numOfRot = 0;             //default =0, if  >0, motor will rotate
  lastMicros = micros();
}//end stepper_init
//--------------------------------
void stepper_routine() {
  if (stepperIsRunning) {
    if ((micros()-lastMicros)>stepperSpeed) {
      lastMicros = micros();
      digitalWrite(DIR_PIN, stepperDIR);
      digitalWrite(STEP_PIN, !digitalRead(STEP_PIN));
    }//end if
  }//end if
  if (numOfPulse<PULSE_PER_ROT*numOfRot) {
    if ((micros()-lastMicros)>stepperSpeed) {
      lastMicros = micros();
      digitalWrite(DIR_PIN, stepperDIR);
      digitalWrite(STEP_PIN, !digitalRead(STEP_PIN));
      if (++numOfPulse>=PULSE_PER_ROT*numOfRot) {//stop rotation
        numOfPulse =0; numOfRot = 0;
        #ifndef HOLD_TORQUE
          digitalWrite(EN_PIN, HIGH);// disable driver in hardware
        #endif
      }//end if
    }//end if
  }//end if
}//end stepper_routine
//--------------------------------
void stepper_changeSpeed(int speedVal) {
  if (speedVal>MAX_SPEED) {speedVal = MAX_SPEED;}
  if (speedVal<MIN_SPEED) {speedVal = MIN_SPEED;}
  stepperSpeed = map(speedVal,MIN_SPEED,MAX_SPEED,MAX_DELAY,MIN_DELAY);
}//end stepper_changeSpeed
//--------------------------------
int stepper_readSpeed() {
  return map(stepperSpeed,MAX_DELAY,MIN_DELAY,MIN_SPEED,MAX_SPEED);
}//end stepper_readSpeed
//--------------------------------
void stepper_faster(int Spercent) {
  int percentSpeed = map(stepperSpeed,MAX_DELAY,MIN_DELAY,MIN_SPEED,MAX_SPEED); //convert it to 0-100%
  percentSpeed+=Spercent;
  if (percentSpeed>MAX_SPEED) {percentSpeed = MAX_SPEED;}
  if (percentSpeed<MIN_SPEED) {percentSpeed = MIN_SPEED;}
  stepperSpeed = map(percentSpeed,MIN_SPEED,MAX_SPEED,MAX_DELAY,MIN_DELAY); //convert it back to microsecond
}//end stepper_faster
//--------------------------------
void stepper_slower(int Spercent) {
  int percentSpeed = map(stepperSpeed,MAX_DELAY,MIN_DELAY,MIN_SPEED,MAX_SPEED); //convert it to 0-100%
  percentSpeed-=Spercent;
  if (percentSpeed>MAX_SPEED) {percentSpeed = MAX_SPEED;}
  if (percentSpeed<MIN_SPEED) {percentSpeed = MIN_SPEED;}
  stepperSpeed = map(percentSpeed,MIN_SPEED,MAX_SPEED,MAX_DELAY,MIN_DELAY); //convert it back to microsecond
}//end stepper_slower
//--------------------------------
void stepper_dir(bool sDIR) {
  stepperDIR = sDIR;
}//end stepper_run
//--------------------------------
void stepper_run() {
  stepperIsRunning = true;
  #ifndef HOLD_TORQUE
    digitalWrite(EN_PIN, LOW);// enable driver in hardware
  #endif
}//end stepper_run
//--------------------------------
void stepper_stop() {
  stepperIsRunning = false;
  #ifndef HOLD_TORQUE
    digitalWrite(EN_PIN, HIGH);// disable driver in hardware
  #endif
}//end stepper_stop
//--------------------------------
void stepper_turn(float Rots) {
  numOfRot = Rots;
  #ifndef HOLD_TORQUE
    digitalWrite(EN_PIN, LOW);// enable driver in hardware
  #endif
}//end stepper_turn
//--------------------------------
#endif //__MICRO_STEPPER_CPP
