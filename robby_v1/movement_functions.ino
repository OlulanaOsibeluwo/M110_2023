void left_motor(int rpm, int direction){
  left->setSpeed(rpm);
  if (direction == 1) {
    left->run(FORWARD);}
  else {
    left->run(BACKWARD);
  }
}

void right_motor(int rpm, int direction){
  right->setSpeed(rpm);
  if (direction == 1) {
  right->run(FORWARD);}
  else {
    right->run(BACKWARD);
  }
}


void turn_left(int rpm){
  // Adjust to the right
  left->setSpeed(rpm);
  right->setSpeed(rpm);
  left->run(FORWARD);
  right->run(BACKWARD);
}

void turn_right(int rpm){
  // Adjust to the right
  left->setSpeed(rpm);
  right->setSpeed(rpm);
  left->run(BACKWARD);
  right->run(FORWARD);
}

void forward(int rpm){
  // left_motor(rpm, 1);
  // right_motor(rpm,1);
  left->setSpeed(rpm);
  right->setSpeed(rpm);
  left->run(FORWARD);
  right->run(FORWARD);

}
void go_straight(){
  if (Lmotor_speed != 255){
    left_motor(255,1);
    Lmotor_speed = 255;}
 

  if (Rmotor_speed != 255){
    right_motor(255,1);
    Rmotor_speed = 255;}  
}

void reverse(){
  if (Lmotor_speed != -255){
    left_motor(255,0);
    Lmotor_speed = -255;}
 

  if (Rmotor_speed != -255){
    right_motor(255,0);
    Rmotor_speed = -255;}  
}

void slant_right(){
  if (Lmotor_speed != 255){
    left_motor(255,1);
    Lmotor_speed = 255;}
 
  if (Rmotor_speed != 60){
    right_motor(60,1);
    Rmotor_speed = 60;}

}
void slant_left(){
  if (Rmotor_speed != 255){
    right_motor(255,1);
    Rmotor_speed = 255;}
             
  if (Lmotor_speed != 60){
    left_motor(60,1);
    Lmotor_speed = 60;
  }
}



void stop(){
  // cuts power to motor, does not brake
  left->run(RELEASE);
  right->run(RELEASE);
  Lmotor_speed = 0;
  Rmotor_speed = 0;                               
}

void left90(){//90 degrees left turn, only works if turning onto a line
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
  // while (digitalRead(3)==0)
  // continue;
  // stop();

}
void right90(){//90 degrees right turn
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
  // while (digitalRead(4)==0)
  // continue;
  // stop();

}

void left180(){
  right_motor(150,1);
  Rmotor_speed = 150;
  left_motor(150,0);
  Lmotor_speed = -150;
  while (digitalRead(2) ==1) // turn until vright off line
    continue;
  while (digitalRead(2)==0) 
    continue;
  while (digitalRead(2)==1) 
    continue;
  while (digitalRead(2)==0||digitalRead(5)==0) 
    continue;  
  while (digitalRead(4)==0) //then back to normal line sensing
    continue;
}
void right180(){ //this does NOT work for all cases
  right_motor(150,0);
  Rmotor_speed = -150;
  left_motor(150,1);
  Lmotor_speed = 150;
  while (digitalRead(5) ==1) // turn until vright off line
    continue;
  while (digitalRead(5)==0) 
    continue;
  while (digitalRead(5)==1) 
    continue;
  while (digitalRead(5)==0) 
    continue;
  while (digitalRead(3)==0) //then back to normal line sensing
    continue;
  stop();
}