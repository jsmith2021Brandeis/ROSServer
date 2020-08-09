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

const uint16_t port = 8090;
const char * host = "192.168.43.231";

int INDPIN = 4;

double commandedSpeed = 0;
char c;


bool needToConnect;
String currentLine;
int pulse = 0;

#include <AccelStepper.h>

AccelStepper leftDrive(AccelStepper::FULL4WIRE, 19, 18, 23, 5); // Defaults to AccelStepper::FULL4WIRE (4 pins) on 2, 3, 4, 5
AccelStepper rightDrive(AccelStepper::FULL4WIRE, 26, 14, 27, 12);

int speed = 100;

int leftDir = 1;
int rightDir = 1;

int leftInvert = -1;
int rightInvert = -1;

//runs once
void setup() {
  pinMode(FRONTLEFTOBSTACLE,INPUT);
  pinMode(FRONTRIGHTOBSTACLE,INPUT);

  leftDrive.setMaxSpeed(speed * 2);
  rightDrive.setMaxSpeed(speed * 2);
  //attach finger
  //display welcome message
  Serial.begin(9600);
  pinMode(INDPIN, OUTPUT);
  connect();

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

    leftDrive.runSpeed();
    rightDrive.runSpeed();
   
    client.println("connected");
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
  Serial.println(receivedChars);


  if (strcmp(receivedChars, "0") == 0) {
    Serial.println("Stop");
    leftDir = 0;
    rightDir = 0;
  } else if (strcmp(receivedChars, "1") == 0) {
    Serial.println("Forward");
    leftDir = 1;
    rightDir = 1;
  } else if (strcmp(receivedChars, "2") == 0) {
    Serial.println("Left");
    leftDir = 1;
    rightDir = -1;
  } else if (strcmp(receivedChars, "3") == 0) {
    Serial.println("Backward");
    leftDir = -1;
    rightDir = -1;
  } else if (strcmp(receivedChars, "4") == 0) {
    Serial.println("Right");
    leftDir = -1;
    rightDir = 1;
  }

  if (leftDir == 0) {
    leftDrive.stop();
  } else {
    leftDrive.setSpeed(driveSpeed * leftDir * leftInvert);
  }
  if (rightDir == 0) {
    rightDrive.stop();
  } else {
    rightDrive.setSpeed(driveSpeed * rightDir * rightInvert);
  }

}
