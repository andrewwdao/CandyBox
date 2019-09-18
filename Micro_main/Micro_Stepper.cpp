/*------------------------------------------------------------*-
  Stepper Motor Controller - header file
  ARDUINO MICRO
  (c) Minh-An Dao 2019
  version 1.00 - 17/09/2019
 --------------------------------------------------------------
 * 
 * 
 -------------------------------------------------------------- */
#ifndef  __MICRO_STEPPER_CPP 
#define  __MICRO_STEPPER_CPP
#include "Micro_Stepper.h"

// ------ Private constants -----------------------------------
#define EN_PIN           2 // Enable
#define DIR_PIN          4 // Direction
#define STEP_PIN         3 // Step
#define DUMB_BIT         true //only exists because of the library need a dumb boolean value
#define DRIVER_ADDRESS 0b00 // TMC2209 Driver address according to MS1 and MS2
#ifdef SOFTWARE_SERIAL_ON
  #define SW_RX            9 // TMC2208/TMC2224 SoftwareSerial receive pin
  #define SW_TX            8 // TMC2208/TMC2224 SoftwareSerial transmit pin
#else
  #define SERIAL_PORT Serial // TMC2208/TMC2224 HardwareSerial port
#endif
#define R_SENSE 0.11f // Match to your driver
                      // SilentStepStick series use 0.11
                      // UltiMachine Einsy and Archim2 boards use 0.2
                      // Panucatt BSD2660 uses 0.1
                      // Watterott TMC5160 uses 0.075

#define DEFAULT_SPEED 70  //%
#define MIN_SPEED     0   //%
#define MAX_SPEED     100 //%
#define MIN_DELAY 80
#define MAX_DELAY 5000

#define PULSE_PER_ROT 3200
// ------ Private function prototypes -------------------------

// ------ Private variables -----------------------------------
#ifdef SOFTWARE_SERIAL_ON
  TMC2208Stepper driver(SW_RX, SW_TX, R_SENSE);                     // Software serial
#else
  TMC2208Stepper driver(&SERIAL_PORT, R_SENSE,DUMB_BIT);            // Hardware Serial
#endif

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
  //pinMode(DIR_PIN, OUTPUT);
  digitalWrite(EN_PIN, LOW);      // Enable driver in hardware
  #ifdef SOFTWARE_SERIAL_ON
    driver.beginSerial(115200);     // SW UART drivers
  #else
    SERIAL_PORT.begin(115200);      // HW UART drivers
  #endif
  driver.begin();                 // UART: Init SW UART (if selected) with default 115200 baudrate
  driver.toff(5);                 // Enables driver in software
  driver.rms_current(600);        // Set motor RMS current
  driver.microsteps(16);          // Set microsteps to 1/16th
//driver.en_spreadCycle(false);   // Toggle spreadCycle on TMC2208/2209/2224
  driver.pwm_autoscale(true);     // Needed for stealthChop
  numOfPulse = 0;
  numOfRot = 0; //default =0, if  >0, motor will rotate
  lastMicros = micros();
}//end stepper_init
//--------------------------------
void stepper_routine() {
  if (stepperIsRunning) {
    if ((micros()-lastMicros)>stepperSpeed) {
      lastMicros = micros();
      // digitalWrite(DIR_PIN, stepperDIR);
      driver.shaft(stepperDIR);
      digitalWrite(STEP_PIN, !digitalRead(STEP_PIN));
    }//end if
  }//end if
  if (numOfPulse<PULSE_PER_ROT*numOfRot) {
    if ((micros()-lastMicros)>stepperSpeed) {
      lastMicros = micros();
      // digitalWrite(DIR_PIN, stepperDIR);
      driver.shaft(stepperDIR);
      digitalWrite(STEP_PIN, !digitalRead(STEP_PIN));
      if (++numOfPulse>=PULSE_PER_ROT*numOfRot) {//stop rotation
        numOfPulse =0; numOfRot = 0;
      }//end if
    }//end if
  }//end if
      // Run 5000 steps and switch direction in software
    //shaft = !shaft;
    //driver.shaft(DIR);
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
}//end stepper_run
//--------------------------------
void stepper_stop() {
  stepperIsRunning = false;
}//end stepper_stop
//--------------------------------
void stepper_turn(float Rots) {
  numOfRot = Rots;
}//end stepper_turn
//--------------------------------
#endif //__MICRO_STEPPER_CPP
