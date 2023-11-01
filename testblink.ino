#include "Ticker.h"

bool ledState;
int counterUS;
int bonk =0;

void blink() {
  digitalWrite(8, ledState);
  ledState = !ledState;
  }

Ticker timer4(blink, 500); // changing led every 500ms



void setup() {
  pinMode(8, OUTPUT);
  
  //Pin for button
  pinMode(7, INPUT);

  Serial.begin(115200);
  delay(2000);


  Serial.println("kk here we go");
  // int button_val = digitalRead(7);
  while (digitalRead(7) == LOW) {
    Serial.println(digitalRead(7));
    // do nothing
    ;
  } 
  }

void loop() {
  timer4.start();

  while (bonk == 1){
    Serial.print("it better still b blinking...");
  }
  }



