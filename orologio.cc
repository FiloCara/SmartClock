#include <FastLED.h>

#define LED_PIN     7
#define NUM_LEDS    60

// Define global variables
CRGB leds[NUM_LEDS];
int turn_on[NUM_LEDS] = {0};
String mode = "clock";

// Function to fill turn_on given an array with leds to turn on
void fillArr(int* arr, int arr_length, int* turn_on) {
  for (int i=0;i<arr_length;i++) {
    turn_on[arr[i]] = arr[i]; 
  }
}

// Turn on leds in turn_on array
void ledOn() {
  for (int i=0;i<NUM_LEDS;i++) {
    if (turn_on[i] == i) {
      leds[i] = CRGB::White;
    }
  }
  FastLED.show();
}

// Function to map hour in leds
void hour2LED(int hour, int minutes, int* turn_on, bool article) {

  // Compute the reminder to get what round led to turn on
  int minute_reminder = minutes % 5;
  // Turn on round led
  if (minute_reminder < 5) {
    for (int i=0;i < NUM_LEDS;i++) {
      turn_on[i] = i;
    }
  }
  // Turn on leds depeding on minute
  switch (minutes / 5) {
    case 0:
      // It is X oclock
      // fillArr([], turn_on);
      break;
    case 1:
      break;
    case 2:
      break;
    case 3:
      break;
    case 4:
      break;
    case 5:
      break;
    case 6:
      break;
    case 7:
      break;
    case 8:
      break;
    case 9:
      break;
    case 10:
      break;
    case 11:
      break;
  }

  switch (hour) {
    case 0:

      break;
    case 1:

      break;
    case 2:

      break;
    case 3:
    
      break;
    case 4:

      break;
    case 5:
    
      break;
    case 6:

      break;
    case 7:

      break;
    case 8:
    
      break;
    case 9:
    
      break;
    case 10:

      break;
    case 11:

      break;
  }

  // scomporre minuti
  // 24 --> 20, 4
  
}

void setup() {
  // Setup Leds
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(10);  
}

void loop() {
    
  int arr[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49};
  fillArr(arr, 50, turn_on);
  ledOn();
  delay(2000);
  memset(turn_on, 0, sizeof(turn_on));
  FastLED.clear();
  FastLED.show();
  delay(2000);
//  int ind = 0;
//  for (int i=0;i<NUM_LEDS;i++) {
//  
//    leds[i] = CRGB(20,20,20);
//    FastLED.show();
//    delay(500);
//  }
 
}
  
//  leds[0] = CRGB(255, 0, 0);
//  FastLED.show();  
//  leds[1] = CRGB(0, 255, 0);
//  FastLED.show();
//  leds[2] = CRGB(0, 0, 255);
//  FastLED.show();
//  delay(500);
//
//  leds[0] = CRGB(0, 0, 255);
//  FastLED.show();  
//  leds[1] = CRGB(255, 0, 0);
//  FastLED.show();
//  leds[2] = CRGB(0, 255, 0);
//  FastLED.show();
//  delay(500);
