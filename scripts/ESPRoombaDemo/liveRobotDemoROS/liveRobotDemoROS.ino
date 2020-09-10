/* Jacob Smith 7/27/20 ROSSERVER
    Live Robot Demo in ROS, uses token processing, accelstepper, and
    one of four mdoes (drive forward, drivebckward, turnright, turnleft)
    to demonsrate how the robot can be connected to ROS
*/
//obstacle avoidance sensor
#define FRONTLEFTOBSTACLE 32
#define FRONTRIGHTOBSTACLE 34

//Serial data global variables
const byte numChars = 32;
char receivedChars[numChars];   // an array to store the received data
bool newData;

//parameter list global variables
int paramIndex = 0;
int params[4];
int numParams = 0;

int driveSpeed = 100;

#include <WiFi.h>

const char* ssid = "Moto E (4) 5232";
const char* password = "pppppppp";

const uint16_t port = 8090;//was 8080
const char * host = "192.168.43.231";//was 192.168.43.231 or 172.22.61.194

int INDPIN = 15;
char c;


bool needToConnect;
String currentLine;

#include <AccelStepper.h>

AccelStepper leftDrive(AccelStepper::FULL4WIRE, 19,18,23,5); // Defaults to AccelStepper::FULL4WIRE (4 pins) on 2, 3, 4, 5
AccelStepper rightDrive(AccelStepper::FULL4WIRE, 0,16,4,17);

int speed = 100;

int leftInvert = 1;
int rightInvert = 1;

double lin;
double ang;
int cmdvel=0;

double leftSpeed=0;
double rightSpeed=0;
double leftPower=0;
double rightPower=0;
//distance between the wheels in m
#define WHEEL_DIST .13


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


String uniq;


//runs once
void setup() {
//logic to store unique ID of ESP32 chip 
byte mac[6];
WiFi.macAddress(mac);
  uniq= String(mac[0],HEX) +String(mac[1],HEX) +String(mac[2],HEX) +String(mac[3],HEX) + String(mac[4],HEX) + String(mac[5],HEX);
 //turn color sensor lamp on
  pinMode(LAMPPIN, OUTPUT);
  digitalWrite(LAMPPIN, HIGH);

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
  
  pinMode(FRONTLEFTOBSTACLE,INPUT);
  pinMode(FRONTRIGHTOBSTACLE,INPUT);
  

  leftDrive.setMaxSpeed(speed * 2);
  rightDrive.setMaxSpeed(speed * 2);
  //attach finger
  //display welcome message
  Serial.begin(9600);
  pinMode(INDPIN, OUTPUT);
  connect();
  initTimeColor=millis();
}


WiFiClient client;

//runs many times
void loop() {



  if (!client.connect(host, port)) {

    Serial.println("Connection to host failed");
    connect();
    delay(500);
    needToConnect = true;
  } else if (needToConnect) {
    Serial.println("Connected to server successful!");
    client.print("Hello from ESP32! Brandeis");
    needToConnect = false;
  }
  Serial.println(millis());                     // if you get a client,
  currentLine = "";                // make a String to hold incoming data from the client

  while (client.connected()) {            // loop while the client's connected
    digitalWrite(INDPIN, HIGH);
    
    //read any serial input
    getToken();
    //if there is serial input, parse it
    if (newData) {
      process();
      newData = false;
    }

    if (leftSpeed!=0){
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

      //the leading print becomes a newline, so use a space and ingore it
      client.print("/*");
      client.print(uniq);
      client.print(" ");
      client.print("COLOR:");
      client.print(ambient_light);
      client.print(":");
      client.print(red_light);
      client.print(":");
      client.print(green_light);
      client.print(":");
      client.print(blue_light);
       client.print("*\\");
      //client.print("\");

      initTimeColor=millis();
    }else{
      
      client.print(""); 
    }
  
  }
  digitalWrite(INDPIN, LOW);
}


void connect() {
  int count = 0;
  while (WiFi.status() != WL_CONNECTED) {
    count = 0;
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED && count < 7) {
      delay(500);
      Serial.print(".");
      count++;
    }
    if (WiFi.status() != WL_CONNECTED) {
      Serial.println("\tretrying");
      WiFi.disconnect();
    }
  }

  Serial.print("WiFi connected with IP: ");
  Serial.println(WiFi.localIP());


}


//checks for Serial data separated by tokens
void getToken() {
  static byte ndx = 0;
  char endMarker = ' ';
  char endLineMarker = '\n';
  char rc;
  while (client.available() > 0 && newData == false) {
    rc = client.read();

    if (rc != endMarker && rc != endLineMarker) {
      receivedChars[ndx] = rc;
      ndx++;
      if (ndx >= numChars) {
        ndx = numChars - 1;
      }
    } else {
      receivedChars[ndx] = '\0'; // terminate the string
      ndx = 0;
      newData = true;
    }
  }
}



//Parse recieved commands in a non waiting fashion
void process() {
  if(strcmp(receivedChars, "CMDVEL")==0){
    cmdvel=1;
  }else if (cmdvel==1){
    ang=atof(receivedChars);
    cmdvel=2;
  }else if(cmdvel==2){
    lin=atof(receivedChars);
    Serial.print("LIN=\t");
    Serial.print(lin);
    Serial.print("\tANG=\t");
    Serial.println(ang);
    rightSpeed=(ang*WHEEL_DIST)/2 +lin;
    leftSpeed=lin*2-rightSpeed;
    leftPower=getPower(leftSpeed);
    rightPower=getPower(rightSpeed);
    Serial.print("leftSpeed=\t");
    Serial.print(leftSpeed);
    Serial.print("\trightSpeed=\t");
    Serial.println(rightSpeed);
    Serial.print("leftPower=\t");
    Serial.print(leftPower);
    Serial.print("\trightPower=\t");
    Serial.println(rightPower);
    cmdvel=0;
   }


  
   leftDrive.setSpeed(leftPower *  leftInvert);
   rightDrive.setSpeed(rightPower * rightInvert);
  

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
