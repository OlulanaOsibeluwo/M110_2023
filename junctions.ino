
#include <Adafruit_MotorShield.h>
#include <Servo.h>


// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield();

// Select motor ports M1,2,3,4
Adafruit_DCMotor *left = AFMS.getMotor(4);
Adafruit_DCMotor *right = AFMS.getMotor(3);

//initialise line_sensor pins
int lineSensorpin_1 = 8;
int lineSensorpin_2 = 9;
int lineSensorpin_3 = 11;
int lineSensorpin_4 = 12;

int right_lineSensor = digitalRead(lineSensorpin_1); //8
int v_right_lineSensor = digitalRead(lineSensorpin_2); //9
int left_lineSensor = digitalRead(lineSensorpin_3); //11
int v_left_lineSensor = digitalRead(lineSensorpin_4);

int buttonState = 0;
int buttonPin = 13;
int echoPin = 2;
int trigPin = 3;
int greenLedPin = 7;
int redLedPin = 6;
int amberLedPin = 5;

bool ledOn = false;
const int blinkDuration = 500;
unsigned long timeNow = 0;
unsigned long prevTime = 0;
unsigned long start_time =0;
unsigned long end_time =0;
unsigned long main_start_time =0;
unsigned long main_end_time =0;

int loop_num = 0;
int data;
int block_got=0;
int type;
Servo myservo;
int pos = 0;

void setup() {
 //run startup sequence, flash some LEDs to tell us its alive

  Serial.begin(9600);           // set up Serial library at 9600 bps
  //set digital line sensor pins to read
  pinMode(lineSensorpin_1, INPUT); 
  pinMode(lineSensorpin_2, INPUT);
  pinMode(lineSensorpin_3, INPUT); 
  pinMode(lineSensorpin_4, INPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(redLedPin, OUTPUT);
  pinMode(greenLedPin, OUTPUT);
  pinMode(buttonPin, INPUT);
  pinMode(amberLedPin, OUTPUT);

  myservo.attach(10);
      myservo.write(170);              
  while (buttonState == 0) {
    buttonState = digitalRead(buttonPin);
    //Serial.println(buttonState);
  }

//check if motorshield is connected
  if (!AFMS.begin()) {         // create with the default frequency 1.6KHz
    Serial.println("Could not find Motor Shield. Check wiring.");
    while (1);
  }
  Serial.println("Motor Shield found.");

//Left
  // Set the speed to start, from 0 (off) to 255 (max speed)
  left->setSpeed(225);
  left->run(RELEASE);
//RIght
  // Set the speed to start, from 0 (off) to 255 (max speed)
  right->setSpeed(225);
  right->run(RELEASE);
}
String line_data = "";
 int startFlag = 0;
//volatile int stop_follow = false;


void junction_type(int v_left_lineSensor, int left_lineSensor, int right_lineSensor, int v_right_lineSensor) {

    int junction_num;

    ||| option 1 |||
    int light_sequence[4] = {v_left_lineSensor, left_lineSensor, right_lineSensor, v_right_lineSensor};

    if (light_sequence[0] == 0 && light_sequence[1] == 0 && light_sequence[2] == 0 && light_sequence[3] == 1) {
        junction_num = 1;
    } else if (light_sequence[0] == 1 && light_sequence[1] == 0 && light_sequence[2] == 0 && light_sequence[3] == 0) {
        junction_num = 2;
    } else if (light_sequence[0] == 1 && light_sequence[1] == 0 && light_sequence[2] == 0 && light_sequence[3] == 1) {
        junction_num = 3;
    } else if (light_sequence[0] == 1 && light_sequence[1] == 1 && light_sequence[2] == 1 && light_sequence[3] == 1) {
        junction_num = 4;
    } else if (light_sequence[0] == 0 && light_sequence[1] == 1 && light_sequence[2] == 1 && light_sequence[3] == 1) {
        junction_num = 5;
    } else if (light_sequence[0] == 1 && light_sequence[1] == 1 && light_sequence[2] == 1 && light_sequence[3] == 0) {
        junction_num = 6;
    }

   ||| option 2 |||
    int light_sequence = v_left_lineSensor * 1000 + left_lineSensor * 100 + right_lineSensor * 10 + v_right_lineSensor;
    
    if (light_sequence == 0) {
        junction_num = 1;
    } else if ( light_sequence == 1000) {
        junction_num = 2;
    } else if ( light_sequence == 1001) {
        junction_num = 3;
    } else if ( light_sequence == 1111) {
        junction_num = 4;
    } else if ( light_sequence == 111) {
        junction_num = 5;
    } else if ( light_sequence == 1110) {
        junction_num = 6;
    }

}

// technically only required when looking for blocks in the free space
void position(int prev_junction_num, int prev_turn, int junction_type){

  // initialise prev_junction_num for starting point as 0
  // prev_turn: left = 0, right = 1, straight = 2

  // initially bot moves from start box to cross junction
  prev_junction_num = 0 
  prev turn = 2
  junction_type = 4
  
  position = 6

  //go forwards n detect 1111 (junction 1) special case just for starting position as 1111 usually means X junction
  //go forwards

  junction_type = junction_type(); // should be junction 6

}

void turn(int turn) {
  // straight = 0
  // right = 1
  // left = 2

  if (turn == 1) {
      left->run(FORWARD); 
      right ->run(BACKWARD);
  } else if (turn == 2){
      left->run(BACKWARD); 
      right ->run(FORWARD);
  } else if (turn == 0){
      left->run(FORWARD); 
      right ->run(FORWARD);
  } 
}

void block_detector(/* ultrasound + laser range thingy */) {

  /* logic to determine block_found_status*/
  
  return block_found_status
}

void block_density(/* ultrasound + laser range thingy */) {
  /* logic to determine block density*/
  // less dense = 0
  // more dense = 1

  return density
}

void 

void loop {
  
  block_found_status = false

  // preset path to find blocks in the corners
  while(block_found_status == false) {
  // from starting position move forwards
  int junction_type = junction_type(int v_left_lineSensor, int left_lineSensor, int right_lineSensor, int v_right_lineSensor); // should give 4
  //update position
  position = 6
  block_found_status = block_detector(/*insert params*/):

  //turn right
  turn(1);
  junction_type = junction_type(int v_left_lineSensor, int left_lineSensor, int right_lineSensor, int v_right_lineSensor); // should give 6
  position = 7
  block_found_status = block_detector(/*insert params*/):

  //*go straight
  turn(0);
  junction_type = junction_type(int v_left_lineSensor, int left_lineSensor, int right_lineSensor, int v_right_lineSensor); // should give 3
  position = 8
  block_found_status = block_detector(/*insert params*/):

  //*turn left
  turn(2);
  junction_type = junction_type(int v_left_lineSensor, int left_lineSensor, int right_lineSensor, int v_right_lineSensor); // should give 2
  position = 9
  block_found_status = block_detector(/*insert params*/):

  //*turn left
  turn(2);
  junction_type = junction_type(int v_left_lineSensor, int left_lineSensor, int right_lineSensor, int v_right_lineSensor); // should give 6
  position = 10
  block_found_status = block_detector(/*insert params*/):

  //*go straight
  turn(0);
  junction_type = junction_type(int v_left_lineSensor, int left_lineSensor, int right_lineSensor, int v_right_lineSensor); // should give 6
  position = 11
  block_found_status = block_detector(/*insert params*/):

  // *go straight
  turn(0);
  junction_type = junction_type(int v_left_lineSensor, int left_lineSensor, int right_lineSensor, int v_right_lineSensor); // should give 6
  position = 12
  block_found_status = block_detector(/*insert params*/):

  // *go straight
  turn(0);
  junction_type = junction_type(int v_left_lineSensor, int left_lineSensor, int right_lineSensor, int v_right_lineSensor); // should give 3
  position = 13
  block_found_status = block_detector(/*insert params*/):

  // *turn left
  turn(2);
  junction_type = junction_type(int v_left_lineSensor, int left_lineSensor, int right_lineSensor, int v_right_lineSensor); // should give 6
  position = 4
  block_found_status = block_detector(/*insert params*/):

  // *turn left
  turn(2);
  junction_type = junction_type(int v_left_lineSensor, int left_lineSensor, int right_lineSensor, int v_right_lineSensor); // should give 6
  position = 5
  block_found_status = block_detector(/*insert params*/):

  // *go straight
  turn(0);
  junction_type = junction_type(int v_left_lineSensor, int left_lineSensor, int right_lineSensor, int v_right_lineSensor); // should give 4
  position = 6
  block_found_status = block_detector(/*insert params*/):

  //* turn left
  turn(2);
  junction_type = junction_type(int v_left_lineSensor, int left_lineSensor, int right_lineSensor, int v_right_lineSensor); // should give 3
  position = 11
  block_found_status = block_detector(/*insert params*/):

  //* turn left
  turn(2);
  junction_type = junction_type(int v_left_lineSensor, int left_lineSensor, int right_lineSensor, int v_right_lineSensor); // should give 6
  position = 12
  block_found_status = block_detector(/*insert params*/):

  //* turn left
  turn(2);
  junction_type = junction_type(int v_left_lineSensor, int left_lineSensor, int right_lineSensor, int v_right_lineSensor); // should give 3
  position = 5
  block_found_status = block_detector(/*insert params*/):

  //* turn left
  turn(2);
  junction_type = junction_type(int v_left_lineSensor, int left_lineSensor, int right_lineSensor, int v_right_lineSensor); // should give 4
  position = 6
  block_found_status = block_detector(/*insert params*/):

  // * go straight
  turn(0);
  junction_type = junction_type(int v_left_lineSensor, int left_lineSensor, int right_lineSensor, int v_right_lineSensor); // should give 6
  position = 7
  block_found_status = block_detector(/*insert params*/):

  //* turn left
  turn(2);
  junction_type = junction_type(int v_left_lineSensor, int left_lineSensor, int right_lineSensor, int v_right_lineSensor); // should give 3
  position = 10
  block_found_status = block_detector(/*insert params*/):

  //* turn left
  turn(2);
  junction_type = junction_type(int v_left_lineSensor, int left_lineSensor, int right_lineSensor, int v_right_lineSensor); // should give 3
  position = 11
  block_found_status = block_detector(/*insert params*/):

  //* turn left
  turn(2);
  junction_type = junction_type(int v_left_lineSensor, int left_lineSensor, int right_lineSensor, int v_right_lineSensor); // should give 4
  position = 6
  block_found_status = block_detector(/*insert params*/):
  }

  // block_found_status = true (and while loop breaks)
  // depending on the position and the type of block add path here
  int density = block_density(/*add params here*/)

}
