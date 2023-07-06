#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <esp_pthread.h>
#include "painlessMesh.h"

#define BUTTON_PIN 21
#define LED_PIN 17
#define BOUNCING_DELAY 50

#define   MESH_PREFIX     "whateverYouLike"
#define   MESH_PASSWORD   "somethingSneaky"
#define   MESH_PORT       5555

painlessMesh  mesh;

int btn_last_status = LOW;
unsigned long initial_time;

void *sendMessage(void *args) ; // Prototype so PlatformIO doesn't complain

void *stupid_function(void *args);

// Needed for painless library
void receivedCallback( uint32_t from, String &msg ) {
  Serial.printf("startHere: Received from %u msg=%s\n", from, msg.c_str());
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

void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);
  pinMode(BUTTON_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);

  //mesh.setDebugMsgTypes( ERROR | MESH_STATUS | CONNECTION | SYNC | COMMUNICATION | GENERAL | MSG_TYPES | REMOTE ); // all types on
  mesh.setDebugMsgTypes( ERROR | STARTUP );  // set before init() so that you can see startup messages

  pthread_t mesh_thread; 
  
  mesh.init( MESH_PREFIX, MESH_PASSWORD, &mesh_thread, MESH_PORT );
  mesh.onReceive(&receivedCallback);
  mesh.onNewConnection(&newConnectionCallback);
  mesh.onChangedConnections(&changedConnectionCallback);
  mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);

  pthread_create(&mesh_thread, NULL, sendMessages, NULL);

  pthread_t first_thread;

  pthread_create(&first_thread, NULL, stupid_function, NULL);

}

void loop() {
  // put your main code here, to run repeatedly:
//  stupid_function();
  delay(BOUNCING_DELAY);
}

void *stupid_function(void *args) {
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

void *sendMessage(void *args) {
  String msg = "Hello from node ";
  msg += mesh.getNodeId();
  mesh.sendBroadcast( msg );
  taskSendMessage.setInterval( random( TASK_SECOND * 1, TASK_SECOND * 5 ));
}
