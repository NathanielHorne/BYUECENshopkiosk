#include <stdio.h>
#include <string.h>
#include "Playground.h"
#include "mesh_backends.h"
#include "string_declarations.h"

#define   MESH_PREFIX             "whateverYouLike"
#define   MESH_PASSWORD           "somethingSneaky"
#define   MESH_PORT               5555

#define   BUTTON_PIN              21
#define   LED_PIN                 17
#define   BOUNCING_DELAY          175

#define   NO_HELP_NEEDED_STRING   "No help needed."
#define   HELP_NEEDED_STRING      "I need help!"

Task taskSendMessage( TASK_SECOND * 1 , TASK_FOREVER, &sendMessage );

int help_state = LOW;

void setup() {
  Serial.begin(115200);

  pinMode(BUTTON_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  no_help_needed = NO_HELP_NEEDED_STRING;

  help_needed = HELP_NEEDED_STRING;

  current_msg = no_help_needed;

  userScheduler.addTask( taskSendMessage );
  taskSendMessage.enable();
}

void loop() {
  // it will run the user scheduler as well
  button_function();
  help_state_printer();
  mesh.update();
}

void button_function() {
  if(digitalRead(BUTTON_PIN)) {
    delay(BOUNCING_DELAY);
    if(digitalRead(BUTTON_PIN)) {
      help_state = !help_state;
    }
  }
  
  digitalWrite(LED_PIN, help_state); 
}

void help_state_printer() {
  if(help_state) {
    current_msg = help_needed;
  }
  else {
    current_msg = no_help_needed;
  }
  Serial.println(current_msg);
}
