/* Jacob Smith 7/27/20 ROSSERVER
    Live Robot Demo in ROS, uses token processing, accelstepper, and
    one of four mdoes (drive forward, drivebckward, turnright, turnleft)
    to demonsrate how the robot can be connected to ROS
*/


int driveSpeed = 100;


#include <AccelStepper.h>

AccelStepper leftDrive(AccelStepper::FULL4WIRE, 19,18,23,5); // Defaults to AccelStepper::FULL4WIRE (4 pins) on 2, 3, 4, 5
AccelStepper rightDrive(AccelStepper::FULL4WIRE, 0,16,4,17);

int speed = 100;

int leftInvert = 1;
int rightInvert = 1;

double leftSpeed=0;
double rightSpeed=0;
double leftPower=0;
double rightPower=0;


#include <Wire.h>
#include <SparkFun_APDS9960.h>

// Global Variables
SparkFun_APDS9960 apds = SparkFun_APDS9960();
uint16_t ambient_light = 0;
uint16_t red_light = 0;
uint16_t green_light = 0;
uint16_t blue_light = 0;

//the pin for the LED that illuminates the light sensor
#define LAMPPIN 13
//scaling factor to distinguish colors
//so color is green if gree nvalue is greater than COLORFACTOR*red or blue
double COLORFACTOR = 1.5;
String color="NONE";


long initTimeColor;

int INDPIN=4;

//runs once
void setup() {
 //turn color sensor lamp on
  pinMode(LAMPPIN, OUTPUT);
  digitalWrite(LAMPPIN, HIGH);
  pinMode(INDPIN, OUTPUT);


if ( apds.init() ) {
    Serial.println(F("Color Sensor initialization complete"));
  } else {
    Serial.println(F("Something went wrong during Color Sensor init!"));
  }

  // Start running the APDS-9960 light sensor (no interrupts)
  if ( apds.enableLightSensor(false) ) {
    Serial.println(F("Light sensor is now running"));
  } else {
    Serial.println(F("Something went wrong during light sensor init!"));
  }
  

  leftDrive.setMaxSpeed(speed * 2);
  rightDrive.setMaxSpeed(speed * 2);
  //attach finger
  //display welcome message
  Serial.begin(9600);
  delay(1000);
  Serial.println("Roomba Demonstration without ROS");
  initTimeColor=millis();
}


//runs many times
void loop() {
   
    if(leftSpeed!=0){
      leftDrive.runSpeed();
    }

    if(rightSpeed!=0){
      rightDrive.runSpeed();  
    }

    if(millis()-initTimeColor>200){
      
      //get and print color readouts every x milliseconds
      apds.readAmbientLight(ambient_light);
      apds.readRedLight(red_light);
      apds.readGreenLight(green_light);
      apds.readBlueLight(blue_light);

  //reuturn color
  if (ambient_light < 15) {
    color="BLACK ";
    leftSpeed=.5;
    rightSpeed=.5;
     digitalWrite(INDPIN, LOW);
  } else {
    color="WHITE";
    leftSpeed=0;
    rightSpeed=0;
    digitalWrite(INDPIN, HIGH);
    
  }



  leftPower=getPower(leftSpeed);
  rightPower=getPower(rightSpeed);
  leftDrive.setSpeed(leftPower *  leftInvert);
  rightDrive.setSpeed(rightPower * rightInvert);


  Serial.print(color);
  Serial.print("\t");
  Serial.println(ambient_light);


      initTimeColor=millis();

      
    }
  

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
