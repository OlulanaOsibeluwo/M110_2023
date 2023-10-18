#include <Adafruit_MotorShield.h>

Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *left = AFMS.getMotor(3);
Adafruit_DCMotor *right = AFMS.getMotor(4);

//linesensors pins
int v_LeftLineSensorPin = 3; 
int LeftLineSensorPin = 4;
int RightLineSensorPin = 5; 
int v_RightLineSensorPin = 6;

//junction number variable
int junction_num;

int degree_time(int rpm) {
    int r_big = 26;
    int r_small = 4;

    // no. revs for 1 degree rotation
    float one_deg = (float)(r_big / r_small) / 360.0;

    // rotation per second
    float rps = (float)rpm / 60.0;

    // time taken for 1 rotation
    float period = 1.0 / rps;

    // time taken for 1 revolution
    int t_onedeg = (int)(one_deg * period);

    return t_onedeg;
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

  //Setting speed for left and right motors
  left->setSpeed(200);
  right->setSpeed(200);

  left->run(BACKWARD);
  right->run(BACKWARD);
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

void forward(){
  left->run(FORWARD);
  right->run(FORWARD);
}

void reverse(){
  left->run(BACKWARD);
  right->run(BACKWARD);
}

void stop(){
  // cuts power to motor, does not brake
  left->run(RELEASE);
  right->run(RELEASE);
}

void loop() {

  int v_leftVal = digitalRead(v_LeftLineSensorPin); 
  Serial.print(v_leftVal);
  int leftVal  = digitalRead(LeftLineSensorPin);
  Serial.print(leftVal);
  int rightVal = digitalRead(RightLineSensorPin);
  Serial.print(rightVal);
  int v_rightVal = digitalRead(v_RightLineSensorPin);
  Serial.println(v_rightVal);

  // straight line motion
  if (v_leftVal == 0 && leftVal == 1 && rightVal == 1 && v_rightVal == 0){
    forward();
  }

  // Either robot has slanted or has met a junction
  else { while (!(v_leftVal == 0 && leftVal == 1 && rightVal == 1 && v_rightVal == 0)){

        // Slant right
        if (v_leftVal == 0 && leftVal == 0 && rightVal == 1 && v_rightVal == 0) {
            while (v_leftVal == 0 && leftVal == 0 && rightVal == 1 && v_rightVal == 0) {
                int startTime = millis();
                int deg_time = degree_time(200);
                turn_left(200);

                int currentTime = millis();
                if (currentTime - startTime >= deg_time) {
                    stop();
                }
            }

        // slant left
        } else if (v_leftVal == 0 && leftVal == 1 && rightVal == 0 && v_rightVal == 0) {
            while (v_leftVal == 0 && leftVal == 1 && rightVal == 0 && v_rightVal == 0) {
                int startTime = millis();
                int deg_time = degree_time(200);
                turn_right(200);

                int currentTime = millis();
                if (currentTime - startTime >= deg_time) {
                    stop();
                }
            }

        // junctions start
        } else {
          if (v_leftVal == 0 && leftVal == 0 && rightVal == 0 && v_rightVal == 1){
            junction_num = 1;
          }
          else if (v_leftVal == 1 && leftVal == 0 && rightVal == 0 && v_rightVal == 0){
            junction_num = 2;
          }
          else if (v_leftVal == 1 && leftVal == 0 && rightVal == 0 && v_rightVal == 1){
            junction_num = 3;
          }
          else if (v_leftVal == 1 && leftVal == 1 && rightVal == 1 && v_rightVal == 1){
            junction_num = 4;
          }
          else if (v_leftVal == 0 && leftVal == 1 && rightVal == 1 && v_rightVal == 1){
            junction_num = 5;
          }
          else if (v_leftVal == 1 && leftVal == 1 && rightVal == 1 && v_rightVal == 0){
            junction_num = 6;
          }
          Serial.print("Robot is at junction ");
          Serial.println(junction_num);
          stop();
          break;
          // Junctions end
          }
}}}
