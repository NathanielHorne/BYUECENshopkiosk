#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <esp_pthread.h>
#include <LiquidCrystal_I2C.h>

#define SERIAL_BAUD_RATE 9600
#define DELAY 1000
#define MAX_STR_LEN 256
#define LCD_COLS 16
#define LCD_ROWS 2

struct display_data_t {
  LiquidCrystal_I2C LCD_target = LiquidCrystal_I2C(0x27, LCD_COLS, LCD_ROWS);
  int line_num = 0;
  char message[MAX_STR_LEN];
};

display_data_t dd1;
display_data_t dd2;

void *print_LCD(void *args);
void mywait(int timeInMs);

void setup() {
  // put your setup code here, to run once:
  dd1.LCD_target = LiquidCrystal_I2C(0x27, LCD_COLS, LCD_ROWS);
  dd1.line_num = 0;
  char *line_1 = "FIRST LINE";
  for (int i = 0; i < strlen(line_1); i++) {
    dd1.message[i] = line_1[i];
  }
  dd2.LCD_target = LiquidCrystal_I2C(0x26, LCD_COLS, LCD_ROWS);
  dd2.line_num = 1;
  char *line_2 = "SECOND LINE";
  for (int i = 0; i < strlen(line_2); i++) {
    dd2.message[i] = line_2[i];
  }

  pthread_t first_line;
  pthread_create(&first_line, NULL, print_LCD, (void *)&dd1);
}

void loop() {
  // put your main code here, to run repeatedly:

}

void *print_LCD(void *args) {

  display_data_t my_data = (display_data_t *)args;
  
  while (true) {
    my_data.LCD_target.setCursor(0, my_data.line_num);
    my_data,LCD_target.print(my_data.message);
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
