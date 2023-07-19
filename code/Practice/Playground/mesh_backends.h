#include "painlessMesh.h"

Scheduler userScheduler; // to control your personal task
painlessMesh  mesh;

// Needed for painless library
void receivedCallback( uint32_t from, String &msg );

void newConnectionCallback(uint32_t nodeId);

void changedConnectionCallback();

void nodeTimeAdjustedCallback(int32_t offset);

void sendMessage();
