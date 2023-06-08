#include <pthread.h>
#include <esp_pthread.h>
#include <LiquidCrystal_I2C.h>

//Welcome string
String mesg = "Press the button to get help!";

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
  Serial.begin(9600);
  
  // Defined the two led pins as output and the button pin as a type of input
  pinMode(LED1, OUTPUT);
  pinMode(BTN_PIN, INPUT_PULLUP);
  
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
  digitalWrite(LED1, HIGH);
  delay(800);
  digitalWrite(LED1, LOW);
  delay(800);
}

void *print_screen(void *arg) {
  while(true) {
    for(int i = 0; i < mesg.length() - 1; i++) {
      LCD.clear();
      LCD.print("Welcome!");
      LCD.setCursor(0, 1);
      if ((i + 15) > mesg.length()) {
        break;
      }
      else {
        for(int j = i; j < i + 16; j++){    
          LCD.print(mesg[j]);
        }
      }
      delay(250);
    }
    delay(1000);
  }
}

void *serial_print(void *arg) {
  while(true) {
    for(int i = 0; i <= 100; i++) {
      Serial.println(i);
      delay(1000);
    }
  }
}
