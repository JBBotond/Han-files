/*
  MIC1 Week 6. Sensors and Actuators 2

  Author Janosi Barna-Botond
         2170302
  Date   10/10/2025
*/
// Include the EEPROM library
#include <EEPROM.h>
#include <lcd16x2_i2c.h>
#include <chip_temperature.h>

struct time {
  int h;
  int m;
  int s;
} myTime, myAlarm;

int alarmMillis = 0;
unsigned long currentMillis = 0;

// Previous values
unsigned long previousMillis = 0;
int previousButtonState = 0;

// The pin connected to the pushbutton
const int buttonPin = 21;

// EEPROM data
const int N_DATA = 3;
int data[N_DATA] = { 0, 0, 0};

// Interval at which to display the EEPROM contents (milliseconds)
unsigned long interval = 1000;

//variable used for conversions
int temp;
char foo;

// The value read from the chip temperature sensor
int16_t chipTemperature = 0;

//bool flag for alarm clock function
bool ringing = false;
//ring time in seconds
const int ringDur = 250;
int lastRing = 0;
bool lcdState = false;
//10000ms = 10s timeout
long int ringTimeout = 1000;
int ringTimer = 0;
//0 is off, 1 is on
int screenState = 1;

void numToClock(int numberSec) {
  int minute = 0;
  int second = 0;

    //case only seconds
    if (numberSec > 0 && numberSec < 60) {
        second = numberSec;
        /*
        Serial.print("00:00:");
        Serial.println(second);
        */
        lcd_put("00:00:");
        lcd_put(String(second).c_str());
    }
    //case only minutes
    else if (numberSec >= 60 && numberSec < 3600) {
        minute = numberSec / 60;
        second = (numberSec - minute) % 60;
        /*
        Serial.print("00:");
        Serial.print(String(minute).c_str());
        Serial.print(":");
        Serial.println(String(second).c_str());
        */
        lcd_put("00:");
        lcd_put(String(minute).c_str());
        lcd_put(":");
        lcd_put(String(second).c_str());
    }
    //case hours too
    else if (numberSec >= 3600) {
        int hour = 0;
        hour = numberSec /3600;
        minute = numberSec/60 - hour*60;
        second = numberSec - (hour * 3600) - (minute * 60);
        /*
        Serial.print(hour);
        Serial.print(':');
        Serial.print(minute);
        Serial.print(':');
        Serial.println(second);
        */

        lcd_put(String(hour).c_str());
        lcd_put(':');
        lcd_put(String(minute).c_str());
        lcd_put(':');
        lcd_put(String(second).c_str());
        myTime.h = hour;
    }
    myTime.m = minute;
    myTime.s = second;
}

void setup() {
  Serial.begin(9600);
  currentMillis = millis();

  // Initialize the button pin as an input
  pinMode(buttonPin, INPUT);

  // Read the button pin startup value
  previousButtonState = digitalRead(buttonPin);

  //initialize LCD
  lcd_init();
  lcd_backlight(on);
  lcd_set_cursor(0, 0);
  lcd_clear();
  numToClock(currentMillis/1000);

  //read EEPROM values
    for (unsigned int i = 0; i < N_DATA; i++) {
      // The size of an int is two bytes. Use the sizeof() preprocessor
      // operator instead.
      unsigned int address = i * sizeof(int);
      int value = 0;

      EEPROM.get(address, value);

      Serial.print(value);
      if(i == 0)
        myAlarm.h = value;
      else if(i == 1)
        myAlarm.m == value;
      else if(i == 2)
        myAlarm.s = value;
    }
    Serial.println("Alarm set to:");
    Serial.print(myAlarm.h);
    Serial.print(myAlarm.m);
    Serial.println(myAlarm.s);

    lcd_set_cursor(0,1);
    lcd_put(String(myAlarm.h).c_str());
    lcd_put(String(myAlarm.m).c_str());
    lcd_put(String(myAlarm.s).c_str());
//---------------------------------------
    alarmMillis = (myAlarm.h*3600 + myAlarm.m*60 + myAlarm.s)*1000;
}

void loop() {
  //handle button
  // Read the pushbutton input pin
  int buttonState = digitalRead(buttonPin);
  currentMillis = millis();
  if(ringing == true) {
    Serial.print("Ringing! Timer: ");
    Serial.println(ringTimer);
    ringTimer ++;
    //ringing = false;
    //handle lcd
    if(ringTimer >= alarmMillis) {
      ringing = false;
      ringTimer = 0;
      Serial.println("Stopped ringing!");
    }
    if(currentMillis - lastRing >= ringDur) {
      //interval
      if(lcdState == true) {
        lcd_backlight(off);
        lcdState = false;
        Serial.println("Off!");
        lastRing = currentMillis;
      }
      else {
        lcd_backlight(on);
        lcdState = true;
        Serial.println("On!");
        lastRing = currentMillis;
      }
    }
  }
  if (currentMillis - previousMillis >= interval) {
      // Save the current time so it can be used to calculate if the has passed
      previousMillis = currentMillis;
      lcd_clear();
      String string = String(currentMillis); 
      String *sp = &string;
      lcd_set_cursor(0,0);
      lcd_put(string.c_str());

      lcd_set_cursor(sizeof("00:00:00")+2, 0);
      //code to fetch and display chip temperature
      chipTemperature = chip_temperature_read();
      char output[20];
      String tempString = String(chipTemperature);
      int tempVar;
      for(int i = 0; i < sizeof(tempString); i++) {
        output[i] = tempString[i];
        tempVar = i;
      }
      lcd_put(output);

      char degreeS[3] = {
        (char)0xDF, 'C', '\0'
      };
      lcd_put(degreeS);
      lcd_set_cursor(0,0);
      numToClock(currentMillis/1000);
      lcd_set_cursor(0,1);
      numToClock(myAlarm.h*3600 + myAlarm.m*60 + myAlarm.s);
      // --------------------------------------------------------------------------
        lcd_set_cursor(0,0);
        numToClock(currentMillis/1000);
        //print current time every second
        // Compare the buttonState to its previous state
        if (buttonState != previousButtonState) {
          // State has changed, so save the new state
          previousButtonState = buttonState;
          // Check the button value. If the value is LOW the button was pressed
          if (buttonState == LOW) {
             alarmMillis= currentMillis;
            lcd_clear();

            Serial.print("time of button press: ");
            Serial.println(currentMillis);
            Serial.print(myTime.h);
            Serial.print(myTime.m);
            Serial.println(myTime.s);

            myAlarm.h = myTime.h;
            myAlarm.m = myTime.m;
            myAlarm.s = myTime.s;

            data[0] = myAlarm.h;
            data[1] = myAlarm.m;
            data[2] = myAlarm.s;
            
            Serial.print("Switch press, writing: ");
            Serial.print(myAlarm.h);
            Serial.print(myAlarm.m);
            Serial.println(myAlarm.s);
            for (unsigned int i = 0; i < N_DATA; i++) {
              // The size of an int is two bytes. Use the sizeof() preprocessor
              // define instead.
              unsigned int address = i * sizeof(int);
              int value = data[i];

              EEPROM.put(address, value);
            }
          }

          // Delay a little bit to avoid bouncing
          // Only required if there is no hardware debouncing
          // delay(30);
        }
        //check if it is alarm time
        if(myTime.h == myAlarm.h &&
          myTime.m == myAlarm.m &&
          myTime.s == myAlarm.s) {
            ringing = true;
        }
        if(myTime.h == myAlarm.h &&
          myTime.m == myAlarm.m &&
          myTime.s == myAlarm.s + 10) {
            ringing = false;
            lcd_backlight(on);
        }
        
    }
}
