void update_line_values() { //updates global variables for the last reading of the line sensors
  v_leftVal = digitalRead(2); 
  leftVal = digitalRead(3);
  rightVal = digitalRead(4);
  v_rightVal = digitalRead(5);

  unsigned long currentTime = millis();

  if (currentTime - lastBlink >= 500 && moving ==1){ //causes blue LED to blink at 2Hz while moving
    bool ledState = digitalRead(8);
    digitalWrite(8,!ledState);
    lastBlink = currentTime;
    Serial.print("blink!!!");
  }


  // Serial.print(v_leftVal);
  // Serial.print(leftVal);
  // Serial.print(rightVal);
  // Serial.println(v_rightVal);


}
void full_line_following(){ //line following function to be called after update line values
  //go straight or slant left or right based all 4 line sensor readings
  
  if (v_leftVal == 0 && leftVal == 1 && rightVal == 1 && v_rightVal == 0) {
      // Forward motion
    go_straight();
    
    }
  else if (v_leftVal == 0 && leftVal == 0 && rightVal == 0 && v_rightVal == 0) { //check whether going off lines is a problem
      go_straight();
    }

  else if (v_leftVal == 0 && leftVal == 0 && rightVal == 1 && v_rightVal == 0){
    //adjust to the right
    slant_right();
    }

  else if (v_leftVal == 0 && leftVal == 1 && rightVal == 0 && v_rightVal == 0) {
    //adjust to the left
    slant_left();
    }

}

void front2_line_following(){ //line following function that only refers to 2 central line sensors, this will result in straight line following even when crossing a junction
  
  if (leftVal == 1 && rightVal == 1) {
        // Forward motion
      go_straight();
      
      }
    else if (leftVal == 0 && rightVal == 0) { //shouldnt happen but this could be real bad, turn back a bit as a contingency??
        go_straight();
      }

    else if (leftVal == 0 && rightVal == 1){
      //adjust to the right
      slant_right();
      }

    else if (leftVal == 1 && rightVal == 0) {
      //adjust to the left
      slant_left();
      }

}

void junctionHandler(){ //called upon reaching a junction in phase 1, increments junction count, checks for a block, 
  //if block found, calls function to deliver block, else navigate based on the preset path to continue searching for blocks
  stop();
  currentJunction++;
  if (p1_laser_distance < 90){ //if true, block is found
    Serial.println("block found ig");
    bool isDense = ultrasound(); //true/false should be returned from the ultrasound sensor
    homeseeking(isDense);}
  else{
    if (currentJunction == 1||currentJunction == 3||currentJunction == 6||
            currentJunction == 7||currentJunction == 8||currentJunction == 11||currentJunction == 13){
      
      ;//do nothing
    }

    else if (currentJunction == 2){
      //turn right
      right90();
    }

    else if (currentJunction == 4||currentJunction == 5||currentJunction == 9||currentJunction == 10){ //settle this
      //turn left
      left90();
    }

    
    do{
      //line follow straight to leave the junction n not re-read the same junction
      
      update_line_values();
      front2_line_following();

    }while (v_leftVal==1||v_rightVal==1);
  }
}

void homeseeking(bool isDense){//brings the robot back to the start zone from whichever junction a block was found at
  if (currentJunction == 10){
    left180();
  }

  else if (currentJunction == 11){
    ; // do nothing keep going straight
  }

  else if (currentJunction == 7){
    left90();
  }

  else {
  right180();
  }

  
  
  do{
    //line follow straight to leave the junction n not re-read the same junction
    update_line_values();
    front2_line_following();
  }while (v_leftVal==1||v_rightVal==1);

  currentJunction=currentJunction-1; 
  
  while (currentJunction>0){ //might as well be while true...
    update_line_values();

    if (v_leftVal == 1||v_rightVal == 1){
      //junction met pog
      if (currentJunction>1 && currentJunction<6){
        reversejunctionHandler();
      }
      else if (currentJunction>=6){
        reversejunctionHandler2();
      }
      
      else { //back on start line
        break;
      }
    }
    full_line_following();
  }

  //calls block delivery sequence
  offline_delivery(isDense);
  // currentJunction = 1;
    blocks_delivered++;
    if(blocks_delivered<2){//continue searching for blocks if first 2 blocks have not been found
      do{
      update_line_values();
      front2_line_following();
    }while(v_leftVal == 1||v_rightVal==1);}  
    //end here and then we return to main loop, facing out just past junction 1
    

}



void reversejunctionHandler2(){ //different path for junctions 7-10 to bring the robot back to start faster
  stop();
  
  if (currentJunction == 7 || currentJunction == 9){
    right90();
  }

  else if (currentJunction == 6){ // step after 7, not junction 6 on the layout
    // this actually means junction 2
    currentJunction = 2;
    // go_straight();
    // reversejunctionHandler();
  }

  else if (currentJunction == 8) {
    // go_straight();
    ;
  }

  else if (currentJunction == 10){ // steo after 11, not junction 10 on the layout
  // this actually means junction 2
    right90();
    currentJunction = 2;
    // go_straight();
    }

  do{
      update_line_values();
      front2_line_following();
    }while(v_leftVal == 1||v_rightVal==1);

  currentJunction=currentJunction -1;
}



void reversejunctionHandler(){//determines the preset path to bring robot back to start zone after finding a block
  stop();
  if (currentJunction == 1||currentJunction == 3||currentJunction == 6|| //currentJunction = 1 shouldn't happen
          currentJunction == 7||currentJunction == 8||currentJunction == 11){
    
    ;//do nothing
  }

  else if (currentJunction == 2||currentJunction == 12){
    //turn left
    left90();
  }

  else if (currentJunction == 4||currentJunction == 5||currentJunction == 9||currentJunction == 10){
    //turn r
    right90();
  }

  
  do{
    //line follow straight to leave the junction n not re-read the same junction
    update_line_values();
    front2_line_following();
  }while (v_leftVal==1||v_rightVal==1);
  
  currentJunction=currentJunction -1;
}

void offline_delivery(bool isDense){
  //called at the intersection of the starting zone
  //navigates the robot to either the red or green scoring zone off the white lines to avoid the speed bump
  if (isDense == 1){ //high density to red, to the left of starting facing out
   //turn right, go straight, turn left, move for x time, turn right, go off line, go on line, reverse back onto start line 
    right90();
    do{
      update_line_values();
      front2_line_following();

    }while(v_leftVal == 0);

    

    left90();
    //now we need to go forward until the block is in the range of the box
    int starttime = millis();
    int endtime = starttime;

    while ((endtime - starttime)<=1000){
      update_line_values();
      front2_line_following();
      endtime = millis();
    } 
    
    //time for the weird right 90 turn with no front sensor guidance
    right_motor(200,0);
    Rmotor_speed = -200;
    left_motor(200,1);
    Lmotor_speed = 200;
    while (digitalRead(2) ==0||digitalRead(5) ==0) // turn until both far sensors on line
      continue;
    delay(100);//at this point we are facing the scoring zone straight on
    go_straight(); 
    while (digitalRead(3) ==0&&digitalRead(4) ==0) // straight until either front on line, seems like only (3) reads coloured lines
      continue;
    delay(50); //at this point the block has been delivered, now to reverse out
    stop();
    delay(50);//this was intended to stop the rear of the robot from lifting when reversing
    reverse();
    while (digitalRead(2) ==0&&digitalRead(5) ==0) // reverse until either far sensor on line
      continue;
    right90();
    
    do{
      update_line_values();
      front2_line_following();
    }while(v_rightVal==0);
    right90();
    do{
      update_line_values();
      front2_line_following();
    }while(v_rightVal == 1);
    do{
      update_line_values();
      front2_line_following();
    }while(v_leftVal == 0);
    left90(); //now we are back at junction 1 facing out
    currentJunction=1;
  }


  else if (isDense == 0){ //low density to green, to the right of starting facing out
   //turn right, go straight, turn left, move for x time, turn right, go off line, go on line, reverse back onto start line 
    left90();
    do{
      update_line_values();
      front2_line_following();

    }while(v_rightVal == 0);

    

    right90();//at top right corner of starting zone rn
    //now we need to go forward until the block is in the range of the box
    int starttime = millis();
    int endtime = starttime;

    while ((endtime - starttime)<=700){
      update_line_values();
      front2_line_following();
      endtime = millis();
    } 
    //time for the weird left 90 turn with no front sensor guidance
    right_motor(200,1);
    Rmotor_speed = 200;
    left_motor(200,0);
    Lmotor_speed = -200;
    while (digitalRead(2) ==0||digitalRead(5) ==0) // turn until both far sensors on line
      continue;
    delay(100);
    go_straight();
    while (digitalRead(3) ==0&&digitalRead(4) ==0) // straight until either front on line, seems like only (3) reads coloured lines
      continue;
    delay(50); //at this point the block has been delivered, now to reverse out
    stop();
    delay(50);
    reverse();
    while (digitalRead(2) ==0&&digitalRead(5) ==0) // reverse until either far sensor on line
      continue;
    left90();
    
    do{
      update_line_values();
      front2_line_following();
    }while(v_leftVal==0);
    left90();
    do{
      update_line_values();
      front2_line_following();
    }while(v_leftVal == 1);
    do{
      update_line_values();
      front2_line_following();
    }while(v_rightVal == 0);
    right90(); //now we are back at junction 1 facing out,
    currentJunction = 1;

    }
  }