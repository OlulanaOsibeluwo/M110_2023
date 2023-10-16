#include <Adafruit_MotorShield.h>

Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *left = AFMS.getMotor(3);
Adafruit_DCMotor *right = AFMS.getMotor(4);

//linesensors pins
int v_LeftLineSensorPin = 3; 
int LeftLineSensorPin = 4;
int RightLineSensorPin = 5; 
int v_RightLineSensorPin = 6;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  //Pins for Line Sensors 
  pinMode(v_LeftLineSensorPin,INPUT);
  pinMode(LeftLineSensorPin,INPUT);
  pinMode(RightLineSensorPin,INPUT);
  pinMode(v_RightLineSensorPin,INPUT);

  if (!AFMS.begin()) {         // create with the default frequency 1.6KHz
  // if (!AFMS.begin(1000)) {  // OR with a different frequency, say 1KHz
    Serial.println("Could not find Motor Shield. Check wiring.");
    while (1);
  }

  Serial.println("Motor Shield found.");

  //Setting speed for left and right motors
  left->setSpeed(225);
  /*left->run(RELEASE);*/

  right->setSpeed(225);
  /*right->run(RELEASE);*/

  left->run(BACKWARD);
  right->run(BACKWARD);
  delay(2000);
  

  Serial.println("Delay Over");
}
void loop() {
  // put your main code here, to run repeatedly:
  int v_leftVal = digitalRead(v_LeftLineSensorPin); 
  //Serial.print(v_leftVal);
  int leftVal  = digitalRead(LeftLineSensorPin);
  //Serial.print(leftVal);
  int rightVal = digitalRead(RightLineSensorPin);
  //Serial.print(rightVal);
  int v_rightVal = digitalRead(v_RightLineSensorPin);
  //Serial.println(v_rightVal);

  if (v_leftVal == 0 && leftVal == 1 && rightVal == 1 && v_rightVal == 0){
    /* GO FORWARD */
    //left->run(FORWARD);
    //right->run(FORWARD);
    left->run(BACKWARD);
    right->run(BACKWARD);
  

}}
