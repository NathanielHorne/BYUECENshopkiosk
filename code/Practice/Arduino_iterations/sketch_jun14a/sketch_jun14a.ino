#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <esp_pthread.h>
#include <LiquidCrystal_I2C.h>

#define SERIAL_BAUD_RATE 9600
#define DELAY 1000
#define MAX_STR_LEN 256

struct display_data_t {
  LiquidCrystal_I2C LCD_target;
  int line_num;
  char message[MAX_STR_LEN];
};

display_data_t d_d_1;

char standby_mesg[MAX_STR_LEN] = "Press the button to get help!";
char help_mesg[MAX_STR_LEN] = "EGADS! I need help!";
char welcome_bar[MAX_STR_LEN] = "Welcome to the Electrical Engineering Experiential Learning Center";
char current_mesg[MAX_STR_LEN];

pthread_mutex_t fakeMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t fakeCond = PTHREAD_COND_INITIALIZER;

#define LCD_COLS 16
#define LCD_ROWS 2
LiquidCrystal_I2C LCD_1 = LiquidCrystal_I2C(0x27, LCD_COLS, LCD_ROWS);
LiquidCrystal_I2C LCD_2 = LiquidCrystal_I2C(0x26, LCD_COLS, LCD_ROWS);

void *print_LCD(void *args);
void mywait(int timeInMs);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(SERIAL_BAUD_RATE);
  
  // Initialize LCD screen
  LCD_1.init();
  LCD_1.clear();
  LCD_1.backlight();
  d_d_1.LCD_target = LCD_1;
  d_d_1.line_num = 0;
  d_d_1.message = "first line";
  
  LCD_2.init();
  LCD_2.clear();
  LCD_2.backlight();
  d_d_1.LCD_target = LCD_1;
  d_d_1.line_num = 0;
  d_d_1.message = "second line";


  pthread_t line_1;
  pthread_create(&line_1, NULL, print_LCD, (void *)d_d_1);

  pthread_t line_2;
  pthread_create(&printert, NULL, print_LCD, (void *)d_d_1);
}

void loop() {
  // put your main code here, to run repeatedly:

}

void *print_LCD(void *args) {

  struct display_data *my_data;
  my_data = (struct display_data *)args;
  
  while (true) {
    my_data->LCD_target.setCursor(0, my_data->line_num);
    my_data->LCD_target.print(my_data->message);
    mywait(DELAY / 4); 
  }
}

void mywait(int timeInMs) {
  
    struct timespec timeToWait;
    struct timeval now;
    int rt;

    gettimeofday(&now,NULL);

    // Edited this one line to work more like the original wait function
    timeToWait.tv_sec = now.tv_sec+(timeInMs/1000UL);
    timeToWait.tv_nsec = (now.tv_usec+1000UL*timeInMs)*1000UL;

    pthread_mutex_lock(&fakeMutex);
    rt = pthread_cond_timedwait(&fakeCond, &fakeMutex, &timeToWait);
    pthread_mutex_unlock(&fakeMutex);
}
