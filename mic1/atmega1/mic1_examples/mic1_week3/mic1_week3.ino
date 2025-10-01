/*
  MIC1 Week 3. Communication

  Author Janosi Barna-Botond
         2170302
  Date   19/09/2025
*/
String studNum = "2170302";
// A String to hold incoming data
String inputString = "";
bool ok = false;

// A flag that indicates if the string is complete
bool stringComplete = false;

void setup() {
  Serial.begin(115200);
}

void loop() {

  // --------------------------------------------------------------------------

  if (stringComplete == true) {
    // Clear the flag
    stringComplete = false;
    ok = true;

    for(int i = 0; i < studNum.length() && 
        i < inputString.length() && ok; i++) {
            if(studNum[i] != inputString[i])
              ok = false;
  }
    if(ok) {
      Serial.println("Hello Boti!");
    }
    else
      Serial.println("IDK U");

    /* Serial.print("Characters received in hexadecimal:");

    for(unsigned int i=0; i<inputString.length(); i++)
    {
      Serial.print(" 0x");
      Serial.print(inputString[i], HEX);
    }

    Serial.println();
*/
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
      inChar = '\r';
      inputString += inChar;
      stringComplete = true;
    }
  }
}
