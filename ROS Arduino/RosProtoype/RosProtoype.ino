/**
 * Uses internet connection and also passes information on robot state
 */
//from https://techtutorialsx.com/2018/05/17/esp32-arduino-sending-data-with-socket-client/

#include <Wire.h>


//include background library to make program work
#include <Stepper.h>
//how many steps in one revolution of specific motor
const int stepsPerRevolution = 2000;
// initialize the right and left motors of the robot
//the 2nd and 3rd paremeters are switched so motor can go backwards
Stepper rightMotors(stepsPerRevolution, 19, 18, 23, 5);
Stepper leftMotors(stepsPerRevolution, 26, 14, 27, 12);
double speed = .06; //set the desired speed in m/s

int forwardMultiplier=1;
#define LAMPPIN 13


#include <WiFi.h>

const char* ssid = "Moto E (4) 5232";
const char* password = "pppppppp";
 
const uint16_t port = 8090;
const char * host = "192.168.43.199";

 //obstacle avoidance sensor
#define FRONTLEFTOBSTACLE 32
#define FRONTRIGHTOBSTACLE 34

void setup()
{  //set mode for obstacle avoidance sensors
  pinMode(FRONTLEFTOBSTACLE, INPUT);
  pinMode(FRONTRIGHTOBSTACLE, INPUT);
  pinMode(LAMPPIN,OUTPUT);
  digitalWrite(LAMPPIN,LOW);
  Serial.begin(115200);
  
 connect();
}
bool needToConnect;
String currentLine;
int pulse=0;
void connect(){
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
 
  Serial.print("WiFi connected with IP: ");
  Serial.println(WiFi.localIP());
   
  
}

double commandedSpeed=0;
String temp;
void loop()
{
    WiFiClient client;
 
    if (!client.connect(host, port)) {
 
        Serial.println("Connection to host failed");
        connect();
        delay(500);
        needToConnect=true;
    }else if (needToConnect){
        Serial.println("Connected to server successful!");
        client.print("Hello from ESP32! Brandeis");
        needToConnect=false;
    }
    Serial.println(millis());                     // if you get a client,
    currentLine = "";                // make a String to hold incoming data from the client

    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
       currentLine = "";
       char c = client.read();             // read a byte, then
       while(c!='\n'){
        c = client.read();
        currentLine += c;   
       } 
      
       Serial.println(currentLine);
      
        

        //dont print newlines because downstream will insert them
        client.print("PULSE: ");
        client.print(pulse);
        client.print("FLEFT: ");
        Serial.print(!digitalRead(FRONTLEFTOBSTACLE) + !digitalRead(FRONTRIGHTOBSTACLE));
        client.print(!digitalRead(FRONTLEFTOBSTACLE));
        client.print("FRIGHT: ");
        client.print(!digitalRead(FRONTRIGHTOBSTACLE));

        for(int i=0;i<100;i++){
           moveStepper(commandedSpeed);
        }
        if(currentLine.startsWith("EL")){
          temp=currentLine.substring(currentLine.indexOf("{")+1,currentLine.indexOf("}"));
          commandedSpeed=temp.toDouble();
          Serial.print("TEMP");
          Serial.println(temp);
          Serial.print("I WILL GO AT: ");
          Serial.println(commandedSpeed);
          digitalWrite(LAMPPIN,HIGH); 
        }
         pulse++; 
        
    }
    }
    
}


//tells stepper motor to move by certain speed
void moveStepper(double localSpeed){
 //set local speed to global speed if new speed entered
  if(speed!=localSpeed){
      speed=localSpeed;
      leftMotors.setSpeed(getSpeed(speed));
      rightMotors.setSpeed(getSpeed(speed));

      //handle whether motor moves forward or backwards
      //this multipler apporach avoids having an extra if statement
      forwardMultiplier = 1;
      if (speed < 0) {
        forwardMultiplier = -1;
      }
  } 

  
    
  if(speed!=0){//move motor one step, could be forward or backward based on multiplier
      leftMotors.step(-1 * forwardMultiplier);
      rightMotors.step(1 * forwardMultiplier);
      delay(3);
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
