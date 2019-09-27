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
#define DEFAULT_SPEED 100  //%
#define MIN_SPEED     0   //%
#define MAX_SPEED     100 //%
#define MIN_DELAY 95
#define MAX_DELAY 5000

#define PULSE_PER_ROT 3200
// ------ Private function prototypes -------------------------

// ------ Private variables -----------------------------------
volatile uint16_t  stepperSpeed; //already converted value
volatile bool      stepperIsRunning;
volatile bool      stepperDIR;
volatile uint32_t  numOfPulse;
volatile float     numOfRot;
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
  //lastMicros = micros();
  Timer1.initialize(stepperSpeed); //us
  Timer1.attachInterrupt(stepper_routine);

}//end stepper_init
//--------------------------------
void stepper_activate() {
  numOfRot=0; numOfPulse=0;
  Timer1.attachInterrupt(stepper_routine);
}//end stepper_activate
//--------------------------------
void stepper_deactivate() {
  numOfRot=0; numOfPulse=0;
  Timer1.detachInterrupt();
}//end stepper_deactivate
//--------------------------------
void stepper_routine() {
  if (stepperIsRunning) {
    digitalWrite(DIR_PIN, stepperDIR);
    digitalWrite(STEP_PIN, !digitalRead(STEP_PIN));
    numOfPulse =0; numOfRot = 0; //make sure the motor won't receive the F| command during Go! and Pause!
  } else {
    if (numOfPulse<PULSE_PER_ROT*numOfRot) {
      digitalWrite(DIR_PIN, stepperDIR);
      digitalWrite(STEP_PIN, !digitalRead(STEP_PIN));
      if (++numOfPulse>=PULSE_PER_ROT*numOfRot) {//stop rotation
        numOfPulse =0; numOfRot = 0;
        #ifndef HOLD_TORQUE
          digitalWrite(EN_PIN, HIGH);// disable driver in hardware
        #endif
      }//end if
    }//end if
  }//end if else
}//end stepper_routine
//--------------------------------
void stepper_changeSpeed(int speedVal) {
  if (speedVal>MAX_SPEED) {speedVal = MAX_SPEED;}
  if (speedVal<MIN_SPEED) {speedVal = MIN_SPEED;}
  stepperSpeed = map(speedVal,MIN_SPEED,MAX_SPEED,MAX_DELAY,MIN_DELAY);
  Timer1.initialize(stepperSpeed); //us
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
  Timer1.initialize(stepperSpeed); //us
}//end stepper_faster
//--------------------------------
void stepper_slower(int Spercent) {
  int percentSpeed = map(stepperSpeed,MAX_DELAY,MIN_DELAY,MIN_SPEED,MAX_SPEED); //convert it to 0-100%
  percentSpeed-=Spercent;
  if (percentSpeed>MAX_SPEED) {percentSpeed = MAX_SPEED;}
  if (percentSpeed<MIN_SPEED) {percentSpeed = MIN_SPEED;}
  stepperSpeed = map(percentSpeed,MIN_SPEED,MAX_SPEED,MAX_DELAY,MIN_DELAY); //convert it back to microsecond
  Timer1.initialize(stepperSpeed); //us
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
  if (numOfRot==0) { //no interfere with rotation if F| command is active
    #ifndef HOLD_TORQUE
      digitalWrite(EN_PIN, HIGH);// disable driver in hardware
    #endif
  }//end if
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
