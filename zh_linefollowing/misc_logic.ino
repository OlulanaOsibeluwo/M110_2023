// high density
void red_LED(char PinStatus) {
  // LOW = on = 1,  HIGH = off, = 0
  digitalWrite(10, (PinStatus == 'LOW') ? LOW : HIGH);
}

// high density
void green_LED(char PinStatus) {
  // LOW = on = 1,  HIGH = off, = 0
  digitalWrite(9, (PinStatus == 'LOW') ? LOW : HIGH);
}

void ultrasound() {
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
    Serial.print("low density foam ");
    green_LED("LOW"); //switch on green LED
  } else {
    block_density = 1;
    Serial.print("high density foam ");
    red_LED("LOW"); //switch on red LED
  }
}

// phase 1 laser
void laser_p1() {

  int distance= sensor.getDistance();
  bool block_ahead = false;
  Serial.print("Distance: ");Serial.println(distance);
  //Get the distance
  // Serial.print("Distance: ");Serial.println(sensor.getDistance());
  //The delay is added to demonstrate the effect, and if you do not add the delay,
  //it will not affect the measurement accuracy
  delay(500);

  // phase 1
  // check if there is a block in front 
  int threshold_distance = 100; //plug in number

  if (distance < threshold_distance) {
    block_ahead = true;
    Serial.print("block ahead");
  } else {
    // nothing
  }
}

void laser_p2() {
  
  int distance= sensor.getDistance();
  int contact_distance = 20; // plug in max number for when block is in contact
  bool block_ahead = false;
  Serial.print("Distance: ");Serial.println(distance);

  // comand to turn servo 90 deg
  go_straight();
  
  // check if there is a block in front 
  int threshold_distance = 100; //plug in number

  if (distance < threshold_distance) {
    block_ahead = true;
    stop() ;
    Serial.print("block ahead");
    right90() ;//assuming we are sweeping w laser pointed to the right
    // command to turn servo left90
    go_straight();
    if (distance < contact_distance) {
      stop();
      Serial.print("cheese touch");
      ultrasound();
      if (block_density == 0) {
        Serial.print("low density block found, heading to green box");
        // path
      } else {
        Serial.print("high density block found, heading to red box");
        // path
      }
    }
  } else {
    // nothing
  }

}

/*pretend setup
void setup {
  // first junction at start box
  // initializing coordinates
  int x = 0;
  int y = 0;
  */
// void junctionHandler_V2(){

//   bool block_met = false;

//   // first junction at start box
//   // initializing coordinates
//   int x = 0;
//   int y = 0;

//   // bottom quarter sweep
//   go_straight();

//   // Read sensor values
//   light_sequence(v_leftVal, leftVal, rightVal, v_rightVal);

//   if (v_leftVal == 1||v_rightVal == 1){
//   //junction met pog
  


// }
