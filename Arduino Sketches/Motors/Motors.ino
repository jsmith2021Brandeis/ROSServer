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

int time = 2000;
int power = 30;

/*motor control*/
void go_Advance(int time)  //Forward
{
  digitalWrite(dir1PinL, HIGH);
  digitalWrite(dir2PinL, LOW);
  digitalWrite(dir1PinR, HIGH);
  digitalWrite(dir2PinR, LOW);
  delay(time);
}
void go_Left(int time)  //Turn left
{
  digitalWrite(dir1PinL, HIGH);
  digitalWrite(dir2PinL, LOW);
  digitalWrite(dir1PinR, HIGH);
  digitalWrite(dir2PinR, LOW);
  delay(time);
}
void go_Right(int time)  //Turn right
{
  digitalWrite(dir1PinL, LOW);
  digitalWrite(dir2PinL,x HIGH);
  digitalWrite(dir1PinR, LOW);
  digitalWrite(dir2PinR, HIGH);
  delay(time);
}
void go_Back(int time)  //Reverse
{
  digitalWrite(dir1PinL, LOW);
  digitalWrite(dir2PinL, HIGH);
  digitalWrite(dir1PinR, LOW);
  digitalWrite(dir2PinR, HIGH);
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
//Pins initialize
void init_GPIO()
{
  pinMode(dir1PinL, OUTPUT);
  pinMode(dir2PinL, OUTPUT);
  pinMode(speedPinL, OUTPUT);

  pinMode(dir1PinR, OUTPUT);
  pinMode(dir2PinR, OUTPUT);
  pinMode(speedPinR, OUTPUT);
  stop_Stop(time);
}
void setup()
{
  init_GPIO();
  set_Motorspeed(power, power);
}
void loop() {
  go_Advance(time);//Forward
  go_Back(time);//Reverse
  go_Left(time);//Turn left
  go_Right(time);//Turn right
  stop_Stop(time);//Stop
}
