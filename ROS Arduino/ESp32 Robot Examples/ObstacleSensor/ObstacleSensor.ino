//obstacle avoidance sensor
#define FRONTLEFTOBSTACLE 32
#define FRONTRIGHTOBSTACLE 34

void setup() {
  pinMode(FRONTLEFTOBSTACLE,INPUT);
  pinMode(FRONTRIGHTOBSTACLE,INPUT);
  Serial.begin(9600);
  delay(2000);
  Serial.println("\nOBSTACLE SENSOR DEMO\n");
  
}

void loop() {
  Serial.print("FRONT LEFT:\t");
  Serial.print(digitalRead(FRONTLEFTOBSTACLE));
  
  Serial.print("\tFRONT RIGHT:\t");
  Serial.println(digitalRead(FRONTRIGHTOBSTACLE));

  delay(1000);
}
