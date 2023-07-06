#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <esp_pthread.h>
#include <LiquidCrystal_I2C.h>

// Define magic numbers
#define TOTAL_ROWS 9
#define SERIAL_BAUD_RATE 9600
#define DELAY 1000
#define MAX_STR_LEN 256
#define LCD_COLS 16
#define LCD_ROWS 2
// Define I2C addresses for each LCD
#define LCD_1_ADD 0x27
#define LCD_2_ADD 0x26
#define LCD_3_ADD 0x25
#define LCD_4_ADD 0x24



// Setup struct for later threaded function
typedef struct {
  
  // This is the GLOBAL declaration; how far from the top is this line compared to the others in a GENERAL sense
  int row_num = 0;

  // This is the LOCAL declaration; on EACH LCD, is this the first or second row
  // Eventually, we'll do row_num % 2 in order to find which row it'll end up being
  int line_num = 0;

  // Arduino and the ESP32 REALLY don't like it if you leave this undeclared.
  // So. This is the boilerplate.
  LiquidCrystal_I2C LCD = LiquidCrystal_I2C(LCD_1_ADD, LCD_COLS, LCD_ROWS);
  
} disp_data_t;

// Function stubs
void *line_printer(void *args);

void setup() {
  // put your setup code here, to run once:
  
  // Initialize that space as char *
  for (int i = 0; i < TOTAL_ROWS; i++)
  {
      row[i] = (char *)calloc(MAX_STR_LEN, sizeof(char));
  }

  // Initialize each of those char *s as the strings we decide in advance
  row[0] = "first line";
  row[1] = "second line";
  row[2] = "third line";
  row[3] = "fourth line";
  row[4] = "fifth line";
  row[5] = "sixth line";
  row[6] = "seventh line";
  row[7] = "eighth line";
  row[8] = "Getting help, standby";

  // Create array of display_data_t structs
  // This is good for when we pass arguments to threaded functions in the future.
  disp_data_t disp_data[TOTAL_ROWS];

  /* 
   *  For loop that assigns each struct (carrying the data for each line)
   *  their respective line numbers, local LCD line numbers,
   *  and initializes their LCD object
  */ 
  for (int i = 0; i < TOTAL_ROWS; i++) {
    disp_data[i].row_num = i;
    disp_data[i].line_num = disp_data[i].row_num % 2;
    if (i == 0 || i == 1) {
      disp_data[i].LCD = LiquidCrystal_I2C(LCD_1_ADD, LCD_COLS, LCD_ROWS);
    }
    else if (i == 2 || i == 3) {
      disp_data[i].LCD = LiquidCrystal_I2C(LCD_2_ADD, LCD_COLS, LCD_ROWS);
    }
    else if (i == 4 || i == 5) {
      disp_data[i].LCD = LiquidCrystal_I2C(LCD_3_ADD, LCD_COLS, LCD_ROWS);
    }
    else {
      disp_data[i].LCD = LiquidCrystal_I2C(LCD_4_ADD, LCD_COLS, LCD_ROWS);
    }

    // Once all the rows for a specific screen are initialized, setup that screen
    if(i % 2) {
      (disp_data[i].LCD).init();
      (disp_data[i].LCD).clear();
      (disp_data[i].LCD).backlight();
    }
  }

  for (int i = 0; i < TOTAL_ROWS; i++) {
    if(!(i%2)) {
      (disp_data[i].LCD).setCursor(0, 0);      
    }
    else {
      (disp_data[i].LCD).setCursor(0, 1);
    }
    (disp_data[i].LCD).print(row[i]);
  }
  
  free(row);
}

void loop() {
  // put your main code here, to run repeatedly:
}

void *line_printer(void *args) {
  
}

// Credit for function: Furquan and andrewrk in Stack Overflow thread https://stackoverflow.com/questions/1486833/pthread-cond-timedwait
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
