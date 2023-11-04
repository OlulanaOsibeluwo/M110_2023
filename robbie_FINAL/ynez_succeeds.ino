// low density
void green_LED(char PinStatus) { //turns green led on for 5s
  // HIGH = on = 1,  LOW = off, = 0
  if (PinStatus == 1){
    digitalWrite(9, HIGH);
    delay(5000);
    digitalWrite(9, LOW);
  }
  else{
    digitalWrite(9, LOW);
  }
}

// high density
void red_LED(char PinStatus) { //turns red led on for 5s
  // HIGH = on = 1,  LOW = off, = 0
  if (PinStatus == 1){
    digitalWrite(10, HIGH);
    delay(5000);
    digitalWrite(10, LOW);
  }
  else{
    digitalWrite(10, LOW);
  }
}


bool ultrasound() { //calls ultrasound sensor to determine density of the block found, if reading >17cm it is a high density block, else low density

  bool block_density;
  sensity_t = analogRead(sensityPin);
  // turn the ledPin on
  dist_t = sensity_t * MAX_RANG / ADC_SOLUTION;//
  Serial.print("Ultrasound: ");
  Serial.print(dist_t,0);
  Serial.println("cm");
  delay(500); //should we delete this? doesnt seem to achieve much

  // 0: low density, 1: high density 
  int benchmark = 17 ; // plug in number

  if (dist_t <= benchmark ){
    block_density = 0;
    Serial.print("low density foam ");
    green_LED(1); //switch on green LED
  } else {
    block_density = 1;
    Serial.print("high density foam ");
    red_LED(1); //switch on red LED
  };

  return block_density;
}

float call_laser(){ //gets a reading and a printed distance from the time-of-flight sensor
  sensor.start();
  float laser_distance = sensor.getDistance();
  Serial.print("Laser Distance: ");
  Serial.println(laser_distance);
  return laser_distance;
}


