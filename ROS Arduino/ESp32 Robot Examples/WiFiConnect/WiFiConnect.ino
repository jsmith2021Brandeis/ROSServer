/* Jacob Smith 9/10/20 ROSSERVER Project
 *  Shows how ESP32 robot connects to the internet       
*/

#include <WiFi.h>

const char* ssid = "Moto E (4) 5232";
const char* password = "pppppppp";

const uint16_t port = 8090;
const char * host = "192.168.43.231";//was .231
//bsssid, used for testing
const uint8_t bssid [6] = {0x38, 0x80, 0xDF, 0xEC, 0x35, 0xFC};//was FC

//Serial data global variables
const byte numChars = 32;
char receivedChars[numChars];   // an array to store the received data
bool newData;

bool needToConnect;
String currentLine;

//runs once
void setup() {
  //display welcome message
  Serial.begin(9600);
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
      //slash and stars are how I am processign text
      client.println("/*I like Isaac Asimov*\\");

      //read any serial input
    getToken();
    //if there is serial input, parse it
    if (newData) {
      Serial.println((char *)receivedChars);
      newData = false;
    }
     
}
}

void connect() {
  int count = 0;
  while (WiFi.status() != WL_CONNECTED) {
    count = 0;
    //WiFi.begin(ssid, password);
    WiFi.begin(ssid, password, 0, bssid,1);
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
  Serial.print("WiFi connected with IP: ");
  Serial.println(WiFi.BSSIDstr());


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
