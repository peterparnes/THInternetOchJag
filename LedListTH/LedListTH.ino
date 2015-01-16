/*
 Created by Peter Parnes, peter@parnes.com, 2014
 Teknikens Hus Internet och Jag exhibition 
 */
 
#include <Adafruit_NeoPixel.h>
#include <Wire.h>

#define PIN 11
#define NUMPATHS 4
#define NUMWHITEPATHS 0
#define NUMELINES 4
#define ANIMATE true
#define NUMPIX 273
#define brightness 50
#define BLUEBRIGHTNESS 255
#define WHITEBRIGHTNESS 255

// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLOR0A pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIX, PIN, NEO_GRB + NEO_KHZ800);

bool pathson[NUMPATHS];

int path0[]  = {
  32, 0
};
int path1[] = {
  15, 33
};
int path2[] = {
  19, 48
};
int path3[] = {
  20, 68
};
int path4[] = {
  6, 88
};
int path5[] = {
  36, 94
};
int path6[] = {
  15, 130
};
int path7[] = {
  15, 145
};
int path8[] = {
  13, 160
};
int path9[] = {
  7, 174
};
int path10[] = {
  10, 181
};
int path11[] = {
  29, 191
};
int path12[] = {
  5, 220
};
int path13[] = {
  10, 225
};
int path14[] = {
  37, 236
};

// Japan
int line0[]  = {
  3, 2, 1, 0
};

// West Coast 
int line1[] = {
  1, 0
};

// China 
int line2[] = {
  3, 8, 7, 6
};

// NYC 
int line3[] = {
  2, 13, 12
};

const uint32_t colors[] = {
  strip.Color(255, 0, 0), strip.Color(0, 255, 0), strip.Color(0, 255, 255), strip.Color(255, 255, 0),
  strip.Color(0, 0, brightness), strip.Color(255, 255, 255)
};

void setup() {
  Serial.begin(9600);

  strip.begin();
  strip.setBrightness(BLUEBRIGHTNESS);
  strip.show(); // Initialize all pixels to 'off'

  for (int i = 0; i < NUMPATHS; i++ ) {
    pathson[i] = true;
  }
  /* for (int i =3; i < 4; i++ ) {

      Serial.println(i);
      pathson[i] = false;
      lightPath(i, true, true);
    }*/

  /*
      for (int i = NUMPATHS; i < NUMPATHS + NUMWHITEPATHS; i++) {
        pathson[i] = false;
        lightPath(i, true, false);
      } */

  pinMode(2, OUTPUT);  // channel A
  pinMode(3, OUTPUT);  // channel B
  pinMode(4, OUTPUT);  // channel C
  pinMode(5, OUTPUT);  // channel D

  lightWhites(true);

  Wire.begin(4);                // join i2c bus with address #4
  Wire.onReceive(receiveEvent); // register event

  Serial.println("Started");
}

/**
* Light up the white paths
*/
void lightWhites(bool on) {
  for (int i = 0; i <= NUMPIX; i++) {
    if (on) {
      strip.setPixelColor(i, colors[4]);
    } else {
      strip.setPixelColor(i, 0);
    }
  }

  // Light the first pixel of each path
  strip.setPixelColor(0, colors[2]);
  strip.setPixelColor(130, colors[2]);
  strip.setPixelColor(220, colors[2]);
  strip.setPixelColor(174, colors[2]);

  strip.show();
}

/**
* Called when we receive an event over I2C (i.e. which buttons are pressed)
*/
void receiveEvent(int howMany) {
  Serial.print("receiveEvent: ");
  for (int i = 0; i < NUMPATHS; i++) {
    int x = Wire.read();    // receive byte as an integer
    Serial.print(x);         // print the integer
    Serial.print(" ");
    bool on = x > 0;
    pathson[i] = on;
  }
  Serial.println();

  updateElWire();
}

/**
* Update the ElWires (on/off)
*/
void updateElWire() {
  for (int index = 0; index < NUMPATHS; index++ ) {
    if (pathson[index]) {
      digitalWrite(2 + index, HIGH); // turn the EL channel on
    } else {
      digitalWrite(2 + index, LOW); // turn the EL channel on

    }
  }
}

/**
* Old code for turning on/off single Paths.
* Kept for testing.
*/
/*
void lightPath(int index, bool on, bool elwire, bool animate) {
  String d = "lightPath: ";
  d += index;
  d += " ";
  d += on;
  // Serial.println(d);

  if (pathson[index] == on) {
    return;
  }

  int *line = getLine(index);
  int len = line[0];
  //Serial.println(len);

  for (int i = 1; i <= len; i++) {
    int *path = getPath(line[i]);
    int len2 = path[0];
    int start = path[1];
    //Serial.print("Len: ");
    //Serial.println(len2);
    //Serial.print("Start: ");
    //Serial.println(start);
    for (int j = len2 + start; j >= path[1]; j--) {
      // Serial.println(j);
      if (on) {

        strip.setPixelColor(j, colors[4]);
        // Serial.println(j);
        if (animate) {
          strip.show();
        }

        if (elwire) {
          // Serial.println("el: on");
          digitalWrite(2 + index, HIGH); // turn the EL channel on
        }
      }
      else {
        strip.setPixelColor(j, strip.Color(0, 0, 0));
        if (elwire) {
          digitalWrite(2 + index, LOW); // turn the EL channel on
        }
      }
    }
  }
  strip.show();

  pathson[index] = on;

  if ((index == 3) && !on && (pathson[0] == true)) {
    pathson[0] = false;
    lightPath(0, true, true, false);
  }


  // Check special case with path 0 on and path 3 going off
  if ((index == 0) && !on && (pathson[3] == true)) {
    pathson[3] = false;
    lightPath(3, true, true, false);
  }
}
*/

/**
* Update all paths
*/
void lightPaths() {
  bool alloff = true;
  for (int index = 0; index < NUMPATHS; index++ ) {
    if (pathson[index]) {
      alloff = false;
      break;
    }
  }
  lightWhites(!alloff);
  if (alloff) {
    return;
  } 

    for (int index = 0; index < NUMPATHS; index++ ) {
      if (!pathson[index]) {
        continue;
      }

      int *line = getLine(index);
      int len = line[0];
      //Serial.println(len);

      for (int i = 1; i <= len; i++) {
        int *path = getPath(line[i]);
        int len2 = path[0];
        int start = path[1];
        for (int j = len2 + start; j >= path[1]; j--) {
          strip.setPixelColor(j, colors[5]);
          strip.show();
          delay(10);
          if (j == 0 || j == 130 || j == 220 || j == 174) {
            strip.setPixelColor(j, colors[2]);
          } else {
            strip.setPixelColor(j, colors[4]);
          }
          delay(10);
          strip.show();
        }
      }
    }
}

// int framecount = 0;
// long lastLightUpdate = 0;
void loop() {
  // if ((millis() - lastLightUpdate) > 500) {  // Code to limit how often we update the paths to slow down updates if needed
  lightPaths();
  //  lastLightUpdate = millis();
  //}

  /* Test code for turning on/off individual paths
   lightPath(0, true);
    lightPath(1, true);
    lightPath(2, true);
    lightPath(3, true);
    */


  /* Test code, just randomly turn on/off the paths
  if((framecount++ % 10000) == 0) {
   bool on = (random(2) > 0);
   int path = random(NUMPATHS);
   Serial.println(path);
   lightPath(path, on, true);
   }*/
}

// XXXXXXXXXXXXXXXXXXXX
// Utility functions below

int* getPath(int index) {
  switch (index) {
    case 0:
      return path0;
      break;
    case 1:
      return path1;
      break;
    case 2:
      return path2;
      break;
    case 3:
      return path3;
      break;
    case 4:
      return path4;
      break;
    case 5:
      return path5;
      break;
    case 6:
      return path6;
      break;
    case 7:
      return path7;
      break;
    case 8:
      return path8;
      break;
    case 9:
      return path9;
      break;
    case 10:
      return path10;
      break;
    case 11:
      return path11;
      break;
    case 12:
      return path12;
      break;
    case 13:
      return path13;
      break;
    case 14:
      return path14;
      break;
  }
}

int* getLine(int index) {
  switch (index) {
    case 0:
      return line0;
      break;
    case 1:
      return line1;
      break;
    case 2:
      return line2;
      break;
    case 3:
      return line3;
      break;
  }
}

