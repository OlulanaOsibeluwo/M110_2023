void goto_scanpoint(){//navigate from scoring zone(junction 1) to next scanpoint based on scans_completed and current no of blocks delivered
  // for now this only brings us to the first scan point (j7)
  if (digitalRead(3)==0 && digitalRead(4)==0){ //brings us there if starting off-line when skipping phase 1
    while(digitalRead(2)==0 && digitalRead(5)==0){
      go_straight();
    }
    stop();
  }
  for(int i=0; i<2; i++){
    do{
        update_line_values();
        front2_line_following();
        }while (v_leftVal==1||v_rightVal==1);

  do{
        update_line_values();
        front2_line_following();
        }while (v_leftVal==0||v_rightVal==0);}
  
  left90();
  
  
}

void clockwise_scan180(){//starts on the line, turn clockwise
  // int loops = 0;
  bool block_found = false;
  // delay(1000); //can probably get rid of this in the full loop
  right_motor(100,0); //probably needs to be really slow
  Rmotor_speed = -100;
  left_motor(100,1);
  Lmotor_speed = 100;
  while(digitalRead(3)==1|| digitalRead(4)==1){
    continue; //dont really need to check this one, but add anyway? test the field of view width
  }
  while(digitalRead(3)==0){ //when this ends we are back on line
    //call laser
    float laser_distance = call_laser();
    if (laser_distance>80 && laser_distance< 500){//potential block, should we make this 2 readings in a row?
      block_found = attempt_lock_on();
      if(block_found){
        //go to block
        stop();
        Serial.println("block ahead skrrrrtt!!");
        fetch_block(); //fetch block will also deliver it and then return to the same scan point 
        break;
      }
      // //implement wiggle function? only if we miss blocks but need to prevent turning back over line right at the start
      // else if (loops<6){ //turn back for x time
      //   right_motor(100,1); //probably needs to be really slow
      //   Rmotor_speed = 100;
      //   left_motor(100,0);
      //   Lmotor_speed = -100;
      //   loops++;
      //   int starttime = millis();
      //   int endtime = starttime;

      //   while ((endtime - starttime)<=500){//more time?
      //     float laser_distance = call_laser();
      //     if (laser_distance>80 && laser_distance< 500){//potential block, should we make this 2 readings in a row?
      //       if(attempt_lock_on()){
      //         // go to block
      //         stop();
      //         Serial.println("block ahead skrrrrtt!!");
      //         fetch_block();
      //       }
            
      //       else{//continue turning back
      //         right_motor(100,1); //probably needs to be really slow
      //         Rmotor_speed = 100;
      //         left_motor(100,0);
      //         Lmotor_speed = -100;
      //       }
      //     }
      //     endtime = millis();
      //   }
      //   stop();
      //   right_motor(100,0); //start turning clockwise again, continue scanning normally
      //   Rmotor_speed = -100;
      //   left_motor(100,1);
      //   Lmotor_speed = 100;
      // }
      else{
        stop();
        right_motor(100,0); //start turning clockwise again, continue scanning normally
        Rmotor_speed = -100;
        left_motor(100,1);
        Lmotor_speed = 100;
      }
    }
  }
  if(block_found=false)//only increment scans_completed if the loop died of natural causes
    scans_completed++;
}

bool attempt_lock_on(){
  stop();
  // consistency check?
  float x = 0;
  for (int i=0; i<10; i++){
  x = x + call_laser(); //should we filter out the values here instead of later?
  }
  //there are alternative ways to filter data that we need to try :)
  x = x/10;
  // Serial.print("x =");
  // Serial.println(x);

  if (x>80 && x< 500)//locked in
    return true;
  else
    return false;
}

void fetch_block(){ //called after lock on successful
  bool block_acquired=false;
  bool isDense;
  float recent_distances[10] = {call_laser(), call_laser(), call_laser(), call_laser(), call_laser(),
   call_laser(), call_laser(), call_laser(), call_laser(), call_laser()}; //lol
  go_straight(); //do we need to go slowly? test 
  // while (digitalRead(3)==1||digitalRead(3)==1){}//should we have a failsafe for this situation? really shouldn't happen
  while (digitalRead(3)==0 && digitalRead(3)==0){
    if(block_acquired==false){
      for(int i=0;i<9;i++)
        recent_distances[i] = recent_distances[i+1];
      recent_distances[9]=call_laser();//we may need to filter for high-reading noise, tbc
      if (recent_distances[9]<70){ //seems like this is an issue rn
        block_acquired=true; //run the ultrasound? for isDense
        stop(); //remove this or make it run further down the loop again
        isDense =ultrasound();
        
      }
      else{ //this is incomplete, but arguably unnecessary
        float older_distances, newer_distances =0;
        for(int i =0;i<5;i++){
          older_distances = older_distances + recent_distances[i];
          newer_distances = newer_distances + recent_distances[i+5];
        }
        if((newer_distances - older_distances)/5>150){//arbitrary distance determined by the jump when block lost
        stop();//block lost!
        //find the block again by turning within a small range and comparing with older_distances
        }
      }
    }
    go_straight();
  }
  //we've hit a line, start to seek path back somehow and then deliver (call self_locate) if correct
  if (block_acquired == true){
    self_locate();
    offline_delivery(isDense);
    blocks_delivered++;}
  else{//didnt find block somehow, we overshot
    self_locate();
    right180();
    //return back to mainloop here and then goto_scanpoint will be called? depends on how we implement positioning!!
  }
  
}

void self_locate(){ //called by fetch_block when one of the FRONT 2 sensors hits a line
  //run until we hit 1 of 2 key junctions and then go to junction 1 facing rear
  go_straight();
  while (digitalRead(2)==0||digitalRead(5)==0) //continue until either of rear hits line
    continue;//then turn anticlockwise until on line
  stop();
  left_motor(150,0);
  Lmotor_speed = -150;
  right_motor(150,1);
  Rmotor_speed = 150;
  while (digitalRead(3)==0||digitalRead(4)==0) //only stops when both line sensors on line
    continue;

  int start_time, end_time = millis(); 
  //for the next x period of time, line follow and take EVERY left turn until eventually we are on track??!
  while (end_time - start_time < 10000){ //probably needs to be quite long
    update_line_values();
    if (v_leftVal == 1){//turn acw until front 2 BOTH OFF then on line again, then leave the junction
      stop();
      left_motor(150,0);
      Lmotor_speed = -150;
      right_motor(150,1);
      Rmotor_speed = 150;
      while (digitalRead(3)==1||digitalRead(4)==1)
        continue;
      while (digitalRead(4)==0)
        continue;
      do{
        //line follow straight to leave the junction n not re-read the same junction
        
        update_line_values();
        front2_line_following();

      }while (v_leftVal==1);
    }  
    else
      front2_line_following();
    end_time = millis();
  } //once this loop times out ideally we are on track and we can line follow until we hit the key junctions now
  while (true){
    update_line_values();
    if (v_leftVal == 1){
      if (leftVal == 1 ||rightVal == 1){//junction 10 found
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
        }while (v_leftVal==0||v_rightVal==0); //stops at j2

        right90();
        do{
        update_line_values();
        front2_line_following();
        }while (v_leftVal==1||v_rightVal==1);

        do{
        update_line_values();
        front2_line_following();
        }while (v_leftVal==0||v_rightVal==0); //stops at j1
        
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