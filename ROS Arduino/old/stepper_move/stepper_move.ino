
/*  Stepper Motor Example Program, moves motor by number of revolutuions forward or bakcwoard
 *    so -2 is two revolutions backward
 *  Written by Jacob smith for Brandeis Automation Laboratory 5/20/2020
 *  Made from Arduino Stepper Motor Library https://github.com/arduino-libraries/Stepper
 */
//include background library to make program work
#include <Stepper.h>
//how many steps in one revolution of specific motor
const int stepsPerRevolution = 2000; 
// initialize the stepper library on pins 8 through 11:
Stepper myStepper(stepsPerRevolution, 3,5, 4, 6);

//sets up program, not used
void setup() {}

//runs forever, main part of program
void loop() {
  //move motor forward two revolutions
  moveStepper(-.5);
  //wati x milliseconds
  delay(2000);
  //move motor backward two revolutions
  moveStepper(.5);
  //wait x milliseconds
  delay(2000);
}

//move stepper x revolutions forward or backward
void moveStepper(double revolutions){
  //handle whether motor moves forward or backwards
  //this multipler apporach avoids having an extra if statement
  int forwardMultiplier=1;
  if(revolutions<0){
    forwardMultiplier=-1; 
   }
   //take absolute value of revolutions because we know if going forward or backward in multipler variable
   revolutions=abs(revolutions);
  //advance the stepper, fordwardMultipler moves forward or backward 
  int stepCount=0; 
  //keep moving as long as stepCount is less than total step count
  while(stepCount<stepsPerRevolution*revolutions){
    //move motor one step, could be forward or backward based on multiplier
    myStepper.step(1*forwardMultiplier);
    //increment step count
    stepCount++;
    //wait x milliseconds so motor has time to move
    delay(10);
  }
}
