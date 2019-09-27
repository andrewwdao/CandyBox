/*-------------------------------------------
  UART - functions file
  ARDUINO MICRO
  (c) Minh-An Dao 2019 
  version 1.10 - 18/09/2019
----------------------------------------------
 * All test function used with UART to USB has
 * to be in "No line ending" mode
 --------------------------------------------*/ 
#ifndef  __MICRO_UART_CPP
#define  __MICRO_UART_CPP
#include "Micro_UART.h"

// ------ Private constants -----------------------------------
#define DEFAULT_BAUDRATE  115200

#define ON_PIN           9  // Enable
#define OFF_PIN          10 // Disable

#define RED_PIN          16 // Indicator LED
#define GREEN_PIN        14 // Indicator LED
#define BLUE_PIN         15 // Indicator LED
// ------ Private function prototypes -------------------------
/**
Check if the master is ready or not
**/
bool MasterNotReady();
// ------ Private variables -----------------------------------
bool masterIsRunning;
// ------ PUBLIC variable definitions -------------------------

//--------------------------------------------------------------
// FUNCTION DEFINITIONS
//--------------------------------------------------------------
void UART_init()
{
  Serial.begin(DEFAULT_BAUDRATE);
  pinMode(ON_PIN,INPUT_PULLUP);
  pinMode(OFF_PIN,INPUT_PULLUP);

  pinMode(RED_PIN,OUTPUT);
  pinMode(GREEN_PIN,OUTPUT);
  pinMode(BLUE_PIN,OUTPUT);
  digitalWrite(GREEN_PIN,LOW); //turn OFF green led
  digitalWrite(BLUE_PIN,LOW); //turn OFF blue led
  digitalWrite(RED_PIN,HIGH); //turn ON red led
  masterIsRunning = false;
  while (MasterNotReady());
}
//------------------------------------------------------------
 bool MasterNotReady() {
  if( Serial.available()) //if something appear in the serial monitor
  { 
    char Ctemp = Serial.read();
    if (Ctemp=='C') { //if first letter is correct
      String rec=Serial.readString();
      if (rec == "andy Box ready!") {
        digitalWrite(RED_PIN,LOW); //turn OFF red led
        digitalWrite(GREEN_PIN,LOW); //turn OFF green led
        digitalWrite(BLUE_PIN,HIGH); //turn ON the blue led
        stepper_deactivate();
        return false;
      }//end if
    }//end if
  }//end if
  return true;
 }//end MasterNotReady
//------------------------------------------------------------
 void UART_getFromMaster() //Command:
                           //Change Speed:        S|<speed from 0 100%> (S|70)
                           //Rotate Direction:    D|<0 for CW - 1 for CCW> (D|0)
                           //Number of Rotation:  F|<float> (F|0.7)
                           //Increase Speed:      H|<speed from 0 100%>   (H|5)
                           //Decrease Speed:      L|<speed from 0 100%>   (L|3)
                           //Read speed:          Read
                           //Run unlimited:       Go!
                           //Stop immediately:    Pause!
                           
{
  if( Serial.available()) //if something appear in the serial monitor
  { 
    char Ctemp = Serial.read();
    if (Ctemp=='S') { //if command is S|<speed from 0 100%> (S|70)
		  uint16_t s1=9999;
		  String buffS="",rec="";
		  rec=Serial.readString();
		  s1=rec.indexOf("|",0); //search for next "password"
		  if(s1==0) {// if exist signal "|"
			  s1=rec.indexOf("|",0);    //position of temp1
			  buffS=rec.substring(s1+1);//get the string out
			  s1=buffS.toInt(); //speed out
			  stepper_changeSpeed(s1);
		  } else {// no exist signal "|"
			  D_PRINTLN(F("Not recognized command!"));
		  }// end if else
	  } else if (Ctemp=='D') { //if command is D|<0 for CW - 1 for CCW> (D|0)
      String rec="";
      rec=Serial.readString();
      if (rec == "|0") {  //CW
        stepper_dir(CW);
      } else if (rec=="|1"){
        stepper_dir(CCW);
      } else {// no exist signal "|"
        D_PRINTLN(F("Not recognized command!"));
      }// end if else
	  } else if (Ctemp=='F') { //if command is F|<float> (F|0.7)
      uint16_t s1=9999;
      float f1=0;
      String buffS="",rec="";
      rec=Serial.readString();
      s1=rec.indexOf("|",0); //search for next "password"
      if(s1==0) {// if exist signal "|"
        s1=rec.indexOf("|",0);    //position of temp1
        buffS=rec.substring(s1+1);//get the string out
        f1=buffS.toFloat(); //number of rotation out
        stepper_turn(f1);
      } else {// no exist signal "|"
        D_PRINTLN(F("Not recognized command!"));
      }// end if else
    } else if (Ctemp=='H') { //if command is H|<speed from 0 100%>   (H|5)
      uint16_t s1=9999;
      String buffS="",rec="";
      rec=Serial.readString();
      s1=rec.indexOf("|",0); //search for next "password"
      if(s1==0) {// if exist signal "|"
        s1=rec.indexOf("|",0);    //position of temp1
        buffS=rec.substring(s1+1);//get the string out
        s1=buffS.toInt(); //number of speed increase
        stepper_faster(s1);
      } else {// no exist signal "|"
        D_PRINTLN(F("Not recognized command!"));
      }// end if else
    } else if (Ctemp=='L') { //if command is L|<speed from 0 100%>   (L|3)
      uint16_t s1=9999;
      String buffS="",rec="";
      rec=Serial.readString();
      s1=rec.indexOf("|",0); //search for next "password"
      if(s1==0) {// if exist signal "|"
        s1=rec.indexOf("|",0);    //position of temp1
        buffS=rec.substring(s1+1);//get the string out
        s1=buffS.toInt(); //number of speed increase
        stepper_slower(s1);
      } else {// no exist signal "|"
        D_PRINTLN(F("Not recognized command!"));
      }// end if else
    } else if (Ctemp=='R') { //if command is Read
      String rec="";
      rec=Serial.readString();
      if (rec == "ead") {  //Read
        Serial.println(stepper_readSpeed());
      } else {// no exist signal "Read"
        D_PRINTLN(F("Not recognized command!"));
      }// end if else
    } else if (Ctemp=='G') { //if command is Go!
      String rec="";
      rec=Serial.readString();
      if (rec == "o!") {  //Go command
        stepper_run();
      } else {// no exist signal "Go!"
        D_PRINTLN(F("Not recognized command!"));
      }// end if else
    } else if (Ctemp=='P') { //if command is Stop!
      String rec="";
      rec=Serial.readString();
      if (rec == "ause!") {  //Pause command
        stepper_stop();
      } else {// no exist signal "Pause!"
        D_PRINTLN(F("Not recognized command!"));
      }// end if else
    } else {// not recognized command
		  D_PRINTLN(F("Not recognized command!"));
	  }// end if else
  }//end if
}// end getFromMaster
//--------------------------------
//------------------------------------------------------------
 void UART_sendToMaster() //Command:
                           //Start Python code:   Run!
                           //Stop Python code:    Stop!
                           
{
  if (digitalRead(ON_PIN)==LOW) { //turn the python code on
    if (!masterIsRunning) {
      masterIsRunning = true;
      stepper_activate();
      Serial.print(F("Run!"));
      digitalWrite(RED_PIN,LOW); //turn OFF red led
      digitalWrite(BLUE_PIN,LOW); //turn OFF the blue led
      digitalWrite(GREEN_PIN,HIGH); //turn ON green led
      return;
    }//end if
  } else if (digitalRead(OFF_PIN)==LOW) { //turn the python code off
    if (masterIsRunning) {
      masterIsRunning = false;
      stepper_deactivate();
      Serial.print(F("Stop!"));
      digitalWrite(RED_PIN,LOW); //turn OFF red led
      digitalWrite(GREEN_PIN,LOW); //turn OFF green led
      digitalWrite(BLUE_PIN,HIGH); //turn ON the blue led
      return;
    }//end if
  }
}//end UART_sendToMaster
//--------------------------------
#endif // __MICRO_UART_CPP
