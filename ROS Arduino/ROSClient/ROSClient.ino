/**
 * Uses internet connection and also passes information on robot state
 */
//from https://techtutorialsx.com/2018/05/17/esp32-arduino-sending-data-with-socket-client/
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
    }else{
      
      
      
     }
      Serial.println(millis());                     // if you get a client,
    currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        
        if (c =='\n') {  // if you got anything else but a carriage return character,
          Serial.println(currentLine);
          currentLine = "";
        } else {    // if you got a newline, then clear currentLine:
           currentLine += c;      // add it to the end of the currentLine
        }
        

        delay(500);
        //dont print newlines because downstream will insert them
        client.print("PULSE: ");
        client.print(pulse);
        client.print("FLEFT: ");
        Serial.print(!digitalRead(FRONTLEFTOBSTACLE) + !digitalRead(FRONTRIGHTOBSTACLE));
        client.print(!digitalRead(FRONTLEFTOBSTACLE));
        client.print("FRIGHT: ");
        client.print(!digitalRead(FRONTRIGHTOBSTACLE));
        pulse++;  
        
    }
    }
    
}
