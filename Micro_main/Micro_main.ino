/*------------------------------------------------------------*-
  MAIN - functions file
  ARDUINO MICRO
  (c) Minh-An Dao - 2019
  version 1.00 - 17/09/2019
---------------------------------------------------------------*/
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
