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

  right->setSpeed(225);


}

//junction number variable
int junction_num;

void loop() {

  int v_leftVal = digitalRead(v_LeftLineSensorPin); 
  Serial.print(v_leftVal);
  int leftVal  = digitalRead(LeftLineSensorPin);
  Serial.print(leftVal);
  int rightVal = digitalRead(RightLineSensorPin);
  Serial.print(rightVal);
  int v_rightVal = digitalRead(v_RightLineSensorPin);
  Serial.println(v_rightVal);

  //either robot has slanted or has met a junction
  if (!(v_leftVal == 0 && leftVal == 1 && rightVal == 1 && v_rightVal == 0)){

  // forward movement start
  // slant left
  if (v_leftVal == 0 && leftVal == 0 && rightVal == 1 && v_rightVal == 0){
    //adjust to the right
    left->run(FORWARD);
    right->run(BACKWARD);
    // straightened
    if (v_leftVal == 0 && leftVal == 1 && rightVal == 1 && v_rightVal == 0) {
      return;
    }}

  // slant right
  if (v_leftVal == 0 && leftVal == 1 && rightVal == 0 && v_rightVal == 0){
    //adjust to the left
    left->run(BACKWARD);
    right->run(FORWARD);
    // straightened
    if (v_leftVal == 0 && leftVal == 1 && rightVal == 1 && v_rightVal == 0) {
      return;
    }}
  // forward movement end

  //junctions start
  if (v_leftVal == 0 && leftVal == 0 && rightVal == 0 && v_rightVal == 1){
    junction_num = 1;
    left->run(BRAKE);
    right->run(BRAKE);
    Serial.print("robot is at junction");
    Serial.println("junction_num");
  }
  else if (v_leftVal == 1 && leftVal == 0 && rightVal == 0 && v_rightVal == 0){
    junction_num = 2;
    left->run(BRAKE);
    right->run(BRAKE);
    Serial.print("robot is at junction");
    Serial.println("junction_num");
  }
  else if (v_leftVal == 1 && leftVal == 0 && rightVal == 0 && v_rightVal == 1){
    junction_num = 3;
    left->run(BRAKE);
    right->run(BRAKE);
    Serial.print("robot is at junction");
    Serial.println("junction_num");
  }
  else if (v_leftVal == 1 && leftVal == 1 && rightVal == 1 && v_rightVal == 1){
    junction_num = 4;
    left->run(BRAKE);
    right->run(BRAKE);
    Serial.print("robot is at junction");
    Serial.println("junction_num");
  }
  else if (v_leftVal == 0 && leftVal == 1 && rightVal == 1 && v_rightVal == 1){
    junction_num = 5;
    left->run(BRAKE);
    right->run(BRAKE);
    Serial.print("robot is at junction");
    Serial.println("junction_num");
  }
  else if (v_leftVal == 1 && leftVal == 1 && rightVal == 1 && v_rightVal == 0){
    junction_num = 6;
    left->run(BRAKE);
    right->run(BRAKE);
    Serial.print("robot is at junction");
    Serial.println("junction_num");
  }
  // junctions end

}}