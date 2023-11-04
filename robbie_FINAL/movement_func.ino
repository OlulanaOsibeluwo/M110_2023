void left_motor(int rpm, int direction){
  moving = 1;
  left->setSpeed(rpm);
  if (direction == 1) {
    left->run(FORWARD);}
  else {
    left->run(BACKWARD);
  }
}

void right_motor(int rpm, int direction){
  moving = 1;
  right->setSpeed(rpm);
  if (direction == 1) {
  right->run(FORWARD);}
  else {
    right->run(BACKWARD);
  }
}

void go_straight(){
  if (Lmotor_speed != 200){
    left_motor(200,1);
    Lmotor_speed = 200;}
 

  if (Rmotor_speed != 200){
    right_motor(200,1);
    Rmotor_speed = 200;}  
}

void reverse(){
  if (Lmotor_speed != -200){
    left_motor(200,0);
    Lmotor_speed = -200;}
 

  if (Rmotor_speed != -200){
    right_motor(200,0);
    Rmotor_speed = -200;}  
}

void slant_right(){ 
  if (Lmotor_speed != 200){
    left_motor(200,1);
    Lmotor_speed = 200;}
 
  if (Rmotor_speed != 100){
    right_motor(100,1);
    Rmotor_speed = 100;}

}
void slant_left(){
  if (Rmotor_speed != 200){
    right_motor(200,1);
    Rmotor_speed = 200;}
             
  if (Lmotor_speed != 100){
    left_motor(100,1);
    Lmotor_speed = 100;
  }
}



void stop(){
  // cuts power to motor, does not brake
  left->run(RELEASE);
  right->run(RELEASE);
  Lmotor_speed = 0;
  Rmotor_speed = 0;  
  moving = 0;
}

void left90(){//90 degrees left turn, only works if turning onto a line
  left_motor(140,0);
  Lmotor_speed = -140;
  right_motor(140,1);
  Rmotor_speed = 140;
  // note that this only works if right front sensor is not off the line in the wrong dir before turning!!!!!!
  while (digitalRead(4) ==1) // turn until right line sensor is off line
    continue;
  while (digitalRead(4)==0) //and then until on the new line
    continue;
  stop();
  
}
void right90(){//90 degrees right turn
  right_motor(140,0);
  Rmotor_speed = -140;
  left_motor(140,1);
  Lmotor_speed = 140;


  while (digitalRead(3) ==1) // turn until left line sensor off line
    continue;
  while (digitalRead(3)==0) 
    continue;
  stop(); 

}

void left180(){ //executes a 180 degrees left turn
  right_motor(140,1);
  Rmotor_speed = 140;
  left_motor(140,0);
  Lmotor_speed = -140;
  while (digitalRead(2) ==1) // turn until vright off line
    continue;
  while (digitalRead(2)==0) 
    continue;
  while (digitalRead(2)==1) 
    continue;
  while (digitalRead(2)==0||digitalRead(5)==0) 
    continue;  
  while (digitalRead(4)==0) 
    continue;
    stop();
}
void right180(){ //this does NOT work for all cases
  right_motor(140,0);
  Rmotor_speed = -140;
  left_motor(140,1);
  Lmotor_speed = 140;
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