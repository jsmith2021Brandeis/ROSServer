/* Jacob Smith Brandeis Automation Laboratory 6/4/2020
  Demonstrates all teh components of the ESP32 prototype robot intended for use
  in ROSSERVER project/ The components are: stepper motors, color sensor, 2 obstacle
  avodiance sensors, and a color sensor
  source for color sensor code
  https://github.com/sparkfun/APDS-9960_RGB_and_Gesture_Sensor
  The color sensor is wired to 3.3 V, SDA on 12 and SCL on 22, that's preset by the Sparkfun_APDS9960 code
  so there is no variable for it
*/

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
double COLORFACTOR=1.5;

//obstacle avoidance sensor
#define FRONTLEFTOBSTACLE 32
#define FRONTRIGHTOBSTACLE 34

//include background library to make program work
#include <Stepper.h>
//how many steps in one revolution of specific motor
const int stepsPerRevolution = 2000; 
// initialize the right and left motors of the robot
//the 2nd and 3rd paremeters are switched so motor can go backwards
Stepper rightMotors(stepsPerRevolution, 19,18, 23, 5);
Stepper leftMotors(stepsPerRevolution, 26,14, 27, 12);


void setup() {
  //set mode for obstacle avoidance sensors
  pinMode(FRONTLEFTOBSTACLE, INPUT);
  pinMode(FRONTRIGHTOBSTACLE, INPUT);
  
  //turn color sensor lamp on
  pinMode(LAMPPIN, OUTPUT);
  digitalWrite(LAMPPIN, HIGH);
  // Initialize Serial port
  Serial.begin(9600);
  Serial.println();
  Serial.println(F("--------------------------------"));
  Serial.println(F("SparkFun APDS-9960 - ColorSensor"));
  Serial.println(F("--------------------------------"));
  //light sensor startup code
  // Initialize APDS-9960 (configure I2C and initial values)
  if ( apds.init() ) {
    Serial.println(F("APDS-9960 initialization complete"));
  } else {
    Serial.println(F("Something went wrong during APDS-9960 init!"));
  }

  // Start running the APDS-9960 light sensor (no interrupts)
  if ( apds.enableLightSensor(false) ) {
    Serial.println(F("Light sensor is now running"));
  } else {
    Serial.println(F("Something went wrong during light sensor init!"));
  }
  //end light sensor startp code
  // Wait for initialization and calibration to finish
  delay(500);
  Serial.println("Driving Forward");
  //move motor forward two revolutions
  moveStepper(.5);
  //wati x milliseconds
  delay(2000);
  Serial.println("Driving Backward");
  //move motor backward two revolutions
  moveStepper(-.5);
  //wait x milliseconds
  delay(2000);
}

void loop() {
  //light sensor logic
 
  apds.readAmbientLight(ambient_light);
  apds.readRedLight(red_light);
  apds.readGreenLight(green_light);
  apds.readBlueLight(blue_light);

    Serial.print("Ambient: ");
    Serial.print(ambient_light);
    Serial.print(" Red: ");
    Serial.print(red_light);
    Serial.print(" Green: ");
    Serial.print(green_light);
    Serial.print(" Blue: ");
    Serial.print(blue_light);
    Serial.print(" Region: ");
    if(ambient_light<20){
      Serial.print("BLACK ");  
    }else if (green_light>blue_light*COLORFACTOR && green_light>blue_light*COLORFACTOR){
      Serial.print("GREEN ");
    }else{
      Serial.print("WHITE");  
    }
    //obstacle avoidance sensor logic
    Serial.print(" FLObst ");
    Serial.print(!digitalRead(FRONTLEFTOBSTACLE));

    Serial.print(" FRObst ");
    Serial.print(!digitalRead(FRONTRIGHTOBSTACLE));
    
    Serial.println();
  //end light sensor logic

  //end light sensor logic

// Wait 1 second before next reading
delay(500);
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
    leftMotors.step(-1*forwardMultiplier);
    rightMotors.step(1*forwardMultiplier);
    //increment step count
    stepCount++;
    //wait x milliseconds so motor has time to move
    delay(10);
  }
}
