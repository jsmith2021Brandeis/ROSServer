/* Jacob Smith Brandeis Automation Laboratory 6/4/2020
  Moves Stepper Motors at defined speed
*/

#include <Wire.h>


//include background library to make program work
#include <Stepper.h>
//how many steps in one revolution of specific motor
const int stepsPerRevolution = 2000;
// initialize the right and left motors of the robot
//the 2nd and 3rd paremeters are switched so motor can go backwards
Stepper rightMotors(stepsPerRevolution, 19, 18, 23, 5);
Stepper leftMotors(stepsPerRevolution, 26, 14, 27, 12);
double speed = .06; //set the desired speed in m/s

int forwardMultiplier=1;


void setup() {}

void loop() {
  moveStepper(1);
  
}

//tells stepper motor to move by certain speed
void moveStepper(double localSpeed){
 //set local speed to global speed if new speed entered
  if(speed!=localSpeed){
      speed=localSpeed;
      leftMotors.setSpeed(getSpeed(speed));
      rightMotors.setSpeed(getSpeed(speed));

      //handle whether motor moves forward or backwards
      //this multipler apporach avoids having an extra if statement
      forwardMultiplier = 1;
      if (speed < 0) {
        forwardMultiplier = -1;
      }
  } 

  
    
  if(speed!=0){//move motor one step, could be forward or backward based on multiplier
      leftMotors.step(-1 * forwardMultiplier);
      rightMotors.step(1 * forwardMultiplier);
      delay(3);
  }
}


//converts the speed in meters per second to the speed in 
//seconds per revolution using wheels size
int getSpeed(double velocity) {//time is in meters ber second
  //take the absolute value because speeds are positive on this stepper motor
  velocity=abs(velocity);
  //circumference of wheel in meters divided by velocity to get seconds per revolution
  double secondsPerRev=(2*PI*.0325)/velocity;
  return getSpeedSecondsPerRev(secondsPerRev);
}

//Jacob Smith Brandeis Autoamtion Laboratory
//Custom functiion to set teh speed of s8byj stepper mtoors based on desired rotation
//speed, works best between 4 and 10 seconds per rotation
int getSpeedSecondsPerRev(double time) {
  //speed calculated based on measurements from 28byj motor
  //this is the maximum speed of the motor
  if (time <= 4.2) {
    return 15;
    //calculates speed to set for medium speeds
  } else if (time > 4.2 && time <= 20) {
    return .0642 * time * time - 2.241 * time + 22.044;
    //calclates speed to set for very slow speeds, not accurate
  } else {//time>20)
    return .0015 * time * time - .171 * time + 5.8825;
  }
}
