#include <Adafruit_MotorShield.h>
/* laser */
// #include "Arduino.h" //is this needed
#include "Wire.h"
#include "DFRobot_VL53L0X.h"
DFRobot_VL53L0X sensor;

/* ultrasound */
#define MAX_RANG (520)//the max measurement value of the module is 520cm(a little bit longer than effective max range)
#define ADC_SOLUTION (1023.0)//ADC accuracy of Arduino UNO is 10bit
int sensityPin = A2; // select the input pin
// float dist_t, sensity_t;
float dist_t;
float sensity_t;

Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *left = AFMS.getMotor(2);
Adafruit_DCMotor *right = AFMS.getMotor(3); //rn the motors are criss-crossed but this will change

//linesensors pins 
// int v_LeftLineSensorPin = 2; 
// int LeftLineSensorPin = 3;
// int RightLineSensorPin = 4; 
// int v_RightLineSensorPin = 5;
int currentJunction = 0; //junction 0 is an abomination
// bool Lmotor_fw, Rmotor_fw = false;
int Lmotor_speed, Rmotor_speed = 0; //is there a better way to do this?? without the global vars
int v_leftVal, leftVal, rightVal, v_rightVal; //line sensor values
int blocks_delivered = 0; //we'll change this when we're done with the second phase
int scans_completed = 0; //tentatively for tracking in 2nd phase
// float laser_distance = 1000;
// int first_activation = millis(); // make this dependent on the button press
// int buttonPin = 7;
// int button_val = 0;



void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Wire.begin();
  sensor.begin(0x50);
  sensor.setMode(sensor.eContinuous, sensor.eHigh); //we could try getting it to work in single capture to stop the spam
  sensor.start();
  //Pins for Line Sensors 
  pinMode(2,INPUT);
  pinMode(3,INPUT);
  pinMode(4,INPUT);
  pinMode(5,INPUT);

  //Pin for button
  pinMode(7, INPUT);

  //Pin for LEDs
  pinMode(8, OUTPUT); // blue
  pinMode(9, OUTPUT); // green
  pinMode(10, OUTPUT);  // red


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
  call_laser();

  int button_val = digitalRead(7); 
  // if (button_val == LOW){
  //   // do nothing aka do not move
  //   ;
  // }
  while (true) {  
  // laser_distance = sensor.getDistance();
  // Serial.println(laser_distance);
  if(blocks_delivered<2){ //phase one
    update_line_values();

    if (v_leftVal == 1||v_rightVal == 1){
      //junction met
      junctionHandler();

    }

    else{
      full_line_following();
    }
  }
  // second phase, should we make this time dependent?
  else if (true){//time condition,once not enough time engage bail algorithm
    // fn to go to starting position and face the side
    //start turning slowly
    //if you get a low reading then stop/ turn back and lock in,
    goto_scanpoint();
    clockwise_scan180();
    
  } 


  else{
    //go home from wherever
  }

  }
}