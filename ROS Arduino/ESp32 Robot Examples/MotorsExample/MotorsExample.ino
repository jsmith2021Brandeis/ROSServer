/* Jacob Smith 7/27/20 ROSSERVER
 *  Demonstrates the use of the motors calbirated to move in speeds
 *  of meters per second
*/

#include <AccelStepper.h>
//only chnage based on the robot
AccelStepper leftDrive(AccelStepper::FULL4WIRE, 19, 18, 23, 5); // Defaults to AccelStepper::FULL4WIRE (4 pins) on 2, 3, 4, 5
AccelStepper rightDrive(AccelStepper::FULL4WIRE, 26, 14, 27, 12); 
int leftInvert=-1;
int rightInvert=-1;


//how fast the robot should move in meters per second
double driveSpeed;
//the actual power to set the motors on
double drivePower;


//variables to control desired pseed for demo
long initTime;
long elapsedTime;
bool drivenBackward;
int cycleTime=6000;

//runs once
void setup() {
   //set maximum speed, required for mtoors to drive but not used
   leftDrive.setMaxSpeed(10000);
   rightDrive.setMaxSpeed(10000);
   
   //initialize serial connection
   Serial.begin(9600);
   delay(2000);
   Serial.println("\n\nMOTOR EXAMPLE TEST");
   initTime=millis();
   drivenBackward=false;
   Serial.print("Driving Forward at ");
   Serial.print(driveSpeed);
   Serial.println(" m/s");
}



void loop(){

    //logic to progress commanded motors through sequence
    //of 0 m/s, .01 m/s, -.01 m/s etc
    elapsedTime=millis()-initTime;
   
      if(elapsedTime>cycleTime*2){
          driveSpeed*=-1;
          driveSpeed+=.01;
          initTime=millis();
          drivenBackward=false;
          Serial.print("Driving Forward at ");
          Serial.print(driveSpeed);
          Serial.println(" m/s");
 
       }else if(elapsedTime>cycleTime && !drivenBackward){
          Serial.println("\tDriving Backward now");
          drivenBackward=true;
          driveSpeed*=-1;
       }

       if(driveSpeed>=.06){
          Serial.println("TEST OVER");
          while(1){} 
       
       }
     
      //logic to computer the motor power and actually set the speeds   
      drivePower=getPower(driveSpeed);   
      if(driveSpeed==0){
        leftDrive.stop();  
      }else{
         leftDrive.setSpeed(drivePower);
      }
      if(driveSpeed==0){
          rightDrive.stop();
       }else{
        rightDrive.setSpeed(drivePower);
       }  
       leftDrive.runSpeed();
       rightDrive.runSpeed();
  
}

//converts the speed in meters per second to the speed in
//seconds per revolution using wheels size
double getPower(double velocity) {//time is in meters ber second
  //handle whether the wheel is moving forward or backward
  int invert=1;
  if(velocity<0){
    invert=-1;
    velocity*=-1;
  } 
  //circumference of wheel in meters divided by velocity to get seconds per revolution
  double secondsPerRev = (2 * PI * .0325) / velocity;
  
   //now convert the speed in seconds per revolution to the actual motor power
   return getSpeedSecondsPerRev(secondsPerRev)*invert;
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
