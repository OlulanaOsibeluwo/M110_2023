#include <Adafruit_MotorShield.h>

Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *left = AFMS.getMotor(3);
Adafruit_DCMotor *right = AFMS.getMotor(2);

//linesensors pins GET RID OF THIS
// int v_LeftLineSensorPin = 2; 
// int LeftLineSensorPin = 3;
// int RightLineSensorPin = 4; 
// int v_RightLineSensorPin = 5;

// bool Lmotor_fw, Rmotor_fw = false;
int Lmotor_speed, Rmotor_speed = 0; //is there a better way to do this?? without the global vars
int currentJunction = 0;
int v_leftVal, leftVal, rightVal, v_rightVal; //line sensor values
int blocks_delivered = 0;







void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  //Pins for Line Sensors 
  pinMode(2,INPUT);
  pinMode(3,INPUT);
  pinMode(4,INPUT);
  pinMode(5,INPUT);

  if (!AFMS.begin()) {         // create with the default frequency 1.6KHz
  // if (!AFMS.begin(1000)) {  // OR with a different frequency, say 1KHz
    Serial.println("Could not find Motor Shield. Check wiring.")                                                   ;
    while (1); 
  }

  Serial.println("Motor Shield found.");

  //initiating speed and direction of wheels
  // left_motor(200, 1);
  // right_motor(200, 1);



  delay(2000);
  Serial.println("kk here we go");
}

void loop(){

 
  if(blocks_delivered<2){
    update_line_values();

    if (v_leftVal == 1||v_rightVal == 1){
      //junction met
      junctionHandler();

    }

    else{
      full_line_following();
    }
  }
}