/*------------------------------------------------------------*-
  MAIN - functions file
  ARDUINO MICRO
  (c) Minh-An Dao 2019
  version 1.00 - 17/09/2019
 --------------------------------------------------------------
 * This code use the TMC2208 as the driver for the stepper.
 * Other Driver (A4988, DRV8825) should work fine too.
 * You just need to calibrate the MIN_DELAY(us) and MAX_DELAY(us)
 * in Micro_Stepper.cpp to fit those driver.
 -------------------------------------------------------------- */
#include "Micro_Stepper.h"
#include "Micro_UART.h"
void setup() {
  UART_init();
  stepper_init();
}//end setup

void loop() {
  UART_getFromMaster();
  stepper_routine();
}//end loop
