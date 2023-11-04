#include <Adafruit_MotorShield.h>
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
int currentJunction = 0; 

int Lmotor_speed, Rmotor_speed = 0; 
int v_leftVal, leftVal, rightVal, v_rightVal; //line sensor values
int blocks_delivered = 0; 
int scans_completed = 0 ; //for tracking in 2nd phase
float p1_laser_distance = 1000;
bool moving = 0;
unsigned long lastBlink;


void setup() {
  
  Serial.begin(115200);
  Wire.begin();
  sensor.begin(0x50);
  sensor.setMode(sensor.eContinuous, sensor.eHigh); 
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

 


 
  delay(2000);
  Serial.println("kk here we go");

  while (digitalRead(7) == LOW){
    Serial.println(digitalRead(7));
  }

  moving = 1;
}

void loop(){
  if(blocks_delivered<2){ //phase one, blocks on junction corners
    p1_laser_distance = sensor.getDistance();
    update_line_values();

    if (v_leftVal == 1||v_rightVal == 1){
      //junction met
      junctionHandler();

    }

    else{
      full_line_following();
    }
  }
  // second phase, blocks in empty space
  else if (blocks_delivered < 4){
    
    goto_scanpoint();
    clockwise_scan180();
    
  } 


  

  
}