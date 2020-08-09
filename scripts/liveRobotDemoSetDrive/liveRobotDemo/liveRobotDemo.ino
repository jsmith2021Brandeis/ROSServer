/* Jacob Smith 7/27/20 ROSSERVER
    Live Robot Demo in ROS, uses token processing, accelstepper, and
    uses command vel in ROS to drive the robot instead of preset drivign modes
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
double params[4];
int numParams = 0;

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

int leftInvert = -1;
int rightInvert = -1;

double driveSpeed=0;
//runs once
void setup() {
  pinMode(FRONTLEFTOBSTACLE,INPUT);
  pinMode(FRONTRIGHTOBSTACLE,INPUT);

  leftDrive.setMaxSpeed(1000);
  rightDrive.setMaxSpeed(1000);
  //attach finger
  //display welcome message
  Serial.begin(9600);
  pinMode(INDPIN, OUTPUT);
  //connect();

}


WiFiClient client;

//runs many times
void loop() {

Serial.println("a");

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
double angular=0;
double linear=0;
//Parse recieved commands in a non waiting fashion
void process() {
   Serial.println(receivedChars);
    //if command is arm, prepare to read 4 paremeters
    if (strcmp(receivedChars, "CMDVEL") == 0) {
      acknowledgeCommand(2,"CMDVEL");
      Serial.println(receivedChars);
    }
    //if command is manip, prepare to read 1 paremeter
    /*}else if(paramIndex<numParams){
      int dataNumber = 0;           
      dataNumber = atoi(receivedChars);
      params[paramIndex]=dataNumber;
      paramIndex++;*/
    //if looking for four params and found them, move arm
    /*}else if(numParams==2){
      angular=params[0];
      linear=params[1];
      driveSpeed=200*angular;
      Serial.print("angular\t");
      Serial.print(angular);
      Serial.print("\tlinear\t");
      Serial.println(linear); 
    //if looking for one parameter and found it, move manupulator
    }*/
   
    if (driveSpeed == 0 ) {
      leftDrive.stop();
    } else {
      leftDrive.setSpeed(driveSpeed * leftInvert);
    }
    if (driveSpeed == 0 ) {
      rightDrive.stop();
    } else {
      rightDrive.setSpeed(driveSpeed * rightInvert);
    }
}

//sets global variables and prints message to acknowledge start of header command
void acknowledgeCommand(int providedNumParams,char* command){
  numParams=providedNumParams;
  paramIndex=0;
  client.print("Recieved Command: ");
  client.print(command);
  client.print("\tObst: ");
  client.println(digitalRead(FRONTLEFTOBSTACLE));

}
