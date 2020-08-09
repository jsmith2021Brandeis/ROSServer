/* Jacob Smith 7/27/20 ROSSERVER
    Demonstrates the use of the motors calbirated to move in speeds
    of meters per second
*/

#include <AccelStepper.h>
//only chnage based on the robot
AccelStepper rightDrive(AccelStepper::FULL4WIRE, 26, 14, 27, 12);

int rightInvert = -1;

//the actual power to set the motors on
double drivePower=200;


//variables to control desired pseed for demo
long initTime;
long elapsedTime;

long steps;

double speed;

//runs once
void setup() {
  //set maximum speed, required for mtoors to drive but not used
  rightDrive.setMaxSpeed(10000);
  
  //initialize serial connection
  Serial.begin(9600);
  delay(2000);
  Serial.println("\n\nMOTOR EXAMPLE TEST");
  initTime = millis();
}


/*

    long    currentPosition();
    void    setCurrentPosition(long position);

*/

void loop() {

  //logic to progress commanded motors through sequence
  //o .01 m/s, -.01 m/s etc
  elapsedTime = millis() - initTime;
  steps=rightDrive.currentPosition();
  speed=steps*.5/elapsedTime;
  
  Serial.print("STEPS:\t");
  Serial.print(steps);
  Serial.print("\t");
 
  if(elapsedTime!=0){
     Serial.print(speed);
     Serial.print("\t");
     Serial.println(steps*1.0/elapsedTime*.5);
  }
  delay(10);
 

  rightDrive.setSpeed(drivePower);
  rightDrive.runSpeed();
}
