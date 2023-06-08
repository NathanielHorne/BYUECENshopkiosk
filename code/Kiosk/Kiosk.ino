#include <pthread.h>
#include <esp_pthread.h>
#include <LiquidCrystal_I2C.h>

// Define magic numbers
#define SERIAL_BAUD_RATE 9600
#define DELAY 1000

//Welcome string
String welcome_mesg = "Press the button to get help!";

// Setting the pins for the LEDs and the button
#define LED1 0
#define BTN_PIN 26

// Setup LCD I2C display
// NOTE: the hex address must be changed in an actual implementation
#define LCD_COLS 16
#define LCD_ROWS 2
LiquidCrystal_I2C LCD = LiquidCrystal_I2C(0x27, LCD_COLS, LCD_ROWS);

// Setup global variables for blinking function
int debounce_time = 0;
unsigned long debounce_delay = 50;
int btn_status = HIGH;
bool help_status = false;

// Function stubs
void *print_screen(void *arg);

void *serial_print(void *arg);

void setup() {
  Serial.begin(SERIAL_BAUD_RATE);
  
  // Defined the two led pins as output and the button pin as a type of input
  pinMode(LED1, OUTPUT);
  pinMode(BTN_PIN, INPUT);
  
  // Initialize LCD screen
  LCD.init();
  LCD.clear();
  LCD.backlight();
  LCD.setCursor(0, 0);
  LCD.println("Welcome!");

  pthread_t display;
  pthread_create(&display, NULL, print_screen, NULL);

  pthread_t printert;
  pthread_create(&printert, NULL, serial_print, NULL);
}

void loop() {
  digitalWrite(LED1, digitalRead(BTN_PIN));
}

void *print_screen(void *arg) {
  while(true) {
    for(int i = 0; i < welcome_mesg.length() - 1; i++) {
      LCD.clear();
      LCD.print("Welcome!");
      LCD.setCursor(0, 1);
      if ((i + (LCD_COLS - 1)) > welcome_mesg.length()) {
        break;
      }
      else {
        for(int j = i; j < i + 16; j++){    
          LCD.print(welcome_mesg[j]);
        }
      }
      delay(DELAY / 4);
    }
    delay(DELAY);
  }
}

void *serial_print(void *arg) {
  while(true) {
    Serial.println(digitalRead(BTN_PIN));
    delay(DELAY / 2);
  }
}
