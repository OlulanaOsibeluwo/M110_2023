#include <Adafruit_MotorShield.h>

Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *left = AFMS.getMotor(3);
Adafruit_DCMotor *right = AFMS.getMotor(4);

//linesensors pins
int v_LeftLineSensorPin = 2; 
int LeftLineSensorPin = 3;
int RightLineSensorPin = 4; 
int v_RightLineSensorPin = 5;


//junction number variable
int junction_num;

double degree_time(int rpm) {
    int r_big = 20;
    int r_small = 4;

    // no. revs for 1 degree rotation
    double one_deg = (double)(r_big / r_small) / 360.0;

    // rotation per second
    double rps = (double)rpm / 60.0;

    // time taken for 1 rotation
    double period = 1.0 / rps;

    // time taken for 1 revolution
    double t_onedeg = (double)(one_deg * period);

    return t_onedeg;
}

// forward = 1, backward = 
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
  left->run(FORWARD);}
  else {
    left->run(BACKWARD);
  }
}

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

  //initiating speed and direction of wheels
  // left_motor(200, 1);
  // right_motor(200, 1);

  left->setSpeed(250);
  right->setSpeed(250);
  left->run(FORWARD);
  right->run(FORWARD);

  delay(2000);
  Serial.println("Delay Over");
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

void reverse(int rpm){
  left_motor(rpm, 0);
  right_motor(rpm, 0);
}

void stop(){
  // cuts power to motor, does not brake
  left->run(RELEASE);
  right->run(RELEASE);
}

void light_sequence(int &v_leftVal, int &leftVal, int &rightVal, int &v_rightVal) {
  v_leftVal = digitalRead(v_LeftLineSensorPin); 
  leftVal = digitalRead(LeftLineSensorPin);
  rightVal = digitalRead(RightLineSensorPin);
  v_rightVal = digitalRead(v_RightLineSensorPin);

  Serial.print(v_leftVal);
  Serial.print(leftVal);
  Serial.print(rightVal);
  Serial.println(v_rightVal);
}

void loop() {
  // Variables to store sensor values
  int v_leftVal, leftVal, rightVal, v_rightVal;

  // Read sensor values
  light_sequence(v_leftVal, leftVal, rightVal, v_rightVal);

  /* correct
  // //turn right
  //     left_motor(120, 1);
  //     right_motor(80, 1);}

  // turn left
      left_motor(80, 1);
      right_motor(120, 1);}
  */
  
  // Straight line motion
  if (v_leftVal == 0 && leftVal == 1 && rightVal == 1 && v_rightVal == 0) {
    // Forward motion
    left_motor(255, 1);
    right_motor(255, 1);
    Serial.print("moving forwards");
  } else if (v_leftVal == 0 && leftVal == 0 && rightVal == 0 && v_rightVal == 0) {
    // Forward motion
    left_motor(255, 1);
    right_motor(255, 1);
    Serial.print("moving forwards");

  } else 
    // Either robot has slanted or has met a junction

    // Slant left
    if (v_leftVal == 0 && leftVal == 0 && rightVal == 1 && v_rightVal == 0) {
      //turn right
      left_motor(120, 1);
      right_motor(80, 1);
      delay(200);
      stop();
      Serial.print("adjusting slant");
      delay(1000);
      light_sequence(v_leftVal, leftVal, rightVal, v_rightVal);
    }

    // Slant right
    else if (v_leftVal == 0 && leftVal == 1 && rightVal == 0 && v_rightVal == 0) {
      // turn left
      left_motor(80, 1);
      right_motor(120, 1);
      delay(200);
      stop();
      Serial.print("adjusting slant");
      delay(1000);
      light_sequence(v_leftVal, leftVal, rightVal, v_rightVal);
    }

    // Junctions
    else {
      // Determine junction number based on sensor values
      if (v_leftVal == 0 && leftVal == 0 && rightVal == 0 && v_rightVal == 1) {
        junction_num = 1;
      } else if (v_leftVal == 1 && leftVal == 0 && rightVal == 0 && v_rightVal == 0) {
          junction_num = 2;
      } else if (v_leftVal == 1 && leftVal == 0 && rightVal == 0 && v_rightVal == 1){
          junction_num = 3;
      } else if (v_leftVal == 1 && leftVal == 1 && rightVal == 1 && v_rightVal == 1){
          junction_num = 4;
      } else if (v_leftVal == 0 && leftVal == 1 && rightVal == 1 && v_rightVal == 1){
          junction_num = 5;
      } else if (v_leftVal == 1 && leftVal == 1 && rightVal == 1 && v_rightVal == 0){
        junction_num = 6;}
        light_sequence(v_leftVal, leftVal, rightVal, v_rightVal);
        Serial.print("Robot is at junction ");
        Serial.println(junction_num);
        stop();
        delay(1000);
        // Junctions end
    }} 

