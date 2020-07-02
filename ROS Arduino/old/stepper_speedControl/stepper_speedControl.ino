
/*
 Stepper Motor Control - speed control

 This program drives a unipolar or bipolar stepper motor.
 The motor is attached to digital pins 8 - 11 of the Arduino.
 A potentiometer is connected to analog input 0.

 The motor will rotate in a clockwise direction. The higher the potentiometer value,
 the faster the motor speed. Because setSpeed() sets the delay between steps,
 you may notice the motor is less responsive to changes in the sensor value at
 low speeds.

 Created 30 Nov. 2009
 Modified 28 Oct 2010
 by Tom Igoe

 */
//include background library
#include <Stepper.h>

//the desired speed of the motor
double speed=8;//set the desired speed here
//how many steps are in one revolutino for that motor
const int stepsPerRevolution = 2000;

// initialize the stepper library
Stepper myStepper(stepsPerRevolution, 19, 18, 23, 5);
Stepper myStepper2(stepsPerRevolution, 26,14, 27, 12);
int stepCount=0;
//runs once, sets speed based on desired speed
void setup() {
  myStepper.setSpeed(getSpeed(speed));
  myStepper2.setSpeed(getSpeed(speed));
}

//runs forever, moves motor at desired speed
void loop(){  
  if(stepCount<=2000){
   //move motor one step (increment)
   myStepper.step(1);
   myStepper2.step(-1);
   //wait x milliseconds so motor has time to move
   delay(2);   
   stepCount++; 
  }else{
    delay(5000);
    stepCount=0;  
  }
}

//Jacob Smith Brandeis Autoamtion Laboratory
//Custom functiion to set teh speed of s8byj stepper mtoors based on desired rotation
//speed, works best between 4 and 10 seconds per rotation
int getSpeed(double time){
    //speed calculated based on measurements from 28byj motor
    //this is the maximum speed of the motor
    if(time<=4.2){
      return 15;  
    //calculates speed to set for medium speeds
    }else if(time>4.2 && time<=20){
      return .0642*time*time-2.241*time+22.044;
    //calclates speed to set for very slow speeds, not accurate
    }else {//time>20)
        return .0015*time*time-.171*time+5.8825;
    }
}
