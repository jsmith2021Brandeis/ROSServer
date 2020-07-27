/* Jacob Smith 7/27/20 ROSSERVER
   Uses an obstacle sensor on ENCPIN and a motor with a marker on it
   to time how long different motor speeds take

   This version incorporates the trendlines from the ClibratedMotors sketch,
   and allows for the comparison of desired speed in sec/rev and actual speed in sec/rev
*/
//setup for motors
#include <AccelStepper.h>
//onkt chnage based on the robot
AccelStepper leftDrive(AccelStepper::FULL4WIRE, 19, 18, 23, 5); // Defaults to AccelStepper::FULL4WIRE (4 pins) on 2, 3, 4, 5
AccelStepper rightDrive(AccelStepper::FULL4WIRE, 26, 14, 27, 12);
int leftInvert = -1;
int rightInvert = -1;


//change based on speed and direction you want robot to drive
double driveTime = 80;


//other needed variables for timing
double drivePower;
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
  Serial.println("Will print speeds tested and \ntime for one revolution in ms indented\n");
  Serial.println("DESIRED SPEED (sec/rev)\tCOMMANDED SPEED (unitless)\tACTUAL SPEED (sec/rev)");
  drivePower= getSpeedSecondsPerRev(driveTime);
}


void loop() {
  //logic to test a new speed after the desired number of trials is over
  if (wheelCount >= 3) {
    wheelCount = 0;
    driveTime-=5;
    drivePower= getSpeedSecondsPerRev(driveTime);
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
    Serial.print(driveTime);
    Serial.print("\t");
    Serial.print(drivePower);
    Serial.print("\t");
    Serial.println(millis() - initTime);
    wheelCount++;
    initTime = millis();
  }
}


//Jacob Smith Brandeis Autoamtion Laboratory
//Custom functiion to set teh speed of s8byj stepper mtoors based on desired rotation
//speed, works best between 4 and 10 seconds per rotation
double getSpeedSecondsPerRev(double time) {
  double speed=0;
  //speed calculated based on measurements from 28byj motor
  //this is the maximum speed of the motor

  if (time <= 6) {
    speed=-90*time + 866;
    //calculates speed to set for medium speeds
  } else if (time <= 20) {
    speed= 560 * exp(-.0949*time);
    //calclates speed to set for very slow speeds, not accurate
  } else if(time<= 100){//cap slowest speed
    speed= 141* exp(-0.02225*time);
  }
  //constrain afterwards so we don't need
  //to calculate cutoffs
  return constrain(speed,20,470);
}
