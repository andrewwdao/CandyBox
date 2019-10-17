/*------------------------------------------------------------*-
  MAIN - functions file
  RASPBERRY PI 3B+
  (c) Minh-An Dao 2019
  version 1.00 - 24/09/2019
 --------------------------------------------------------------
 * This code use the TMC2208 as the driver for the stepper.
 * Other Driver (A4988, DRV8825) should work fine too.
 * You just need to calibrate the MIN_DELAY(us) and MAX_DELAY(us)
 * in Rasp_Stepper.cpp to fit those driver.
 -------------------------------------------------------------- */
#include "Rasp_Stepper.h"

int main(int dumbInt,char **InputChar) {
  float          rot = (float)atof(InputChar[1]);
  unsigned int speed = (float)atoi(InputChar[2]);
  printf("Stepper run!\n");
  stepper_run(rot,speed);
  printf("Stepper stop!\n");
}//end main
