// int turntime = 0;

void goto_scanpoint(){//navigate to next scanpoint based on current location and scans_completed
  if (currentJunction ==0 && digitalRead(3)==0 && digitalRead(4)==0){ //brings us there if when skipping phase 1 (ONLY FOR TESTING)
    while(digitalRead(2)==0 && digitalRead(5)==0){
      go_straight();
    }
    stop();
    currentJunction =1;
  }
  if (currentJunction == 1) {for(int i=0; i<2; i++){
    do{
        update_line_values();
        front2_line_following();
        }while (v_leftVal==1||v_rightVal==1);

  do{
        update_line_values();
        front2_line_following();
        }while (v_leftVal==0||v_rightVal==0);}
  
  
  if (scans_completed == 0){
    left90();
    stop();
    }

  else if (scans_completed == 1){
    left90();
    do{
        update_line_values();
        front2_line_following();
        }while (v_leftVal==1||v_rightVal==1);

    do{
        update_line_values();
        front2_line_following();
        }while (v_rightVal==0 && v_leftVal ==0);
      
    do{
        update_line_values();
        front2_line_following();
        }while (v_leftVal==1||v_rightVal==1);

    do{
        update_line_values();
        front2_line_following();
        }while (v_rightVal==0 && v_leftVal ==0);

      right90();
      int starttime = millis();
    int endtime = starttime;

    while ((endtime - starttime)<=2100){ //tbd
      update_line_values();
      front2_line_following();
      endtime = millis();
    }   

  }
  
  else if (scans_completed == 2){
    left90();
    do{
        update_line_values();
        front2_line_following();
        }while (v_leftVal==1||v_rightVal==1);

    do{
        update_line_values();
        front2_line_following();
        }while (v_rightVal==0 && v_leftVal ==0);
      
    do{
        update_line_values();
        front2_line_following();
        }while (v_leftVal==1||v_rightVal==1);

    do{
        update_line_values();
        front2_line_following();
        }while (v_rightVal==0 && v_leftVal ==0);

      right90();
      do{
        update_line_values();
        front2_line_following();
        }while (v_leftVal==1||v_rightVal==1);

    do{
        update_line_values();
        front2_line_following();
        }while (v_rightVal==0 && v_leftVal ==0);
    right90();
    int starttime = millis();
    int endtime = starttime;
    while ((endtime - starttime)<=7000){ //tbd
      update_line_values();
      front2_line_following();
      endtime = millis();
    }   

  }

  else if (scans_completed == 3){
  right90();
    do{
        update_line_values();
        front2_line_following();
        }while (v_leftVal==1||v_rightVal==1);

    do{
        update_line_values();
        front2_line_following();
        }while (v_leftVal==0 && v_rightVal==0);

    do{
        update_line_values();
        front2_line_following();
        }while (v_leftVal==1||v_rightVal==1);

    do{
        update_line_values();
        front2_line_following();
        }while (v_leftVal==0 && v_rightVal==0);

    left90();
    int starttime = millis();
    int endtime = starttime;

    while ((endtime - starttime)<=2100){
      update_line_values();
      front2_line_following();
      endtime = millis();
    } 
    right90();
    stop();}
    
  }
  else if (currentJunction == 99){//alr at a scan point, just finished a scan
    if (scans_completed == 1){
      left90(); //actually 180
      do{
        update_line_values();
        front2_line_following();
        }while (v_leftVal==1||v_rightVal==1);

    do{
        update_line_values();
        front2_line_following();
        }while (v_leftVal==0 && v_rightVal==0);

    do{
        update_line_values();
        front2_line_following();
        }while (v_leftVal==1||v_rightVal==1);

    do{
        update_line_values();
        front2_line_following();
        }while (v_leftVal==0 && v_rightVal==0);
    right90();
      int starttime = millis();
    int endtime = starttime;

    while ((endtime - starttime)<=2100){ //tbd
      update_line_values();
      front2_line_following();
      endtime = millis();
    }   
      

    }
  
  else if (scans_completed == 2){
    left90();
    do{
        update_line_values();
        front2_line_following();
        }while (v_leftVal==0 && v_rightVal==0);
    right90();
    int starttime = millis();
    int endtime = starttime;
    while ((endtime - starttime)<=7000){ //tbd
      update_line_values();
      front2_line_following();
      endtime = millis();
    }   

  }

  else if (scans_completed == 3){
    left90();//180
    do{
        update_line_values();
        front2_line_following();
        }while (v_leftVal==0 && v_rightVal==0);
    right90();

  int starttime = millis();
    int endtime = starttime;
    while ((endtime - starttime)<=2000){ //tbd
      update_line_values();
      front2_line_following();
      endtime = millis();
  }
  }
}}

void clockwise_scan180(){//start turning clockwise and scanning for a block with the tof sensor, range is about 40cm
  bool block_found = false;
  right_motor(160,0); //turn really slowly
  Rmotor_speed = -160;
  left_motor(160,1);
  Lmotor_speed = 160;
  while(digitalRead(3)==1|| digitalRead(4)==1){ 
    if (attempt_lock_on(15)){//potential block identified while turning, stop and check for a block again more stringently, if block found collect and deliver the block, else keep scanning
      stop();
      block_found = attempt_lock_on(40);
      if(block_found){
        //go to block
        // stop();
        Serial.println("block ahead skrrrrtt!!");
        fetch_block(); //fetch block will also deliver it and then return to the same scan point 
        break;
      }
      
      else{
        
        right_motor(100,0); //start turning clockwise again, continue scanning normally
        Rmotor_speed = -100;
        left_motor(100,1);
        Lmotor_speed = 100;
      }
    }
  }
  while(digitalRead(3)==0){ //when this ends we are back on line

    
    if (attempt_lock_on(15)){//potential block
      stop();
      block_found = attempt_lock_on(40);
      if(block_found){
        Serial.println("block ahead skrrrrtt!!");
        fetch_block(); //fetch block will also deliver it and then return to the same scan point 
        break;
      }
      
      else{
        
        right_motor(100,0); //start turning clockwise again, continue scanning normally
        Rmotor_speed = -100;
        left_motor(100,1);
        Lmotor_speed = 100;
      }
    }
  }
  if(block_found=false){//only increment scans_completed if the loop finished naturally instead of being interrupted by block found
      scans_completed++;
      currentJunction = 99;
      if (scans_completed == 4){ //if we have scanned the entire zone and didn't find a block just return to the start zone and stop there
        left90();
        do{
        update_line_values();
        front2_line_following();
        }while (v_leftVal==0 && v_rightVal==0);

        right90();
        do{
        update_line_values();
        front2_line_following();
        }while (v_leftVal==1 || v_rightVal==1);

        do{
        update_line_values();
        front2_line_following();
        }while (v_leftVal==0 && v_rightVal==0);
        do{
        update_line_values();
        front2_line_following();
        }while (v_leftVal==1 || v_rightVal==1);

        do{
        update_line_values();
        front2_line_following();
        }while (v_leftVal==0 && v_rightVal==0);
        left90();

        do{
        update_line_values();
        front2_line_following();
        }while (v_leftVal==1 || v_rightVal==1);

        do{
        update_line_values();
        front2_line_following();
        }while (v_leftVal==0 && v_rightVal==0);
        do{
        update_line_values();
        front2_line_following();
        }while (v_leftVal==1 || v_rightVal==1);

        do{
        update_line_values();
        front2_line_following();
        }while (v_leftVal==0 && v_rightVal==0);

        go_straight();
        delay(300);
        stop();
        exit(0);
      }
  }
}

bool attempt_lock_on(int threshold){//function to identify blocks from time-of-flight sensor readings, the higher the threshold the more valid readings required before block found is confirmed
  int max_dist;
  if (scans_completed == 0 || scans_completed ==2){ //when scanning away from a wall, permissible range is increased
    max_dist == 430;
  }
  else
    max_dist ==410; //at scanpoints near walls, maximum range reduced so that walls are not detected as blocks
  // consistency check?
  float old_dist = call_laser();
  float sum = 0;
  for (int i=0; i<threshold; i++){
    sum = sum + old_dist;
    float new_dist = call_laser();
    if (abs(new_dist - old_dist) > 70){
      return false;}
    else{
      old_dist = new_dist;
    }
  } //consistency test passed
  if (sum/threshold < max_dist && sum/threshold > 90){
  Serial.println("lock_on confirmed");
  return true;}
  }


void fetch_block(){ //called after lock on successful, collects the block and then continues going straight to hit a line before calling function for the robot to locate itself then deliver the block 
  //logic to continually adjust to point towards the block was not finished
  bool block_acquired=false;
  bool isDense;
  float recent_distances[10] = {call_laser(), call_laser(), call_laser(), call_laser(), call_laser(),
   call_laser(), call_laser(), call_laser(), call_laser(), call_laser()}; //lol
  go_straight(); 
  // while (digitalRead(3)==1||digitalRead(3)==1){}//situation where block is on a line shouldn't happen, implement failsafe?
  while (digitalRead(3)==0 && digitalRead(4)==0){
    if(block_acquired==false){
      for(int i=0;i<9;i++)
        recent_distances[i] = recent_distances[i+1];
      recent_distances[9]=call_laser();//we may need to filter for high-reading noise, tbc
      if (recent_distances[9]<75 && recent_distances[9] > 35){ //block would be within the robot's claws at this point
        if (call_laser()<75){block_acquired=true;
        go_straight();
        delay(100); //go straight a bit more so that block settles into correct position for scanning
      
        stop();
        isDense =ultrasound();
        }
      }
      // else{ //this is incomplete, but arguably unnecessary
      //   float older_distances, newer_distances =0;
      //   for(int i =0;i<5;i++){
      //     older_distances = older_distances + recent_distances[i];
      //     newer_distances = newer_distances + recent_distances[i+5];
      //   }
      //   if((newer_distances - older_distances)/5>150){//arbitrary distance determined by the jump when block lost
      //   stop();//block lost!
      //   //find the block again by turning within a small range and comparing with older_distances
      //   }
      }
    
    go_straight();
  }
  //we've hit a line, start to seek path back somehow and then deliver (call self_locate) if correct
  stop();
  if (block_acquired == true){
    self_locate();
    offline_delivery(isDense);
    blocks_delivered++;
    if (blocks_delivered == 3){ //returns to start zone and stops after collecting 3 blocks
      reverse();
      delay(400);
      stop();
      exit(0);
    }}
  else{//didnt find block somehow, overshot? return to start and try again
    self_locate();
    right180();
    
  }
  
}

void self_locate(){ //called by fetch_block when one of the FRONT 2 sensors hits a line
  //keep travelling in a loop and taking every left turn until robot encounters the predetermined junction based on the unique line sensor readings
  //after reaching the correct junction the robot heads to the start zone 
  go_straight();
  while (digitalRead(2)==0 && digitalRead(5)==0) //continue until either of rear hits line
    continue;//then turn anticlockwise until on line
  stop();
  left90();

  while (true){
    update_line_values();
    if (v_leftVal == 1){
      if (leftVal == 1 && v_leftVal ==1||rightVal == 1 && v_leftVal ==1){//junction 10 found
      //if going back to start from here doesnt work we can call homeseeking as well
        stop();
        left90();
        do{
        update_line_values();
        front2_line_following();
        }while (v_leftVal==1||v_rightVal==1);

        do{
        update_line_values();
        front2_line_following();
        }while (v_leftVal==0 && v_rightVal==0); //stops upon reaching j11

        do{
        update_line_values();
        front2_line_following();
        }while (v_leftVal==1||v_rightVal==1); //brings us past junction 11

        do{
        update_line_values();
        front2_line_following();
        }while (v_leftVal==0 && v_rightVal==0); //stops at center junction
        
        stop();
        right90();
        do{
        update_line_values();
        front2_line_following();
        }while (v_leftVal==1||v_rightVal==1);

        do{
        update_line_values();
        front2_line_following();
        }while (v_leftVal==0||v_rightVal==0);

        do{
        update_line_values();
        front2_line_following();
        }while (v_leftVal==1||v_rightVal==1);

        do{
        update_line_values();
        front2_line_following();
        }while (v_leftVal==0||v_rightVal==0);

        stop(); 
        currentJunction = 1;
        break;
      } 
      else{ //we could make it detect junction 4 as well to save time, tbc
        left90();
        do{
          update_line_values();
          front2_line_following();
        }while(v_leftVal==1);
      }
    }
    else
      front2_line_following();
  }
}