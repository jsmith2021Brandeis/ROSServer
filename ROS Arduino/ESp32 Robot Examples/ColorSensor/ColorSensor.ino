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
double COLORFACTOR = 1.5;
String color="NONE";


void setup() {

  //turn color sensor lamp on
  pinMode(LAMPPIN, OUTPUT);
  digitalWrite(LAMPPIN, HIGH);
  // Initialize Serial port
  Serial.begin(9600);
  Serial.println("\n\nROSSERVER COLOR SENSOR DEMO");
  Serial.println();
  //light sensor startup code
  // Initialize APDS-9960 (configure I2C and initial values)
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
  //end light sensor startp code
  // Wait for initialization and calibration to finish
  delay(500);
}

void loop() {
  
  //read sensor values
  apds.readAmbientLight(ambient_light);
  apds.readRedLight(red_light);
  apds.readGreenLight(green_light);
  apds.readBlueLight(blue_light);
  //reuturn color
  if (ambient_light < 20) {
    color="BLACK ";
  } else if (green_light > blue_light * COLORFACTOR && green_light > blue_light * COLORFACTOR) {
    color="GREEN ";
  } else {
    color="WHITE";
  }
  
  Serial.println(color);
  delay(1000);
  
}
