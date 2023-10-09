#include <Adafruit_MotorShield.h>

int v_LeftLineSensorPin = ; 
int LeftLineSensorPin = ; 
int RightLineSensorPin = ; 
int v_RightLineSensorPin = ; 

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(v_LeftLineSensorPin,INPUT);
  pinMode(LeftLineSensorPin,INPUT);
  pinMode(RightLineSensorPin,INPUT);
  pinMode(v_RightLineSensorPin,INPUT);


}

void loop() {
  // put your main code here, to run repeatedly:
  int junction_count = 0;
  int journey_num = 0;
  bool highDensity;
  v_leftVal = digitalRead(v_LeftLineSensorPin); 
  Serial.print(v_leftVal);
  leftVal  = digitalRead(LeftLineSensorPin);
  Serial.print(leftVal);
  rightVal = digitalRead(RightLineSensorPin);
  Serial.print(rightVal);
  v_rightVal = digitalRead(v_RightLineSensorPin);
  Serial.print(v_rightVal);

  if (v_leftVal == 0 && leftVal == 0 && rightVal == 0 && v_rightVal == 0){
    /* GO FORWARD */
  }
  elif (v_leftVal == 1 && leftVal == 1 && rightVal == 1 && v_rightVal == 1) {
    junction_count++;
  }
  if (junction_count == 2 && journey_num == 0){
    /*TURN RIGHT*/
  if (junction_count == 4 && journey_num == 0){
    /*TURN LEFT*/
  }
  if (junction_count == 5 && journey_num == 0){
    /* STOP and use ultrasound to see if high/low density*/
    if (/*ULTRASOUND CHECK*/){
    highDensity = true;} 
    else {
      highDensity = false;}
    /*TURN LEFT*/

  }
  if (junction_count == 7 && journey_num == 0){
    /*TURN LEFT*/
  }
  if (junction_count == 8 && journey_num == 0){
    /*STOP and check density*/
    if (highDensity == True){
      /* TURN RIGHT and go to RED*/
      if (junction_count == 10){
        /*STOP SWIVEL 90degACW
        DEPOSIT
        SWIVEL90degACW*/}
        
      if (junction_count == 12){
        journey_num++;
        junction_count = 0;
        /* Robot now starts part 2 of journey*/
      }}

    else {
      /* TURN LEFT and go to GREEN*/
      if (junction_count == 10){
        /*STOP SWIVEL 90degCW
        DEPOSIT
        SWIVEL90degCW*/}
    if (junction_count == 12){
        journey_num++;
        junction_count = 0;
        /*STOP AND SWIVEL 180degCW*/
        /* Robot now starts part 2 of journey*/
      }}}
    if (junction_count == 2 && journey_num == 1){
      /*TURN RIGHT*/
    }
    if (junction_count == 3 && journey_num == 1){
      /*STOP and use ultrasound to see if high/low density*/
      if (/*ULTRASOUND CHECK*/){
    highDensity = true;} 
    else {
      highDensity = false;}}
      /*TURN RIGHT*/
    if (junction_count == 5 && journey_num == 1)
    {/*TURN RIGHT*/}

    if (junction_count == 6 && journey_num == 1){
      /*STOP and check density*/
    if (highDensity == True){
      /* TURN RIGHT and go to RED*/
      if (junction_count == 8){
        /*STOP SWIVEL 90degACW
        DEPOSIT
        SWIVEL90degACW*/}
        
      if (junction_count == 10){
        journey_num++;
        junction_count = 0;
        /* Robot now starts part 2 of journey*/
      }}

    else {
      /* TURN LEFT and go to GREEN*/
      if (junction_count == 8){
        /*STOP SWIVEL 90degCW
        DEPOSIT
        SWIVEL90degCW*/}
    if (junction_count == 10){
        journey_num++;
        junction_count = 0;
        /*STOP AND SWIVEL 180degCW*/
        /* Robot now starts part 2 of journey*/
      }}
      }}
