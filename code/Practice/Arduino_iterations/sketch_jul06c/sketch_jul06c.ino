#include "painlessMesh.h"

#define BUTTON_PIN 21
#define LED_PIN 17
#define BOUNCING_DELAY 50

void button_function();

Scheduler userScheduler; // to control your personal task

Task taskSendMessage( 10 , TASK_FOREVER, &button_function );

void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);

  pinMode(BUTTON_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);

  digitalWrite(LED_PIN, LOW);

  userScheduler.addTask( taskSendMessage );
  taskSendMessage.enable();
}

void loop() {
  // put your main code here, to run repeatedly:
 
}

void button_function() {
  if(digitalRead(BUTTON_PIN)) {
    delay(BOUNCING_DELAY);
    if(digitalRead(BUTTON_PIN)) {
      digitalWrite(LED_PIN, digitalRead(BUTTON_PIN)); 
    }
  }
  else {
    digitalWrite(LED_PIN, digitalRead(BUTTON_PIN));
  }
}
