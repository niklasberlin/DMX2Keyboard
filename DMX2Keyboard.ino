// - - - - -
//
// DMX to Keyboard
// February 2019 by Niklas Aumüller (http://niklas-aumueller.de)
//
//
// DmxSerial - A hardware supported interface to DMX.
// DMXSerial-Library:
// Copyright (c) 2011-2015 by Matthias Hertel, http://www.mathertel.de
// This work is licensed under a BSD style license. See http://www.mathertel.de/License.aspx
//
//
//
// - - - - -

#include <DMXSerial.h>
#include "Keyboard.h"


//######### CONFIG ################//
short DMXStart = 501; //Startadress, make sure that it is possible to read all Adresses
const byte AdressCount = 12;
byte Keys[AdressCount] = {KEY_F1, KEY_F2, KEY_F3, KEY_F4, KEY_F5, KEY_F6, KEY_F7, KEY_F8, KEY_F9, KEY_F10, KEY_F11, KEY_F12};
//byte Keys[AdressCount] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l'};
int timeout = 1000; //Timeout to release the a key after some time, default 1000 -> 1 second

//######### DO NOT CHANGE BELOW THIS POINT #########//
byte lastFrame[AdressCount]; //Space for DMX Values from the last frame
unsigned int lastPressed[AdressCount]; //used to store the time when the key was pressed the last time to release the key after a timeout

void setup () {
  DMXSerial.init(DMXReceiver);
}


boolean initialized = false;
void loop() {
  byte currentFrame[AdressCount];
  //read the current DMX Input into buffer
  for (byte i = 0; i < AdressCount; i++) {
    currentFrame[i] = DMXSerial.read(DMXStart + i);
  }

  //check if we have an relevant change
  if (initialized) {
    for (byte i = 0; i < AdressCount; i++) {
      if (currentFrame[i] > 127 && lastFrame[i] <= 127) { //check if we got above the treshold from last to current frame
        Keyboard.press(Keys[i]);
        lastPressed[i] = millis();
      } else if (currentFrame[i] < 127 && lastFrame[i] >= 127) { //if we got below the treshold -> release the key
        Keyboard.release(Keys[i]);
      } else if (currentFrame[i] > 127 && lastPressed[i] + timeout < millis()) { //if the key is already pressed longer than the defined timeout -> release the key
        Keyboard.release(Keys[i]);
        lastPressed[i] = millis() + 5000; //add 5000 milliseconds so that the release will be triggert every 5 seconds when the channel is still high
      }
    }

  }
  memcpy(lastFrame, currentFrame, sizeof(lastFrame));
  initialized = true;
}
