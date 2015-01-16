/*
 Created by Peter Parnes, peter@parnes.com, 2014
 Teknikens Hus Internet och Jag exhibition 
 */

#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>

#define LEDPIN 6

// constants won't change. They're used here to 
// set pin numbers:
const int buttonPin = 2;     // the number of the pushbutton pin
const int ledPin =  13;      // the number of the LED pin

Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(8, 8, LEDPIN,
NEO_MATRIX_TOP     + NEO_MATRIX_RIGHT +
NEO_MATRIX_COLUMNS + NEO_MATRIX_PROGRESSIVE,
NEO_GRB            + NEO_KHZ800);

const uint16_t colors[] = {
  matrix.Color(255, 0, 0), matrix.Color(0, 255, 0), matrix.Color(0, 0, 255), matrix.Color(255, 255, 0) };

// variables will change:
int buttonState1 = 0;         // variable for reading the pushbutton status
int buttonState2 = 0;         // variable for reading the pushbutton status
int buttonState3 = 0;         // variable for reading the pushbutton status
int buttonState4 = 0;         // variable for reading the pushbutton status

const int wait = 400;
long startOnTime = 0; 

static const uint8_t PROGMEM
smile_bmp[] =
{ 
  B00111100,
  B01000010,
  B10100101,
  B10000001,
  B10100101,
  B10011001,
  B01000010,
  B00111100 }
,
neutral_bmp[] =
{ 
  B00111100,
  B01000010,
  B10100101,
  B10000001,
  B10111101,
  B10000001,
  B01000010,
  B00111100 }
,
frown_bmp[] =
{ 
  B00111100,
  B01000010,
  B10100101,
  B10000001,
  B10011001,
  B10100101,
  B01000010,
  B00111100 };

void setup() {
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);      
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);  
  pinMode(buttonPin+1, INPUT);    
  pinMode(buttonPin+2, INPUT);   
  pinMode(buttonPin+3, INPUT);    

  matrix.begin();
  matrix.setTextWrap(false);
  matrix.setBrightness(40);
  matrix.setTextColor(colors[0]);

  Serial.begin(9600); 
}

void drawSmiley(int type, int color, bool redeyes) {
  matrix.fillScreen(0);
  switch(type) {
  case 0: 
    matrix.drawBitmap(0, 0, smile_bmp, 8, 8, color);
    break;
  case 1: 
    matrix.drawBitmap(0, 0, smile_bmp, 8, 8, color);
    break;
  case 2: 
    matrix.drawBitmap(0, 0, smile_bmp, 8, 8, color);
    break;
  }
  if(redeyes) {
    matrix.drawPixel(2, 2, colors[0]);  
    matrix.drawPixel(5, 2, colors[0]);  
  }  

  matrix.show();  
}

void loop(){
  matrix.fillScreen(0);

  /*
  matrix.drawRect(0,0, 8,8, colors[2]);
   // matrix.fillRect(2,2, 4,4, LED_GREEN);
   matrix.show();  // write the changes we just made to the display
   */

  // read the state of the pushbutton value:
  buttonState1 = digitalRead(buttonPin);
  buttonState2 = digitalRead(buttonPin+1);
  buttonState3 = digitalRead(buttonPin+2);
  buttonState4 = digitalRead(buttonPin+3);

  if((buttonState1 == HIGH) && (buttonState2 == HIGH) && (buttonState3 == HIGH) && (buttonState4 == HIGH) ) {
    // if(true) {
    // drawSmiley(0, colors[1], true);
    matrix.drawRect(3,3, 2,2, colors[3]);
    matrix.drawRect(2,2, 4,4, colors[2]);
    matrix.drawRect(1,1, 6,6, colors[1]);
    matrix.drawRect(0,0, 8,8, colors[0]);
    matrix.show();  
    delay(wait);
    matrix.fillScreen(0);
    matrix.show();  
    delay(wait);
    matrix.drawRect(3,3, 2,2, colors[3]);
    matrix.show();  
    delay(wait);
    matrix.drawRect(2,2, 4,4, colors[2]);
    matrix.show();  
    delay(wait);
    matrix.drawRect(1,1, 6,6, colors[1]);
    matrix.show();  
    delay(wait);
  } 
  else if((buttonState1 != HIGH) && (buttonState2 != HIGH) && (buttonState3 != HIGH) && (buttonState4 != HIGH) ) {
    matrix.drawRect(0,0, 1,1, colors[2]);
    matrix.drawRect(7,7, 1,1, colors[2]);
    matrix.drawRect(0,7, 1,1, colors[2]);
    matrix.drawRect(7,0, 1,1, colors[2]);
    matrix.show();  

  }
  else {
    if (buttonState1 == HIGH) { 
      matrix.drawRect(0,0, 8,8, colors[0]);
    }
    if (buttonState2 == HIGH) { 
      matrix.drawRect(1,1, 6,6, colors[1]);
    }
    if (buttonState3 == HIGH) { 
      matrix.drawRect(2,2, 4,4, colors[2]);
    }
    if (buttonState4 == HIGH) { 
      matrix.drawRect(3,3, 2,2, colors[3]);
    }
    matrix.show();  

  }

  /*
  // check if the pushbutton is pressed.
   // if it is, the buttonState is HIGH:
   if (buttonState1 == HIGH) {     
   // turn LED on:    
   digitalWrite(ledPin, HIGH);  
   Serial.println("1"); 
   } 
   else {
   // turn LED off:
   digitalWrite(ledPin, LOW); 
   }
   
   
   // read the state of the pushbutton value:
   
   // check if the pushbutton is pressed.
   // if it is, the buttonState is HIGH:
   if (buttonState2 == HIGH) {     
   // turn LED on:    
   digitalWrite(ledPin, HIGH);  
   Serial.println("2"); 
   
   } 
   else {
   // turn LED off:
   digitalWrite(ledPin, LOW); 
   }
   
   
   // read the state of the pushbutton value:
   
   // check if the pushbutton is pressed.
   // if it is, the buttonState is HIGH:
   if (buttonState3 == HIGH) {     
   // turn LED on:    
   digitalWrite(ledPin, HIGH);  
   Serial.println("3"); 
   
   } 
   else {
   // turn LED off:
   digitalWrite(ledPin, LOW); 
   }
   
   
   // read the state of the pushbutton value:
   
   // check if the pushbutton is pressed.
   // if it is, the buttonState is HIGH:
   if (buttonState4 == HIGH) {     
   // turn LED on:    
   digitalWrite(ledPin, HIGH);  
   Serial.println("4"); 
   
   } 
   else {
   // turn LED off:
   digitalWrite(ledPin, LOW); 
   }
   */
}











