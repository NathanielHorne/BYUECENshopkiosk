#include <painlessMesh.h>
#include <LiquidCrystal_I2C.h>

// Setting the pins for the LEDs and the button
#define LED1 2
#define LED2 4
#define BTN_PIN 26

// Setup the mesh network
// ALL nodes on the network need this same information
#define   MESH_PREFIX     "shop_network"
#define   MESH_PASSWORD   "G0gadgetG0!"
#define   MESH_PORT       5555
#define   MESH_BAUD_RATE  115200

// Setup LCD I2C display
// NOTE: the hex address must be changed in an actual implementation
#define LCD_COLS 20
#define LCD_ROWS 4
LiquidCrystal_I2C LCD = LiquidCrystal_I2C(0x27, LCD_COLS, LCD_ROWS);

// Setup global variables for blinking function
int debounce_time = 0;
unsigned long debounce_delay = 50;
int btn_status = HIGH;
bool help_status = false;

// Setup painlessMesh task scheduler
Scheduler userScheduler; // to control your personal task
painlessMesh  mesh;

// User-defined vunction stub
void sendMessage() ; // Prototype so PlatformIO doesn't complain

// Setup when the function sendMessage is actually executed
Task taskSendMessage( TASK_SECOND * 1 , TASK_FOREVER, &sendMessage );

// User-defined function: sendMessage
void sendMessage() {
  String msg = "Hi from node1";
  msg += mesh.getNodeId();
  mesh.sendBroadcast( msg );
  taskSendMessage.setInterval( random( TASK_SECOND * 1, TASK_SECOND * 5 ));
}

// These next four functions are needed for painlessMesh to run
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
  // Defined the two led pins as output and the button pin as a type of input
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(BTN_PIN, INPUT_PULLUP);

  // Begins serial monitor
  Serial.begin(MESH_BAUD_RATE);

  // Lines 75 - 84 = setup the mesh network AND taskScheduler
  // Line 74 = possible error message setup
  //mesh.setDebugMsgTypes( ERROR | MESH_STATUS | CONNECTION | SYNC | COMMUNICATION | GENERAL | MSG_TYPES | REMOTE ); // all types on
  mesh.setDebugMsgTypes( ERROR | STARTUP );  // set before init() so that you can see startup messages

  mesh.init( MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT );
  mesh.onReceive(&receivedCallback);
  mesh.onNewConnection(&newConnectionCallback);
  mesh.onChangedConnections(&changedConnectionCallback);
  mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);

  userScheduler.addTask( taskSendMessage );
  taskSendMessage.enable();

  // Initialize LCD screen
  LCD.init();
  LCD.clear();
  LCD.backlight();
  LCD.setCursor(0, 0);
  LCD.println("Welcome!");
}

void loop() {
  mesh.update();
  LCD.clear();
  LCD.println("Welcome!");
  while(true) {
    if (digitalRead(BTN_PIN) != btn_status) {
      debounce_time = millis();
    }
    if (((millis() - debounce_time) > debounce_delay) && (digitalRead(BTN_PIN) == LOW)) {
      btn_status = HIGH;
      digitalWrite(LED1, btn_status);
      help_status = true;
      break;
    }
    if (((millis() - debounce_time) > debounce_delay) && (digitalRead(BTN_PIN) == HIGH)) {
      btn_status = LOW;
      digitalWrite(LED1, btn_status);
    }
  }
  while(help_status) {
    LCD.clear();
    Serial.printf("EGADS!!! I need help!\n");
    LCD.println("OH NO! HELP NEEDED!");
    digitalWrite(LED2, HIGH);
    delay(500);
    digitalWrite(LED2, LOW);
    break;
  }
}
