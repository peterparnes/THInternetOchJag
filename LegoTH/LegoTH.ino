/*
 Created by Peter Parnes, peter@parnes.com, 2014
 Teknikens Hus Internet och Jag exhibition 
 */
 
const int buttonPin = 10;     // the number of the pushbutton pin

// Variables will change:
int ledState = LOW;         // the current state of the output pin
int buttonState;             // the current reading from the input pin
int lastButtonState = LOW;   // the previous reading from the input pin

// the following variables are long's because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long lastDebounceTime = 0;  // the last time the output pin was toggled
long debounceDelay = 50;    // the debounce time; increase if the output flickers

long startOnTime = 0; 

int blinkTime = 2000;
const bool blink = true;
const long offdelay = 180000;

void setup() {
  pinMode(buttonPin, INPUT);

  // The EL channels are on pins 2 through 9
  // Initialize the pins as outputs
  pinMode(2, OUTPUT);  // channel A  
  pinMode(3, OUTPUT);  // channel B   
  pinMode(4, OUTPUT);  // channel C
  pinMode(5, OUTPUT);  // channel D    
  pinMode(6, OUTPUT);  // channel E
  pinMode(7, OUTPUT);  // channel F
  pinMode(8, OUTPUT);  // channel G
  pinMode(9, OUTPUT);  // channel H

  pinMode(13, OUTPUT);  
  
  Serial.begin(9600);
}

void loop(){
  /// read the state of the switch into a local variable:
  int reading = digitalRead(buttonPin);
  
  long delta = (millis() - startOnTime);
  Serial.println(delta);
  if(ledState && (delta > offdelay)) {
    reading = 1;
  }
  
  // check to see if you just pressed the button 
  // (i.e. the input went from LOW to HIGH),  and you've waited 
  // long enough since the last press to ignore any noise:  

  // If the switch changed, due to noise or pressing:
  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  } 

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer
    // than the debounce delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != buttonState) {
      buttonState = reading;

      // only toggle the LED if the new button state is HIGH
      if (buttonState == HIGH) {
        ledState = !ledState;

        if(ledState) {
          startOnTime = millis(); 
        }
      }
    }
  }

  setElWires(ledState);

  // save the reading.  Next time through the loop,
  // it'll be the lastButtonState:
  lastButtonState = reading;
}

void setElWires(bool on) {
  long delta = millis() - startOnTime; 
  if(on) {
    if(blink) {
      if((delta % blinkTime) < blinkTime/2) {
        for (int x=2; x<=9; x++) {
          digitalWrite(x, HIGH);   // turn the EL channel on
        } 
      } 
      else {
        // delay(100);              // wait for 1/10 second
        for (int x=2; x<=9; x++) {
          digitalWrite(x, LOW);    // turn the EL channel off
        } 
        // delay(100);              // wait for 1/10 second
      }
    } else {
      for (int x=2; x<=9; x++) {
          digitalWrite(x, HIGH);   // turn the EL channel on
        } 
    }
  } 
  else {
    for (int x=2; x<=9; x++) {
      digitalWrite(x, LOW);    // turn the EL channel off
    } 
  }
}













