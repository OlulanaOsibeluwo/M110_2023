#include <Adafruit_MotorShield.h>

Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *left = AFMS.getMotor(3);
Adafruit_DCMotor *right = AFMS.getMotor(4);

// Ultrasound
#define MAX_RANG (520)//the max measurement value of the module is 520cm(a little bit longer than effective max range)
#define ADC_SOLUTION (1023.0)//ADC accuracy of Arduino UNO is 10bit
int sensityPin = A2; // select the input pin

#include "Arduino.h"
#include "Wire.h"
#include "DFRobot_VL53L0X.h"
DFRobot_VL53L0X sensor;

/*
PINS list:

int v_LeftLineSensorPin = 2; 
int LeftLineSensorPin = 3;
int RightLineSensorPin = 4; 
int v_RightLineSensorPin = 5;
int greenLEDPin = 7;
int redLEDPin = 8;
int amberLEDPin = 9;
int buttonPin = 10;

*/

bool Lmotor_fw, Rmotor_fw = false;
int Lmotor_speed, Rmotor_speed = 0; //is there a better way to do this?? without the global vars
int currentJunction = 0;
float dist_t, sensity_t;
bool block_found;
int block_density = 0; // 0: low density, 1: high density


void light_sequence(int &v_leftVal, int &leftVal, int &rightVal, int &v_rightVal) {
  v_leftVal = digitalRead(2); 
  leftVal = digitalRead(3);
  rightVal = digitalRead(4);
  v_rightVal = digitalRead(5);

// //ONLY FOR NEGATIVE PRINT TESTING, RMB TO DELETE
//   v_leftVal = 1 - v_leftVal; 
//   leftVal = 1-leftVal;
//   rightVal = 1 - rightVal;
//   v_rightVal = 1-v_rightVal;
// //ONLY FOR NEGATIVE PRINT TESTING, RMB TO DELETE


  Serial.print(v_leftVal);
  Serial.print(leftVal);
  Serial.print(rightVal);
  Serial.println(v_rightVal);


}

void left_motor(int rpm, int direction){
  left->setSpeed(rpm);
  if (direction == 1) {
    left->run(FORWARD);}
  else {
    left->run(BACKWARD);
  }
}

void right_motor(int rpm, int direction){
  right->setSpeed(rpm);
  if (direction == 1) {
  right->run(FORWARD);}
  else {
    right->run(BACKWARD);
  }
}


void turn_left(int rpm){
  // Adjust to the right
  left->setSpeed(rpm);
  right->setSpeed(rpm);
  left->run(FORWARD);
  right->run(BACKWARD);
}

void turn_right(int rpm){
  // Adjust to the right
  left->setSpeed(rpm);
  right->setSpeed(rpm);
  left->run(BACKWARD);
  right->run(FORWARD);
}

void forward(int rpm){
  // left_motor(rpm, 1);
  // right_motor(rpm,1);
  left->setSpeed(rpm);
  right->setSpeed(rpm);
  left->run(FORWARD);
  right->run(FORWARD);

}
void go_straight(){
  if (Lmotor_fw!=true||Lmotor_speed != 255){
    left_motor(255,1);
    Lmotor_fw = true;
    Lmotor_speed = 255;}
 

  if (Rmotor_fw!=true||Rmotor_speed != 255){
    right_motor(255,1);
    Rmotor_fw = true;
    Rmotor_speed = 255;}  
}
void slant_right(){
  if (Lmotor_fw!=true || Lmotor_speed != 255){
    left_motor(255,1);
    Lmotor_fw = true;
    Lmotor_speed = 255;}
 
  if (Rmotor_fw!=true||Rmotor_speed != 50){
    right_motor(50,1);
    Rmotor_fw = true;
    Rmotor_speed = 50;}

}
void slant_left(){
  if (Rmotor_fw!=true||Rmotor_speed != 255){
    right_motor(255,1);
    Rmotor_fw = true;
    Rmotor_speed = 255;}
             
  if (Lmotor_fw!=true||Lmotor_speed != 50){
    left_motor(50,1);
    Lmotor_speed = 50;
  }
}


void reverse(int rpm){
  left_motor(rpm, 0);
  right_motor(rpm, 0);
}

void stop(){
  // cuts power to motor, does not brake
  left->run(RELEASE);
  right->run(RELEASE);                                    
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  // Serial.begin(115200); // line sensor requires a higher BAUD RATE

  //Pins for Line Sensors 
  pinMode(2,INPUT);
  pinMode(3,INPUT);
  pinMode(4,INPUT);
  pinMode(5,INPUT);

  //Pins for LEDs
  pinMode(7, OUTPUT); // green LED
  pinMode(8, OUTPUT); // red LED

  if (!AFMS.begin()) {         // create with the default frequency 1.6KHz
  // if (!AFMS.begin(1000)) {  // OR with a different frequency, say 1KHz
    Serial.println("Could not find Motor Shield. Check wiring.")                                                   ;
    while (1); 
  }

  Serial.println("Motor Shield found.");

  //initiating speed and direction of wheels
  // left_motor(200, 1);
  // right_motor(200, 1);

  //setting up for laser range finder
  //join i2c bus (address optional for master)
  Wire.begin();
  //Set I2C sub-device address
  sensor.begin(0x50);
  //Set to Back-to-back mode and high precision mode
  sensor.setMode(sensor.eContinuous,sensor.eHigh);
  //Laser rangefinder begins to work
  sensor.start();

  delay(2000);
  Serial.println("kk here we go");
}

// void loop(){
// // Variables to store sensor values
// int v_leftVal, leftVal, rightVal, v_rightVal;
 
//                                   // Read sensor values
// light_sequence(v_leftVal, leftVal, rightVal, v_rightVal);

// if (v_leftVal == 0 && leftVal == 1 && rightVal == 1 && v_rightVal == 0) {
//     // Forward motion
//   go_straight();
  
//   }
// else if (v_leftVal == 0 && leftVal == 0 && rightVal == 0 && v_rightVal == 0) { //check whether going off lines is a problem
//     go_straight();
//   }

// else if (v_leftVal == 0 && leftVal == 0 && rightVal == 1 && v_rightVal == 0){
//   //adjust to the right
//   slant_right();
//   }

// else if (v_leftVal == 0 && leftVal == 1 && rightVal == 0 && v_rightVal == 0) {
//   //adjust to the left
//   slant_left();
//   }


// else if (v_leftVal == 1||v_rightVal == 1){
//   //junction met pog

//   junctionHandler();

// }


// }

void loop() {
  sensity_t = analogRead(sensityPin);
  // turn the ledPin on
  dist_t = sensity_t * MAX_RANG / ADC_SOLUTION;//
  Serial.print(dist_t,0);
  Serial.println("cm");
  delay(500);

  // 0: low density, 1: high density
  int benchmark = 70 ; // plug in number
  if (dist_t <= benchmark ){
    block_density = 0;
    Serial.print("low density foam");
    green_LED("LOW"); //switch on green LED
  } else {
    block_density = 1;
    Serial.print("high density foam");
    red_LED("LOW"); //switch on red LED
  }
}