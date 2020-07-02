/* Curated by Brandeis University
 *  Basic Digital Read, can be used with button
 *  tilt switch,obstacle avoidance, and water sensor
 * code source https://www.arduino.cc/en/tutorial/pushbutton
 */
// choose the input pin (for a pushbutton)
#define TOPLEFTAVOID     12
#define BOTTOMLEFTAVOID  11
#define TOPRIGHTAVOID    2
#define BOTTOMRIGHTAVOID 3

//the current value of the input device  
bool val;
//runs once to set things up for the loop
void setup() {
  //connect to the computer (at baud rate 9600)
  Serial.begin(9600);
  //wait for computer to connect
  while(!Serial){}
  //print welcome message
  Serial.println("Digital Input Example Program");
  //set all pins to input mode
  for (int i=0;i<14;i++){
      pinMode(i, INPUT);   
   }
   Serial.println("Obstacle Avoidance Sensor Example Program");
}

//runs forever, main part of program
void loop(){
  Serial.println(getBumperRegion());
  delay(500);
}

String getBumperRegion(){
  if(!digitalRead(TOPLEFTAVOID)) {
    return "TOPLEFT";  
  }else if(!digitalRead(BOTTOMLEFTAVOID)) {
    return "BOTTOMLEFT";  
  }else if(!digitalRead(TOPRIGHTAVOID)) {
    return "TOPRIGHT";  
  }else if(!digitalRead(BOTTOMRIGHTAVOID)) {
    return "BOTTOMRIGHT";  
  }else{
    return "NONE" ; 
  }
}

 
