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
  left->run(RELEASE);

  right->setSpeed(225);
  right->run(RELEASE);
}

void loop() {
  // put your main code here, to run repeatedly:
  int junction_count = 0;
  int startFlag = 0;

  int v_leftVal = digitalRead(v_LeftLineSensorPin); 
  Serial.println(v_leftVal);
  int leftVal  = digitalRead(LeftLineSensorPin);
  Serial.println(leftVal);
  int rightVal = digitalRead(RightLineSensorPin);
  Serial.println(rightVal);
  int v_rightVal = digitalRead(v_RightLineSensorPin);
  Serial.println(v_rightVal);

  if (v_leftVal == 0 && leftVal == 0 && rightVal == 0 && v_rightVal == 0){
    /* GO FORWARD */
    left->run(FORWARD);
    right->run(FORWARD);
    Serial.println("BLACK");}
  if (v_leftVal == 1 && leftVal == 1 && rightVal == 1 && v_rightVal == 1){
    startFlag = 300;
    Serial.print(startFlag,"WHITE-Junction2");
    junction_count++;
    Serial.print("Junction Count:");
    Serial.print(junction_count);
    left->run(FORWARD);
    right->run(BACKWARD);}
  if ((v_leftVal == 1 && leftVal == 1) || (rightVal == 1 && v_rightVal == 1)){
    junction_count++;
    Serial.print("Junction Count");
    Serial.print(junction_count);
    left->run(FORWARD);
    right->run(BACKWARD);
  }
  v_leftVal = digitalRead(v_LeftLineSensorPin); 
  //Serial.print(v_leftVal);
  leftVal  = digitalRead(LeftLineSensorPin);
  //Serial.print(leftVal);
  rightVal = digitalRead(RightLineSensorPin);
  //Serial.print(rightVal);
  v_rightVal = digitalRead(v_RightLineSensorPin);
  //Serial.print(v_rightVal);
}
