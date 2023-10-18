#include <Adafruit_MotorShield.h>

Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *left = AFMS.getMotor(3);
Adafruit_DCMotor *right = AFMS.getMotor(4);

//linesensors pins
int v_LeftLineSensorPin = 2; 
int LeftLineSensorPin = 3;
int RightLineSensorPin = 4; 
int v_RightLineSensorPin = 5;


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

  left->run(BACKWARD);
  right->run(BACKWARD);
  delay(2500);
}

//junction number variable
int junction_num = 0;
int junction_count = 0;
int journey_num = 1;

//block type variable
bool blockType = true;

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
    Serial.print("AT JUNCTION:");
    Serial.print(junction_num);
    junction_count++;
  // forward movement start
  // slant left
  if (v_leftVal == 0 && leftVal == 0 && rightVal == 1 && v_rightVal == 0){
    //adjust to the right
    left->run(FORWARD);
    right->run(BACKWARD);
    // straightened
    if (v_leftVal == 0 && leftVal == 1 && rightVal == 1 && v_rightVal == 0) {
      return
      Serial.println("RETURNED TO LINE");
      left->run(BACKWARD);
      right->run(BACKWARD);
    }}

  // slant right
  if (v_leftVal == 0 && leftVal == 1 && rightVal == 0 && v_rightVal == 0){
    //adjust to the left
    left->run(BACKWARD);
    right->run(FORWARD);
    // straightened
    if (v_leftVal == 0 && leftVal == 1 && rightVal == 1 && v_rightVal == 0) {
      left->run(BACKWARD);
      right->run(BACKWARD);
      return;
    }}
  // forward movement end

  /////////////////////////////////////////////////////////////////////////////

  if (journey_num == 1){
    if (junction_count ==  1){
      left->run(FORWARD);
      right->run(BACKWARD);
      }
      
    if(junction_count == 3){
      left->run(BACKWARD);
      right->run(FORWARD);
        }
    if(junction_count == 4){
      //STOP
      left->run(RELEASE);
      right->run(RELEASE);
      //CHECK ULTRASOUND
       }
     if(junction_count == 6){
       left->run(BACKWARD);
       right->run(FORWARD);
        }

     if(junction_count == 7){
       left->run(RELEASE);
       right->run(RELEASE);
      if(blockType){
        //GO TO RED
        //RIGHT
        left->run(FORWARD);
        right->run(BACKWARD);
        if (junction_count == 9){
          left->run(BACKWARD);
          right->run(FORWARD);
          //DEPOSIT BLOCK 
          //REVERSE BACK ONTO LINE
        }}
      else{
        //GO TO GREEN
        //LEFT
        left->run(BACKWARD);
        right->run(FORWARD);
        if (junction_count == 9){
          left->run(FORWARD);
          right->run(BACKWARD);
          //DEPOSIT BLOCK 
          //REVERSE BACK ONTO LINE
        }}
      }}
      journey_num++;
      junction_count = 0;
       }
  if (journey_num == 2){
    left->run(FORWARD);
    right->run(BACKWARD);
    if(junction_count == 1){
      left->run(BACKWARD);
      right->run(FORWARD);
    }
    if(junction_count == 3){
      //CHECK ULTRASOUND
      left->run(BACKWARD);
      right->run(FORWARD);
    }
    if (blockType){
      if (junction_count == 4){
        left->run(BACKWARD);
        right->run(BACKWARD);
        //DEPOSIT BLOCK 
        //REVERSE BACK ONTO LINE
      
      }
    }
    else if(!blockType){
      if (junction_count == 4){
        left->run(BACKWARD);
        right->run(FORWARD);
      }
      if (junction_count == 8){
        left->run(FORWARD);
        right->run(BACKWARD);
        //DEPOSIT BLOCK 
        //REVERSE BACK ONTO LINE
      }}
    journey_num++;
    junction_count = 0;
    }
    if (journey_num == 3){
      Serial.println("We are at 3rd stage");
      }
  
  
  //junctions start
  if (v_leftVal == 0 && leftVal == 0 && rightVal == 0 && v_rightVal == 1){
    junction_num = 1;
    left->run(RELEASE);
    right->run(RELEASE);
    Serial.print("robot is at junction ");
    Serial.println(junction_num);
  }
  else if (v_leftVal == 1 && leftVal == 0 && rightVal == 0 && v_rightVal == 0){
    junction_num = 2;
    left->run(RELEASE);
    right->run(RELEASE);
    Serial.print("robot is at junction ");
    Serial.println(junction_num);
  }
  else if (v_leftVal == 1 && leftVal == 0 && rightVal == 0 && v_rightVal == 1){
    junction_num = 3;
    left->run(RELEASE);
    right->run(RELEASE);
    Serial.print("robot is at junction ");
    Serial.println(junction_num);
  }
  else if (v_leftVal == 1 && leftVal == 1 && rightVal == 1 && v_rightVal == 1){
    junction_num = 4;
    left->run(RELEASE);
    right->run(RELEASE);
    Serial.print("robot is at junction ");
    Serial.println(junction_num);
  }
  else if (v_leftVal == 0 && leftVal == 1 && rightVal == 1 && v_rightVal == 1){
    junction_num = 5;
    left->run(RELEASE);
    right->run(RELEASE);
    Serial.print("robot is at junction ");
    Serial.println(junction_num);
  }
  else if (v_leftVal == 1 && leftVal == 1 && rightVal == 1 && v_rightVal == 0){
    junction_num = 6;
    left->run(RELEASE);
    right->run(RELEASE);
    Serial.print("robot is at junction ");
    Serial.println(junction_num);
  }
  // junctions end

}