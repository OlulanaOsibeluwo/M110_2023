#include <Adafruit_MotorShield.h>

Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *left = AFMS.getMotor(3);
Adafruit_DCMotor *right = AFMS.getMotor(4);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

if (!AFMS.begin()) {         // create with the default frequency 1.6KHz
  // if (!AFMS.begin(1000)) {  // OR with a different frequency, say 1KHz
    Serial.println("Could not find Motor Shield. Check wiring.");
    while (1);
  }

  Serial.println("Motor Shield found.");

  left->setSpeed(225);
  right->setSpeed(225);
  
  left->run(RELEASE);
  right->run(RELEASE);

}

void loop() {
  // put your main code here, to run repeatedly:
  
    //Swivel 90deg ACW
    left->run(BACKWARD);
    right->run(FORWARD);
    delay(1500);
    //Swivel 180 deg CW
    left->run(FORWARD);
    right->run(BACKWARD);
    delay(3000);
    //if line of sight sensor detects block, stop at the angle and go straight
    /*if(){
      blockFound = true;}
  }while(!blockFound);*/
  
}
