void left90(){//90 degrees left turn, only works if turning onto a line
  //need to change this when we get new chassis lol
  //turn left, pivot on L wheel
  left_motor(0,1);
  // Lmotor_fw = true;
  Lmotor_speed = 0; //this vs run ->release?
  right_motor(255,1);
    Rmotor_fw = true;
    Rmotor_speed = 255;
  // 1 and 0 here are ALSO FLIPPED FOR PRINTED TEST
  //note that this only works if right front sensor is not off the line before turning!!!!!!
  while (digitalRead(4) ==1) // turn until right off line
    continue;
  while (digitalRead(4)==0) //and then until on the new line
    continue;
}
void right90(){//90 degrees right turn
  //turn right, pivot on r wheel
  right_motor(0,1);
  // Rmotor_fw = true;
  Rmotor_speed = 0; //this vs run ->release?
  left_motor(255,1);
    Lmotor_fw = true;
    Lmotor_speed = 255;
  // 1 and 0 here are ALSO FLIPPED FOR PRINTED TEST
  while (digitalRead(3) ==1) // turn until left off line
    continue;
  while (digitalRead(3)==0) 
    continue;
}
void junctionHandler(){
  currentJunction++;
  //query tof sensor to check for block, if block found check with ultrasound, then reversejunctionhandler()
  //if junction number > 11, jnumber = jnumber - 10 (for the loop?)
  //for now counts up to 13 just to go back to start zone
  if (currentJunction == 1||currentJunction == 3||currentJunction == 6||
          currentJunction == 7||currentJunction == 8||currentJunction == 11||currentJunction == 13){
    
    ;//do nothing
  }

  else if (currentJunction == 2||currentJunction == 12){
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

//reverse junction handler(), uturn, decrement currentJunction, turn opposite way, etc back to start!
//then block delivery functionality

void path_red {
  // top lane
  // 9-10-11-12-13-4-2
  // bottom lane
  // 
}

void coordinates {
  // int x = 0;
  // int y = 0;

  
}