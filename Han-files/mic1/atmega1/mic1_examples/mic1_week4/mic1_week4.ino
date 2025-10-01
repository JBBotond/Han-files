/*
  MIC1 Week 4. Analog

  Author Janosi Barna-Botond
         2170302
  Date   26/09/2025
*/

// Previous values
unsigned long previousMillis = 0;
int previousButtonState = 0;

// Interval at which to chase (milliseconds)
unsigned long interval = 200;

// The pin connected to the pushbutton
const int buttonPin = 21;

// The pin connected to the photoresistor
int photoresistorPin = A0;

// The value read by the ADC
int photoresistorValue = 0;
int prevPhotoresistorValue = 0;

// A String to hold incoming data
String inputString = "";

// A flag that indicates if the string is complete
bool stringComplete = false;

const int led = 13;
const int ledBO = PD3;

//pin for breakout led number 2
const int ledBO2 = PD2;
//variable to indicate state of led number 2
bool ledBO2State = true;

void setup() {
  Serial.begin(9600);

  // Initialize the button pin as an input
  pinMode(buttonPin, INPUT);
  pinMode(photoresistorPin, INPUT);
  pinMode(ledBO, OUTPUT);

  // Read the button pin startup value
  previousButtonState = digitalRead(buttonPin);

  //initialize breakout led as LOW
  analogWrite(ledBO, 0);
}

void loop() {

  // --------------------------------------------------------------------------

  // Check to see if it's time for the next reading
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    // Save the current time so it can be used to calculate if the has passed
    previousMillis = currentMillis;
    photoresistorValue = analogRead(photoresistorPin);

    if(photoresistorValue < 100)
      digitalWrite(led, LOW);
    else if(photoresistorValue >= 500)
      digitalWrite(led, HIGH);

    if(abs(prevPhotoresistorValue - photoresistorValue) > 10) {
      Serial.println(photoresistorValue);
    }
    
    prevPhotoresistorValue = photoresistorValue;
  }

  // --------------------------------------------------------------------------

  // Read the pushbutton input pin
  int buttonState = digitalRead(buttonPin);

  // Compare the buttonState to its previous state
  if (buttonState != previousButtonState) {
    // State has changed, so save the new state
    previousButtonState = buttonState;

    // Check the button value. If the value is LOW the button was pressed
    if (buttonState == LOW) {
      //Serial.println("SW0 pressed");
      ledBO2State = !ledBO2State;
      digitalWrite(ledBO2, ledBO2State);
    }

    // Delay a little bit to avoid bouncing
    delay(30);
  }

  // --------------------------------------------------------------------------

  if (stringComplete == true) {
    // Clear the flag
    stringComplete = false;

    if (inputString == "5\n") {
      Serial.println("50%");
    } 
    else if(inputString[0] >= '0' || inputString[0] <= '9') {
      if(inputString[1] == '\n') {
        //convert char to int
        int number = inputString[0] - '0';
        //Serial.println(number);
        analogWrite(ledBO, map(number, 0, 9, 0, 255));
      }
    }
    //C style character conversions?
    //some other checking, string style?
    // Clear the string:
    inputString = "";
  }
}

/*
  SerialEvent occurs whenever a new data comes in the hardware serial RX. This
  routine is run between each time loop() runs, so using delay inside loop can
  delay response. Multiple bytes of data may be available.
*/
void serialEvent() {
  while (Serial.available()) {
    // Get the new byte
    char inChar = (char)Serial.read();

    // Add it to the inputString
    inputString += inChar;

    // Is the incomming character a New Line ('\n')?
    if (inChar == '\n') {
      // Set the flag
      stringComplete = true;
    }
  }
}
