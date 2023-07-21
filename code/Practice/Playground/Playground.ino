#include <ArduinoJson.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "painlessMesh.h"

StaticJsonDocument<800> doc;

#define   MESH_PREFIX             "whateverYouLike"
#define   MESH_PASSWORD           "somethingSneaky"
#define   MESH_PORT               5555

#define   BUTTON_PIN              21
#define   LED_PIN                 17
#define   BOUNCING_DELAY          175
#define   MAX_STR_LEN             256

#define   NO_HELP_NEEDED_STRING   "No help needed."
#define   HELP_NEEDED_STRING      "I need help!"

char *node_name = (char *)calloc(MAX_STR_LEN, sizeof(char));

char *client_node_name = (char *)calloc(MAX_STR_LEN, sizeof(char));

char *no_help_needed = (char *)calloc(MAX_STR_LEN, sizeof(char));

char *help_needed = (char *)calloc(MAX_STR_LEN, sizeof(char));

char *current_msg = (char *)calloc(MAX_STR_LEN, sizeof(char));

char *client_current_msg = (char *)calloc(MAX_STR_LEN, sizeof(char));

int help_state = LOW;

int client_help_state = LOW;

Scheduler userScheduler; // to control your personal task
painlessMesh  mesh;

String get_msgs() {
  
}

// User stub
void sendMessage() {
  mesh.sendBroadcast( current_msg );
}

Task taskSendMessage( TASK_SECOND * 1 , TASK_FOREVER, &sendMessage );

// Needed for painless library
void receivedCallback( uint32_t from, String &msg ) {
  Serial.printf("startHere: Received from %u msg=%s\n", from, msg.c_str());
  
  DeserializationError error = deserializeJson(doc, msg);

  if (error) {
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.c_str());
    return;
  }
  
  client_node_name = doc["node"]; // "how many characters do I need in order to achieve 250 words in ...
  const char* client_current_msg = doc["current_msg"]; // "how many characters do I need in order to achieve 250 words in this ...
  int client_help_state = doc["help_state"]; // 5
}

void newConnectionCallback(uint32_t nodeId) {
    Serial.printf("--> startHere: New Connection, nodeId = %u\n", nodeId);
}

void changedConnectionCallback() {
  Serial.printf("Changed connections\n");
}

void nodeTimeAdjustedCallback(int32_t offset) {
    Serial.printf("Adjusted time %u. Offset = %d\n", mesh.getNodeTime(),offset);
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

void setup() {
  Serial.begin(115200);

  pinMode(BUTTON_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  no_help_needed = NO_HELP_NEEDED_STRING;

  help_needed = HELP_NEEDED_STRING;

  current_msg = no_help_needed;
  
//mesh.setDebugMsgTypes( ERROR | MESH_STATUS | CONNECTION | SYNC | COMMUNICATION | GENERAL | MSG_TYPES | REMOTE ); // all types on
  mesh.setDebugMsgTypes( ERROR | STARTUP );  // set before init() so that you can see startup messages

  mesh.init( MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT );
  mesh.onReceive(&receivedCallback);
  mesh.onNewConnection(&newConnectionCallback);
  mesh.onChangedConnections(&changedConnectionCallback);
  mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);

  userScheduler.addTask( taskSendMessage );
  taskSendMessage.enable();
}

void loop() {
  // it will run the user scheduler as well
  button_function();
  help_state_printer();
  mesh.update();
}
