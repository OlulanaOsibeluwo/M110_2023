//we should probably just make line following into 2 callable functions, one with junction detection & one with front 2 sensors only
//consider cutting the power before we change direction / reverse? see if the thing doesnt move properly when off-line
//params to tune - speed when slanting, iterations when moving straight during block delivery
//does offline delivery actually work????
void update_line_values() {
  v_leftVal = digitalRead(2); 
  leftVal = digitalRead(3);
  rightVal = digitalRead(4);
  v_rightVal = digitalRead(5);

//ONLY FOR NEGATIVE PRINT TESTING, RMB TO DELETE
  v_leftVal = 1 - v_leftVal; 
  leftVal = 1-leftVal;
  rightVal = 1 - rightVal;
  v_rightVal = 1-v_rightVal;
//ONLY FOR NEGATIVE PRINT TESTING, RMB TO DELETE


  // Serial.print(v_leftVal);
  // Serial.print(leftVal);
  // Serial.print(rightVal);
  // Serial.println(v_rightVal);


}
void full_line_following(){
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

void front2_line_following(){
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

void junctionHandler(){
  stop();
  // delay(1000);
  currentJunction++;
  //query tof sensor to check for block, if block found check with ultrasound, then u turn then reversejunctionhandler()
  //if junction number > 11, jnumber = jnumber - 10 (for the loop?)
  //for now counts up to 13 just to go back to start zone
  if (currentJunction>11){//this should be if block found
    bool isDense =1; //true/false should be returned from the ultrasound sensor
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
//reverse junction handler(), uturn, decrement currentJunction, turn opposite way, etc back to start!
void homeseeking(bool isDense){
  right180();
  //uturn, line follow while i > 0, reverse junction handler, i- at the end AFTER LEAVING THE JUNCTION and then off - line block delivery subroutine
  
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
      if (currentJunction>1){
        reversejunctionHandler();}
      else { //back on start line
        break;
      }
    }
    full_line_following();
  }

  //block delivery sequence
  offline_delivery(isDense);
}





void reversejunctionHandler(){
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

  int v_leftVal, leftVal, rightVal, v_rightVal;
  do{
    //line follow straight to leave the junction n not re-read the same junction
    update_line_values();
    front2_line_following();
  }while (v_leftVal==1||v_rightVal==1);
  
  currentJunction=currentJunction -1;
}

void offline_delivery(bool isDense){//1 = dense, 0 = light
  //starting point is the start zone intersection
  if (isDense == 1){ //high density to red, to the left of starting facing out
   //turn right, go straight, turn left, move for x time, turn right, go off line, go on line(rear 2 =1), reveerse back onto start line 
    right90();
    do{
      update_line_values();
      front2_line_following();

    }while(v_leftVal == 0);

    

    left90();
    //now we need to go forward until the block is in the range of the box
    for (int i =0; i<120; i++){ //i< arbitrary number, tune based on how far the thing actually goes
      update_line_values();
      front2_line_following();
    }
    //time for the weird right 90 turn with no front sensor guidance
    right_motor(255,0);
    Rmotor_speed = -255;
    left_motor(255,1);
    Lmotor_speed = 255;
    while (1-digitalRead(2) ==0||1-digitalRead(5) ==0) // turn until both far sensors on line
      continue;
    go_straight(); //slow down?!
    while (1-digitalRead(2) ==1||1-digitalRead(5) ==1) // straight until both far sensors off line
      continue;
    while (1-digitalRead(3) ==0&&1-digitalRead(4) ==0) // straight until either front on line, seems like only (3) reads coloured lines
      continue; //at this point the block has been delivered, now to reverse out
    stop();
    delay(50);//this was intended to stop the rear of the robot from lifting when reversing, tbd if it helps
    reverse();
    // while (digitalRead(2) ==1||digitalRead(5) ==1) // reverse until both far sensors off line
    //   continue;
    while (1-digitalRead(2) ==0&&1-digitalRead(5) ==0) // reverse until either far sensor on line
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
    
    do{
      update_line_values();
      front2_line_following();
    }while(v_leftVal == 1||v_rightVal==1);
    currentJunction = 1;
    blocks_delivered++;
    //and here and then we return to main loop, facing out just past junction 1
    }


  else if (isDense == 0){ //low density to green, to the right of starting facing out
   //turn right, go straight, turn left, move for x time, turn right, go off line, go on line(rear 2 =1), reveerse back onto start line 
    left90();
    do{
      update_line_values();
      front2_line_following();

    }while(v_rightVal == 0);

    

    right90();//at top right corner of starting zone rn
    //now we need to go forward until the block is in the range of the box
    for (int i =0; i<120; i++){ //i< arbitrary number, tune based on how far the thing actually goes
      update_line_values();
      front2_line_following();
    }
    //time for the weird left 90 turn with no front sensor guidance
    right_motor(255,1);
    Rmotor_speed = 255;
    left_motor(255,0);
    Lmotor_speed = -255;
    while (1-digitalRead(2) ==0||1-digitalRead(5) ==0) // turn until both far sensors on line
      continue;
    go_straight(); //slow down?!
    while (1-digitalRead(2) ==1||1-digitalRead(5) ==1) // straight until both far sensors off line
      continue;
    while (1-digitalRead(3) ==0&&1-digitalRead(4) ==0) // straight until either front on line, seems like only (3) reads coloured lines
      continue; //at this point the block has been delivered, now to reverse out
    stop();
    delay(50);
    reverse();
    // while (digitalRead(2) ==1||digitalRead(5) ==1) // reverse until both far sensors off line
    //   continue;
    while (1-digitalRead(2) ==0&&1-digitalRead(5) ==0) // reverse until either far sensor on line
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
    right90(); //now we are back at junction 1 facing out, continue if more blocks, else we cut
    currentJunction = 1;
    blocks_delivered++;
    if(blocks_delivered<2){
      do{
      update_line_values();
      front2_line_following();
    }while(v_leftVal == 1||v_rightVal==1);}    
    //end here and then we return to main loop, facing out just past junction 1
    else{ //only relevant for first competition, stop after 2 blocks
      reverse();
      delay(400); //bot should be back in start zone after this
      stop(); //returns to main loop but does nothing if blocks_delivered=2

    }


    }
  }






