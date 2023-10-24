//we should probably just make line following into 2 callable functions, one with junction detection & one with front 2 sensors only
//consider cutting the power before we change direction / reverse? see if the thing doesnt move properly when off-line
//params to tune - speed when slanting, iterations when moving straight during block delivery
//does offline delivery actually work????

void left90(){//90 degrees left turn, only works if turning onto a line
  //turn left, pivot on L wheel
  left_motor(150,0);
  Lmotor_speed = -150;
  right_motor(150,1);
  Rmotor_speed = 150;
  // note that this only works if right front sensor is not off the line in the wrong dir before turning!!!!!!
  while (digitalRead(4) ==1) // turn until right off line
    continue;
  while (digitalRead(4)==0) //and then until on the new line
    continue;
  stop();
  //   while (digitalRead(2)==1|| digitalRead(5)==1)
  //   continue;

  // while(digitalRead(2)!=0 || digitalRead(5)!=0)
  //   continue;
  // while (digitalRead(2)==0 && digitalRead(5)==0)
  //   continue;

  stop();

}
void right90(){//90 degrees right turn
  //turn right, pivot on r wheel
  right_motor(150,0);
  Rmotor_speed = -150;
  left_motor(150,1);
  Lmotor_speed = 150;


  while (digitalRead(3) ==1) // turn until left off line
    continue;
  while (digitalRead(3)==0) 
    continue;
  stop(); //delete?
//OR
  // while (digitalRead(2)==1|| digitalRead(5)==1)
  //   continue;

  // while(digitalRead(2)!=0 || digitalRead(5)!=0)
  //   continue;
  //  while (digitalRead(2)==0 && digitalRead(5)==0)
  //   continue;

  // stop();

}


void right180(){ //this does NOT work for all cases, okay for first 2 blocks
  right_motor(255,0);
  Rmotor_speed = -255;
  left_motor(255,1);
  Lmotor_speed = 255;
  while (digitalRead(5) ==1) // turn until vright off line
    continue;
  while (digitalRead(5)==0) 
    continue;
  while (digitalRead(5)==1) 
    continue;
  while (digitalRead(5)==0) 
    continue;
  while (digitalRead(2)==0) //then back to normal line sensing
    continue;
  stop();



}

//reverse junction handler(), uturn, decrement currentJunction, turn opposite way, etc back to start!
void homeseeking(){
  right180();
  //uturn, line follow while i > 0, reverse junction handler, i- at the end AFTER LEAVING THE JUNCTION and then off - line block delivery subroutine
  int v_leftVal, leftVal, rightVal, v_rightVal;
  do{
    //line follow straight to leave the junction n not re-read the same junction
    
    light_sequence(v_leftVal, leftVal, rightVal, v_rightVal);

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
  }while (v_leftVal==1||v_rightVal==1);
  currentJunction=currentJunction-1;
  
  while (currentJunction>0){ //might as well be while true...
    light_sequence(v_leftVal, leftVal, rightVal, v_rightVal);

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


    else if (v_leftVal == 1||v_rightVal == 1){
      //junction met pog
      if (currentJunction>1){
        reversejunctionHandler();}
      else { //back on start line
        break;
      }

    }
  
  }

  //block delivery sequence
  offline_delivery(1);
}



void junctionHandler(){
  stop();
  delay(1000);
  currentJunction++;
  //query tof sensor to check for block, if block found check with ultrasound, then u turn then reversejunctionhandler()
  //if junction number > 11, jnumber = jnumber - 10 (for the loop?)
  //for now counts up to 13 just to go back to start zone
  if (currentJunction>11){//normally this would be if block found but rn this is junction 2
    homeseeking();}
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

    int v_leftVal, leftVal, rightVal, v_rightVal;
    do{
      //line follow straight to leave the junction n not re-read the same junction
      
      light_sequence(v_leftVal, leftVal, rightVal, v_rightVal);

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
    }while (v_leftVal==1||v_rightVal==1);
  }
}

void reversejunctionHandler(){
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

  int v_leftVal, leftVal, rightVal, v_rightVal; //this is PROBABLY unnecessary?
  do{
    //line follow straight to leave the junction n not re-read the same junction
    
    light_sequence(v_leftVal, leftVal, rightVal, v_rightVal);

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
  }while (v_leftVal==1||v_rightVal==1);
  currentJunction=currentJunction -1;
}

void offline_delivery(bool isDense){//1 = dense, 0 = light
  //starting point is the start zone intersection
  if (isDense == 1){ //high density to red, to the left of starting facing out
   //turn right, go straight, turn left, move for x time, turn right, go off line, go on line(rear 2 =1), reveerse back onto start line 
    right90();
    int v_leftVal, leftVal, rightVal, v_rightVal;
    do{
      light_sequence(v_leftVal, leftVal, rightVal, v_rightVal);
      if (leftVal == 1 && rightVal == 1) {
        // Forward motion
        go_straight();
      
      }
      else if (leftVal == 0 && rightVal == 0) { //we will be going off line here so it must go straight
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

    }while(v_leftVal == 0);

    

    left90();
    //now we need to go forward until the block is in the range of the box
    for (int i =0; i<10; i++){ //i< arbitrary number, tune based on how far the thing actually goes
      light_sequence(v_leftVal, leftVal, rightVal, v_rightVal);
      if (leftVal == 1 && rightVal == 1) {
        // Forward motion
        go_straight();
      
      }
      else if (leftVal == 0 && rightVal == 0) { //we will be going off line here so it must go straight
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
    //time for the weird right 90 turn with no front sensor guidance
    right_motor(255,0);
    Rmotor_speed = -255;
    left_motor(255,1);
    Lmotor_speed = 255;
    while (digitalRead(2) ==0||digitalRead(5) ==0) // turn until both far sensors on line
      continue;
    go_straight();
    while (digitalRead(2) ==1||digitalRead(5) ==1) // straight until both far sensors off line
      continue;
    while (digitalRead(2) ==0&&digitalRead(5) ==0) // straight until either on line, hopefully still straight
      continue; //at this point the block has been delivered, now to reverse out
    reverse();
    while (digitalRead(2) ==1||digitalRead(5) ==1) // reverse until both far sensors off line
      continue;
    while (digitalRead(2) ==0&&digitalRead(5) ==0) // reverse until either far sensor on line
      continue;
    right90(); // INCOMPLETE
    //right now on the left edge of start zone, navigate back to junction 1, track that 1 block delivered, set junction count, then back to main
    //when this ends we return to main loop
    }
  }






