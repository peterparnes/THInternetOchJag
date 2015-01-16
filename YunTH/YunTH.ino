/*
 Created by Peter Parnes, peter@parnes.com, 2014
 Teknikens Hus Internet och Jag exhibition 
 */
 
#include <Bridge.h>
#include <YunServer.h>
#include <YunClient.h>
#include <Wire.h>


// Listen on default port 5555, the webserver on the Yun
// will forward there all the HTTP requests for us.
YunServer server;

String currentports;
const int buttonPin[] = { 7, 8, 9, 10 };
const int bluePin = 11;
int lastButtonState[]  = { LOW, LOW, LOW, LOW };
long lastDebounceTime[]  = { 0, 0, 0, 0 };
int buttonState[]  = { 1, 1, 1, 1 };
int port[]  = { 0, 0, 0, 0 };
long debounceDelay = 50;    // the debounce time; increase if the output flickers
long startOnTime[] = { 0, 0, 0, 0 };
long lastSendWire = 500;

const long offdelay = 180000;

const boolean pulseBlue = true;
const int blueStart = 255;

void setup() {
  Serial.begin(9600);

  Serial.println("Starting...");

  // Bridge startup
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  Bridge.begin();
  digitalWrite(13, LOW);

  Wire.begin(); // join i2c bus (address optional for master)

  pinMode(buttonPin[0], INPUT_PULLUP);
  pinMode(buttonPin[1], INPUT_PULLUP);
  pinMode(buttonPin[2], INPUT_PULLUP);
  pinMode(buttonPin[3], INPUT_PULLUP);

  updateCurrentPorts();

  for (int i = 0; i < 4; i++) {
    startOnTime[i] = millis();
  }

  // Listen for incoming connection only from localhost
  // (no one from the external network could connect)
  server.listenOnLocalhost();
  server.begin();

  pinMode(bluePin, OUTPUT);
  if (pulseBlue) {
    analogWrite(bluePin, blueStart);
  } else {
    digitalWrite(bluePin, HIGH);
  }
}

void loop() {

  Serial.println("loop");

  for (int i = 0; i < 4; i++) {
    updateButton(i);
  }

  updateCurrentPorts();

  if((millis() - lastSendWire) > 500) {
  sendWire();
  lastSendWire = millis();
  }

  // Get clients coming from server
  YunClient client = server.accept();

  // There is a new client?
  if (client) {
    // Process request
    process(client);

    // Close connection and free resources.
    client.stop();
  }

  if (pulseBlue) {
    updateBlue();
  }

  delay(50); // Poll every 50ms
}

void updateButton(int button) {
  /// read the state of the switch into a local variable:
  int reading = !digitalRead(buttonPin[button]);

  long delta = (millis() - startOnTime[button]);
  if (port[button] && (delta > offdelay)) {
    reading = 1;
  }

  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH),  and you've waited
  // long enough since the last press to ignore any noise:

  // If the switch changed, due to noise or pressing:
  if (reading != lastButtonState[button]) {
    // reset the debouncing timer
    lastDebounceTime[button] = millis();
  }

  if ((millis() - lastDebounceTime[button]) > debounceDelay) {
    // whatever the reading is at, it's been there for longer
    // than the debounce delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != buttonState[button]) {
      buttonState[button] = reading;

      // only toggle the LED if the new button state is HIGH
      if (buttonState[button] == HIGH) {
        port[button] = !port[button];

        if (port[button]) {
          startOnTime[button] = millis();
        }
      }
    }
  }

  lastButtonState[button] = reading;
}

int blue = blueStart;
int blueStep = 4;

void updateBlue() {
  blue += blueStep;
  if (blue >= 255) {
    blueStep = -blueStep;
    blue = 255;
  } else if (blue < 40) {
    blueStep = -blueStep;
    blue = 40;
  }
  // Serial.println(blue);
  analogWrite(bluePin, blue);
}

void updateCurrentPorts() {
  currentports = "";
  for (int i = 0; i < 4; i++) {
    currentports += port[i];
    currentports += " ";
  }
  // Serial.println(currentports);
}

void process(YunClient client) {
  // read the command
  String command = client.readStringUntil('/');

  client.println("Status: 200");
  client.println("Content-type: text/javascript");
  client.println("Access-Control-Allow-Origin: *");
  // any other header
  client.println(); //mandatory blank line
  client.println(currentports); //the response body
}

void sendWire() {
  Serial.println("sendWire");
  Wire.beginTransmission(4); // transmit to device #4
  for (int i = 0; i < 4; i++) {
    Wire.write(port[i]);
  }
  Wire.endTransmission();    // stop transmitting
  Serial.println("sendWire done");
}
