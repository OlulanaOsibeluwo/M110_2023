// low density
void green_LED(char PinStatus) {
  // LOW = on = 1,  HIGH = off, = 0
  digitalWrite(9, (PinStatus == 'LOW') ? LOW : HIGH);
}

// high density
void red_LED(char PinStatus) {
  // LOW = on = 1,  HIGH = off, = 0
  digitalWrite(10, (PinStatus == 'LOW') ? LOW : HIGH);
}

void LED_off(){
  digitalWrite(9, HIGH);
  digitalWrite(10, HIGH);
}

bool ultrasound() {

  bool block_density;
  sensity_t = analogRead(sensityPin);
  // turn the ledPin on
  dist_t = sensity_t * MAX_RANG / ADC_SOLUTION;//
  Serial.print(dist_t,0);
  Serial.println("cm");
  delay(500); //should we delete this? doesnt seem to achieve much

  // 0: low density, 1: high density 
  int benchmark = 17 ; // plug in number

  if (dist_t <= benchmark ){
    block_density = 0;
    Serial.print("low density foam ");
    green_LED('LOW'); //switch on green LED
  } else {
    block_density = 1;
    Serial.print("high density foam ");
    red_LED('LOW'); //switch on red LED
  };

  return block_density;
}

float call_laser(){ //when we're working in single mode
  sensor.start();
  float laser_distance = sensor.getDistance();
  Serial.println(laser_distance);
  return laser_distance;
}
// phase 1 laser
bool laser_p1() {

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
  int threshold_distance = 70; //plug in number

  if (distance < threshold_distance) {
    block_ahead = true;
    Serial.print("block ahead");
  } 
  return block_ahead;
}

void laser_p2() {
  
  int distance = sensor.getDistance();
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
      Serial.print("in contact with block.");
      bool block_density = ultrasound();
      if (block_density == 0) {
        Serial.print("low density block found, heading to green box");
        // path
      } else {
        Serial.print("high density block found, heading to red box");
        // path
      }
    }
  }
}