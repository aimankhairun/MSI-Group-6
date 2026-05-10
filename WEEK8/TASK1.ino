#include "HUSKYLENS.h"
#include "SoftwareSerial.h"

HUSKYLENS huskylens;
SoftwareSerial mySerial(4, 5); // RX, TX
int ledPin = 13;               // Define the LED pin

void setup() {
  Serial.begin(115200);
  mySerial.begin(9600);
  
  pinMode(ledPin, OUTPUT);     // Set Pin 13 as an Output

  while (!huskylens.begin(mySerial)) {
    Serial.println(F("Begin failed!"));
    delay(500);
  }
  Serial.println("HuskyLens Ready!");
}

void loop() {
  if (!huskylens.request()) return;

  // Check if any object is currently in the camera's view
  if (huskylens.available()) {
    HUSKYLENSResult result = huskylens.read();

    // If the detected object has ID 1, turn the LED ON
    if (result.ID == 1) {
      digitalWrite(ledPin, HIGH);
      Serial.println("Mouse detected! LED ON");
    } 
    // If a different object is detected, turn the LED OFF
    else {
      digitalWrite(ledPin, LOW);
      Serial.println("Mouse not in view. LED OFF");
    }
  } 
  // If the camera sees nothing at all, turn the LED OFF
  else {
    digitalWrite(ledPin, LOW);
  }
}