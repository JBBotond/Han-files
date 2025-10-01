/*
  MIC1 Week 2. Digital

  Author Barna-Botond Janosi
         2170302
  Date   12-09-2025
*/

// Previous values
unsigned long previousMillis = 0;

int previousButtonState = 0;
int buttonState = 0;

int previousButtonState2 = 0;
int buttonState2 = 0;

int previousButtonState3 = 0;
int buttonState3 = 0;
//value to control chaser direction,
//          -1 is normal, +1 is reverse
int value = -1;

// Interval at which to chase (milliseconds)
unsigned int interval = 100;

// The pins connected to the LEDs
const int ledD1Pin = 13;
const int ledD2Pin = 12;
const int ledD3Pin = 11;
const int ledD4Pin = 10;
const int ledD5Pin = 9;
const int ledD6Pin = 8;

// The LED that is on
int led = ledD1Pin;

// The pin connected to the pushbutton
const int buttonPin = 21;
const int buttonPin2 = PD2;
const int buttonPin3 = PD3;

// Indicates if the LED chaser is stopped
bool stop = false;

//variable used for selecting a led to manipulate
// -1 is one direction, +1 is the other direction
int ledMod = -1;



void setup() {
  // Initialize the LED pins as output
  pinMode(ledD1Pin, OUTPUT);
  pinMode(ledD2Pin, OUTPUT);
  pinMode(ledD3Pin, OUTPUT);
  pinMode(ledD4Pin, OUTPUT);
  pinMode(ledD5Pin, OUTPUT);
  pinMode(ledD6Pin, OUTPUT);

  // All LEDs off
  digitalWrite(ledD1Pin, LOW);
  digitalWrite(ledD2Pin, LOW);
  digitalWrite(ledD3Pin, LOW);
  digitalWrite(ledD4Pin, LOW);
  digitalWrite(ledD5Pin, LOW);
  digitalWrite(ledD6Pin, LOW);

  // Initialize the button pin as an input
  pinMode(buttonPin, INPUT);
  pinMode(buttonPin2, INPUT_PULLUP);
  pinMode(buttonPin3, INPUT_PULLUP);

  // Read the button pin startup value
  previousButtonState = digitalRead(buttonPin);
  previousButtonState2 = digitalRead(buttonPin2);
  previousButtonState3 = digitalRead(buttonPin3);

  buttonState = digitalRead(buttonPin);
  buttonState2 = digitalRead(buttonPin2);
  buttonState3 = digitalRead(buttonPin3);

  Serial.begin(9600);
}

void loop() {

  // --------------------------------------------------------------------------

  // Check to see if it's time to select the next LED
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    // Save the current time so it can be used to calculate if the has passed
    previousMillis = currentMillis;

    if (stop == false) {
      // Write logic 0 to the pin, turning off the LED
      digitalWrite(led, LOW);

      // Select the next LED...
      led += value;

      // ... making sure to wrap around when at the end
      if (led < ledD6Pin) {
        led = ledD1Pin;
      }
      else if (led > ledD1Pin)
        led = ledD6Pin;

      // Write logic 1 to the pin, turning on the LED
      digitalWrite(led, HIGH);
    }
  }

  // --------------------------------------------------------------------------

  // Read the pushbutton input pin
  buttonState = digitalRead(buttonPin);
  buttonState2 = digitalRead(buttonPin2);
  buttonState3 = digitalRead(buttonPin3);
  // Compare the buttonState to its previous state
  if (buttonState != previousButtonState) {
    // State has changed, so save the new state
    previousButtonState = buttonState;

    // Check the button value. If the value is LOW the button was pressed
    if (buttonState == LOW) {
      // Toggle the variable 'stop' from true to false and vice versa
      stop = !stop;
    }
    // Delay a little bit to avoid bouncing
    // Only required if there is no hardware debouncing
    // delay(30);
  }
  if(buttonState2 != previousButtonState2) {
    previousButtonState2 = buttonState2;
    if(buttonState2 == LOW) {
      value = -value;
      delay(30);
    }
  }

  if(buttonState3 != previousButtonState3) {
    previousButtonState3 = buttonState3;
    if(buttonState3 == LOW) {
      if(interval == 100)
        interval = 1000;
      else if(interval == 1000)
        interval = 100;
      delay(30);
    }
  }
}