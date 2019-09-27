/*-------------------------------------------
  UART - header file
  ARDUINO MICRO
  (c) Minh-An Dao 2019 
  version 1.10 - 18/09/2019
----------------------------------------------
 * All test function used with UART to USB has
 * to be in "No line ending" mode
--------------------------------------------*/
#ifndef  __MICRO_UART_H 
#define  __MICRO_UART_H
#include <Arduino.h>
#include "Micro_Stepper.h"
#include "debugConfig.h"
//#include "config.h"

// ------ Public constants ------------------------------------

// ------ Public function prototypes --------------------------
/**
Initialize UART
**/
void UART_init();
/**
Collect the data from serial port if existed and execute the appropriate command
**/
void UART_getFromMaster();
/**
Send data to master when needed (Run! and Stop!)
**/
void UART_sendToMaster();
// ------ Public variable -------------------------------------

#endif // __MICRO_UART_H
