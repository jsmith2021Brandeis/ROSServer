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
//obstacle avoidance sensor
#define FRONTLEFTOBSTACLE 32
#define FRONTRIGHTOBSTACLE 34

//include background library to make program work
#include <Stepper.h>
//how many steps in one revolution of specific motor
const int stepsPerRevolution = 2000;
// initialize the right and left motors of the robot
//the 2nd and 3rd paremeters are switched so motor can go backwards
Stepper rightMotors(stepsPerRevolution, 19, 18, 23, 5);
Stepper leftMotors(stepsPerRevolution, 26, 14, 27, 12);
double speed = .06; //set the desired speed in m/s

int stepCount=0;
double revolutions;
int forwardMultiplier=1;

//wifi global
#include <WiFi.h>
#define LEDBUILTIN 5
const char* ssid     = "Moto E (4) 5232";
const char* password = "pppppppp";
WiFiServer server(80);

int value = 0;
int pos1 = 0;
int pos2 = 0;
//end wifi globals

void setup() {
  //set mode for obstacle avoidance sensors
  pinMode(FRONTLEFTOBSTACLE, INPUT);
  pinMode(FRONTRIGHTOBSTACLE, INPUT);

  //turn color sensor lamp on
  pinMode(LAMPPIN, OUTPUT);
  digitalWrite(LAMPPIN, HIGH);
  // Initialize Serial port
  Serial.begin(9600);
  Serial.println("\n\nROSSERVER PROTOTOYPE ROBOT DEMO");
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

  //wifi setup start
  pinMode(LEDBUILTIN, OUTPUT);      // set the LED pin mode

  delay(10);

  Serial.print("Connecting to ");
  Serial.println(ssid);
  int count=0;
  while (WiFi.status() != WL_CONNECTED){
    count=0;
     WiFi.begin(ssid, password);
      while (WiFi.status() != WL_CONNECTED && count<7) {
        delay(500);
        Serial.print(".");
        count++;
      }
     if(WiFi.status()!= WL_CONNECTED){
        Serial.println("\tretrying");
        WiFi.disconnect();
     } 
  }
 

  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  server.begin();
  pinMode(LEDBUILTIN, INPUT);
  //wifi setup end
  //motors setup start
  leftMotors.setSpeed(getSpeed(speed));
  rightMotors.setSpeed(getSpeed(speed));
}

void loop() {
  printSensorData();
  runServer();
}

//move stepper motor x meters
void moveStepper(double meters){
  //divide distance by circmference of wheel
   double revolutions=meters/(2*PI*.0325);
   setStepperRevs(revolutions); 
}

//move stepper x revolutions forward or backward
void setStepperRevs(double revolutions) {

  //handle whether motor moves forward or backwards
  //this multipler apporach avoids having an extra if statement
  forwardMultiplier = 1;
  if (revolutions < 0) {
    forwardMultiplier = -1;
  }
  //take absolute value of revolutions because we know if going forward or backward in multipler variable
  revolutions = abs(revolutions);
  stepCount = 0;
  //the motor is actually run in the main loop, psuedomultithreaded
}



void printSensorData(){
//light sensor logic

  apds.readAmbientLight(ambient_light);
  apds.readRedLight(red_light);
  apds.readGreenLight(green_light);
  apds.readBlueLight(blue_light);
 /*
  Serial.print("Ambient: ");
  Serial.print(ambient_light);
  Serial.print(" Red: ");
  Serial.print(red_light);
  Serial.print(" Green: ");
  Serial.print(green_light);
  Serial.print(" Blue: ");
  Serial.print(blue_light);
  Serial.print(" Region: ");
  */
  if (ambient_light < 20) {
    color="BLACK ";
  } else if (green_light > blue_light * COLORFACTOR && green_light > blue_light * COLORFACTOR) {
    color="GREEN ";
  } else {
    color="WHITE";
  }
  /*Serial.print(color);
  //obstacle avoidance sensor logic
  Serial.print(" FLObst ");
  Serial.print(!digitalRead(FRONTLEFTOBSTACLE));

  Serial.print(" FRObst ");
  Serial.print(!digitalRead(FRONTRIGHTOBSTACLE));  
  
  Serial.println();*/
  //also move motors a step if needed
  if(stepCount < stepsPerRevolution * revolutions){
    //move motor one step, could be forward or backward based on multiplier
    leftMotors.step(-1 * forwardMultiplier);
    rightMotors.step(1 * forwardMultiplier);
    stepCount++;
  
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



//tuns the internet server
void runServer() {
  //how many revolutions the user commanded the robot to move, 0 for no commmand
  double revolutions=0;
  WiFiClient client = server.available();   // listen for incoming clients
  //stored whether the robot was commanded to do something by the website
  boolean commanded=false;
  if (client) {  
    Serial.print(" DrivingDir: ");// if you get a client,
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      printSensorData();
      if (client.available()) {             // if there's bytes to read from the client,
       
        char c = client.read();             // read a byte, then
       
        if (c == '\n') {                    // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            // the content of the HTTP response follows the header:
            client.print(" <title>ROS Server Prototype Robot</title>");
            client.print(" <h1>ROS Server Prototype Robot</h1>");
            client.print(" <h3> Brandeis Automation Laboratory</h3>");
            client.print("Click <a href=\"/H\">here</a> to drive robot forward<br>");
            client.print("Click <a href=\"/L\">here</a> to drive robot backward<br>");
            client.print("Click <a href=\"/S\">here</a> to stop robot<br>");
            client.print("<br></br>");
            client.print("<table style=\"width:50%\">");
            client.print("<tr>");
            client.print("  <th align=\"left\">Color</th>");
            client.print("  <th align=\"left\">FrontLeftObst</th>");
            client.print("   <th align=\"left\">FrontRightObst</th>");
            client.print(" </tr>");
            client.print(" <tr>");
            client.print("   <td>"); client.print(color);   client.print("</td>");
            client.print("   <td>"); client.print(!digitalRead(FRONTLEFTOBSTACLE));   client.print("</td>");
            client.print("   <td>"); client.print(!digitalRead(FRONTRIGHTOBSTACLE));   client.print("</td>");
            client.print(" </tr>");
             client.print("</table>");
              /* LOGIC to handle number input
              // Web Page
            client.println("<p>Drving Speed in Seconds per Revolution : <span id=\"servoPos\"></span></p>");          
            client.println("<input type=\"range\" min=\"5\" max=\"12\" class=\"slider\" id=\"servoSlider\" onchange=\"servo(this.value)\" value=\""+valueString+"\"/>");
            
            client.println("<script>var slider = document.getElementById(\"servoSlider\");");
            client.println("var servoP = document.getElementById(\"servoPos\"); servoP.innerHTML = slider.value;");
            client.println("slider.oninput = function() { slider.value = this.value; servoP.innerHTML = this.value; }");
            client.println("$.ajaxSetup({timeout:1000}); function servo(pos) { ");
            client.println("$.get(\"/?value=\" + pos + \"&\"); {Connection: close};}</script>");
           
            client.println("</body></html>");     
            
            //GET /?value=180& HTTP/1.1
            if(header.indexOf("GET /?value=")>=0) {
              pos1 = header.indexOf('=');
              pos2 = header.indexOf('&');
              valueString = header.substring(pos1+1, pos2);
              
              //Rotate the servo
              for(int i=0;i<10;i++){
                Serial.println(valueString);
              }
               
            }    
             */
            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          } else {    // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }

        // Check to see if the client request was "GET /H" or "GET /L":
        if (currentLine.endsWith("GET /H")) {
          Serial.print("FORWARD ");
          //move motor forward two revolutions
          moveStepper(.5);
          commanded=true;
        }
        if (currentLine.endsWith("GET /L")) {
          Serial.print("BACKWARD");
          //move motor forward two revolutions
          moveStepper(-.5);
          commanded=true;
        }
        if (currentLine.endsWith("GET /S")) {
          Serial.print("STOP    ");
          //move motor forward two revolutions
          moveStepper(0);
          commanded=true;
        }
        moveStepper(revolutions);

        if (!commanded) {
        Serial.print("NONE    ");
        }
        Serial.println();
      }
  }
  // close the connection:
  client.stop();
}
 
}
