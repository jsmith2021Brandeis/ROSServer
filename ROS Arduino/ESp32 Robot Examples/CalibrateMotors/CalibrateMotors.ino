/* Jacob Smith 7/27/20 ROSSERVER
   Uses an obstacle sensor on ENCPIN and a motor with a marker on it
   to time how long different motor speeds take
*/
//setup for motors
#include <AccelStepper.h>
//onkt chnage based on the robot
AccelStepper leftDrive(AccelStepper::FULL4WIRE, 19, 18, 23, 5); // Defaults to AccelStepper::FULL4WIRE (4 pins) on 2, 3, 4, 5
AccelStepper rightDrive(AccelStepper::FULL4WIRE, 26, 14, 27, 12);
int leftInvert = -1;
int rightInvert = -1;


//change based on speed you want robot to drive
int drivePower = 20;

//other needed variables for timing
int ENCPIN = 16;
int leftDir = 1;
int rightDir = 1;
int wheelCount = 0;
long initTime;
bool trig;
bool firstTime = true;


//runs once
void setup() {
  //set up sensor and motors
  pinMode(ENCPIN, INPUT);
  leftDrive.setMaxSpeed(1000);
  rightDrive.setMaxSpeed(1000);
  //initialize serial connection and display welcome message
  Serial.begin(9600);
  delay(2000);
  Serial.println("\n \nSTARTING MOTOR SPEED TEST");
  Serial.println("Used to calibrate commanded speed with actual speeds");
  Serial.println("SPEED (Unitless) \tTIME FOR ONE REV (ms)");
}


void loop() {
  //logic to test a new speed after the desired number of trials is over
  if (wheelCount >= 3) {
    wheelCount = 0;
    drivePower+=5;
    //end test at specified speed
    if (drivePower > 540) {
      Serial.println("TESTING OVER");
      while (1) {}
    }
  }


  //logic to set the speed fro mDriveSpeed variable
  if (leftDir == 0) {
    leftDrive.stop();
  } else {
    leftDrive.setSpeed(drivePower * leftDir * leftInvert);
  }
  if (rightDir == 0) {
    rightDrive.stop();
  } else {
    rightDrive.setSpeed(drivePower * rightDir * rightInvert);
  }
  //runs the motors
  leftDrive.runSpeed();
  rightDrive.runSpeed();

  //logic to track how long a wheel takes to rotate
  trig = digitalRead(ENCPIN) == 0;
  //ignore the first time the sneosr is triggered ebcause the initial position of the
  //hweel is not known
  if (trig && firstTime) {
    firstTime = false;
    initTime = millis();
  }
  //if obstcale sensor is triggered and it was a specified time after previous trigger,
  //record the time
  if (trig && millis() - initTime > 3700 && !firstTime) {
    Serial.print(drivePower);
    Serial.print("\t");
    Serial.println(millis() - initTime);
    wheelCount++;
    initTime = millis();
  }
}
