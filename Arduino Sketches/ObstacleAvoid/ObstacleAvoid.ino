/**
   From https://osoyoo.com/2017/04/23/arduinosmartcarlesson1/#4
   Used as prototype by Brandeis Automation Lab to test out
   components on ROSSERVER Robots
*/
//Define L298N Dual H-Bridge Motor Controller Pins
#define dir1PinL  4    //Motor direction
#define dir2PinL  5    //Motor direction
#define speedPinL 8    // Needs to be a PWM pin to be able to control motor speed
#define dir1PinR  6    //Motor direction
#define dir2PinR  7   //Motor direction
#define speedPinR 9    // Needs to be a PWM pin to be able to control motor speed


// choose the input pin (for a pushbutton)
#define TOPLEFTAVOID     12
#define BOTTOMLEFTAVOID  11
#define TOPRIGHTAVOID    2
#define BOTTOMRIGHTAVOID 3

//external led port
#define LED 13

int time = 200;
int turnTime = 200;
int power = 30;
String region;
//stores whether the robot is dirivng forward or backward, a boolean
boolean dirIsForward = true;

void setup()
{
  Serial.begin(9600);
  init_GPIO();
  set_Motorspeed(power, power);
  stop_Stop(time);//Stop
  digitalWrite(LED, HIGH);
  delay(500);
}
void loop() {
  region = getBumperRegion();
  Serial.println(region);
  //display led if obstacle detected
  digitalWrite(LED, region != "NONE");
  navigate(region);
}


//Pins initialize
void init_GPIO()
{
  pinMode(dir1PinL, OUTPUT);
  pinMode(dir2PinL, OUTPUT);
  pinMode(speedPinL, OUTPUT);

  pinMode(dir1PinR, OUTPUT);
  pinMode(dir2PinR, OUTPUT);
  pinMode(speedPinR, OUTPUT);

  pinMode(TOPLEFTAVOID, INPUT);
  pinMode(BOTTOMLEFTAVOID, INPUT);
  pinMode(TOPRIGHTAVOID, INPUT);
  pinMode(BOTTOMRIGHTAVOID, INPUT);

  pinMode(LED, OUTPUT);
}

void navigate(String region) {

  if (region == "TOPLEFT") {
    go_Back(time);
    stop_Stop(time);
    go_Right(turnTime);
    stop_Stop(time);
    dirIsForward=false;
  } else if (region == "BOTTOMLEFT") {
    go_Advance(time);
    stop_Stop(time);
    go_Right(turnTime);
    stop_Stop(time);
    dirIsForward=true;
  } else if (region == "TOPRIGHT") {
    go_Back(time);
    stop_Stop(time);
    go_Left(turnTime);
    stop_Stop(time);
    dirIsForward=false;
  } else if (region == "BOTTOMRIGHT") {
    go_Advance(time);
    stop_Stop(time);
    go_Left(turnTime);
    stop_Stop(time);
    dirIsForward=true;
  } else {
    defaultDrive();
  }
}

String getBumperRegion() {
  if (!digitalRead(TOPLEFTAVOID)) {
    return "TOPLEFT";
  } else if (!digitalRead(BOTTOMLEFTAVOID)) {
    return "BOTTOMLEFT";
  } else if (!digitalRead(TOPRIGHTAVOID)) {
    return "TOPRIGHT";
  } else if (!digitalRead(BOTTOMRIGHTAVOID)) {
    return "BOTTOMRIGHT";
  } else {
    return "NONE" ;
  }
}

//helper method to store the default driving behavior of robot
void defaultDrive() {

  if (dirIsForward) {
    go_Advance(time / 3);
  } else {
    go_Back(time / 3);
  }
  stop_Stop(time);

}

/*motor control*/
void go_Advance(int time)  //Forward
{
  digitalWrite(dir1PinL, LOW);
  digitalWrite(dir2PinL, HIGH);
  digitalWrite(dir1PinR, LOW);
  digitalWrite(dir2PinR, HIGH);
  delay(time);
}
void go_Left(int time)  //Turn left
{
  digitalWrite(dir1PinL, LOW);
  digitalWrite(dir2PinL, HIGH);
  digitalWrite(dir1PinR, HIGH);
  digitalWrite(dir2PinR, LOW);
  delay(time);
}
void go_Right(int time)  //Turn right
{
  digitalWrite(dir1PinL, HIGH);
  digitalWrite(dir2PinL, LOW);
  digitalWrite(dir1PinR, LOW);
  digitalWrite(dir2PinR, HIGH);
  delay(time);
}
void go_Back(int time)  //Reverse
{
  digitalWrite(dir1PinL, HIGH);
  digitalWrite(dir2PinL, LOW);
  digitalWrite(dir1PinR, HIGH);
  digitalWrite(dir2PinR, LOW);
  delay(time);
}
void stop_Stop(int time)    //Stop
{
  digitalWrite(dir1PinL, LOW);
  digitalWrite(dir2PinL, LOW);
  digitalWrite(dir1PinR, LOW);
  digitalWrite(dir2PinR, LOW);
  delay(time);
}
/*set motor speed */
void set_Motorspeed(int speed_L, int speed_R)
{
  analogWrite(speedPinL, speed_L);
  analogWrite(speedPinR, speed_R);
}
