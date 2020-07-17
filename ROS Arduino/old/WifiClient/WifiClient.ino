
//from https://techtutorialsx.com/2018/05/17/esp32-arduino-sending-data-with-socket-client/
#include <WiFi.h>
 
const char* ssid = "Moto E (4) 5232";
const char* password = "pppppppp";
 
const uint16_t port = 8090;
const char * host = "192.168.43.199";
 int  count=0;
void setup()
{
 
  Serial.begin(115200);
 
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("...");
  }
 
  Serial.print("WiFi connected with IP: ");
  Serial.println(WiFi.localIP());
 
}
bool needToConnect;
String currentLine;
void loop()
{
    WiFiClient client;
 
    if (!client.connect(host, port)) {
 
        Serial.println("Connection to host failed");
        delay(1000);
        needToConnect=true;
    }else if (needToConnect){
        Serial.println("Connected to server successful!");
        client.print("Hello from ESP32! Brandeis");
        needToConnect=false;
    }else{
      currentLine = "";                // make a String to hold incoming data from the client
      Serial.println(client.read());
      /*char c = client.read();             // read a byte, then
      if (c == '\n') {                    // if the byte is a newline character
            Serial.println(currentLine);
            currentLine = "";
      }else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
       Serial.println(currentLine);*/
    }
     
    Serial.println(count);
    client.print(count);
    count++; 
    delay(1000);
}
