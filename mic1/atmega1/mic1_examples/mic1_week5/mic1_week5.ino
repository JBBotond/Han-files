/*
  MIC1 Week 5. Sensors and Actuators 1

  Author Janosi Barna-Botond
         2170302
  Date   03/10/2025
*/

// Include the LCD library
// Must use A4 for SDA and A5 for SCL
#include <lcd16x2_i2c.h>

// Associate the HC-SR04 sensor pins with the arduino pins
const int trigPin = 6;
const int echoPin = 7;

// Previous values
unsigned long previousMillis = 0;

// Interval at which to measure the Ping sensor
unsigned long interval = 300;

//up to 16 characters displayed

//array to represent characters
int bar[16] = {0};
//0 represents a space
#define space 0
//1 represents a square
#define square 1
//2 represents a slash
#define slash 2

//constant to represent bar size
const int barSize = 16;

//variable to track max distance
int maxDist = 1;

//declaration of sw0 button
const int buttonPin = 21;
//variables to control button
int buttonState;
int prevButtonState;

int distance_cm = 0;

//declaration of built in LED
const int led = 13;
//treshold value for LED
const int treshold = 10;

void setup() {
  Serial.begin(9600);

  // Setup HC-SR04 pins
  pinMode(echoPin, INPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(led, OUTPUT);
  
  pinMode(buttonPin, INPUT);

  digitalWrite(trigPin, LOW);
  digitalWrite(led, LOW);

  // Initialize the LCD
  lcd_init();
  lcd_backlight(on);
  lcd_set_cursor(0, 0);
  lcd_put("MIC1 Week 5");

  //initialize button values
  buttonState = digitalRead(buttonPin);
  prevButtonState = buttonState;
}

void loop() {

    //button code
  buttonState = digitalRead(buttonPin);
  if(buttonState != prevButtonState) {
    if(buttonState == LOW) {
      //remove the slash here
      maxDist = 1;
    }
    prevButtonState = buttonState;
    delay(30);
  }
  // --------------------------------------------------------------------------

  // Check to see if the interval has passed
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    // Save the current time so it can be used to calculate if the has passed
    previousMillis = currentMillis;

    // Create a 10 microsecond pulse to trigger the sensor
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    // Measure the pulse returned by the sensor
    // Set timeout to 10 ms = 10.000 microseconds
    
    //            ! this line is gold for project
    unsigned long duration = pulseIn(echoPin, HIGH, 10000);

    lcd_set_cursor(0, 1);

    // Check validity of the measured value
    if (duration == 0) {
      // Notify user of invalid measurement
      lcd_put("Distance invalid");
    } else {
      // Calculate the distance in cm
      distance_cm = duration * 0.034 / 2;
      if(distance_cm == 0 ||
        distance_cm > 100)
        lcd_put("distance invalid");
        //invalid value, do nothing
      //value is valid
      else {
        //track maximum distance
        if(distance_cm > maxDist) {
          maxDist = distance_cm;
        }
        //put a square for each cm
        for(int i = 0; i < barSize && i < distance_cm; i++) {
          bar[i] = square;
        }
        //if there is space left over
        if(distance_cm < barSize) {
          //fill the rest of the bar with spaces
          for(int i = distance_cm; i < barSize; i++)
            bar[i] = space;
        }
        //before output, make sure to put the slash into place
        bar[maxDist - 1] = slash;
        //now output the bar onto the lcd
        for(int i = 0; i < barSize; i++) {
          switch (bar[i]) {
            case space:
              lcd_put(" ");
              break;
            case square:
              lcd_put("#");
              break;
            case slash:
              lcd_put("|");
              break;
          }
            
        }
      }
    }
    Serial.print(distance_cm);
    Serial.print(',');
    Serial.println(maxDist);
    if(distance_cm <= treshold)
      digitalWrite(led, HIGH);
    else
      digitalWrite(led, LOW);
  }
}
