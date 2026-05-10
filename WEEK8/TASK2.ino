#include "HUSKYLENS.h"
#include "SoftwareSerial.h"

HUSKYLENS huskylens;
SoftwareSerial mySerial(4, 5); // RX, TX

// --- Hardware Addition: RGB LED Pins ---
const int redPin = 9;
const int greenPin = 10;
const int bluePin = 11;

void setup() {
  Serial.begin(115200);
  mySerial.begin(9600);
  
  // Set RGB pins as outputs
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

  while (!huskylens.begin(mySerial)) {
    Serial.println(F("Begin failed!"));
    delay(500);
  }
  Serial.println("HuskyLens Integration Ready!");
}

void loop() {
  if (!huskylens.request()) return;
  
  // Check if HuskyLens sees an object
  if (huskylens.available()) {
    HUSKYLENSResult result = huskylens.read();
    
    if (result.command == COMMAND_RETURN_BLOCK) {
      // Switch statement to trigger physical lighting
      switch (result.ID) {
        case 1: // RED
          Serial.println("Color RED is detected");
          setColor(255, 0, 0); // Max Red, No Green, No Blue
          break;
        case 2: // BLUE
          Serial.println("Color BLUE is detected");
          setColor(0, 0, 255); // No Red, No Green, Max Blue
          break;
        case 3: // GREEN
          Serial.println("Color GREEN is detected");
          setColor(0, 255, 0); // No Red, Max Green, No Blue
          break;
        default:
          Serial.println("Unknown Object detected");
          setColor(0, 0, 0);   // LED OFF
          break;
      }
    }
  } 
  else {
    // If the camera sees nothing, turn the LED OFF
    setColor(0, 0, 0);
  }
}

// Helper function to simplify setting the LED color
void setColor(int redValue, int greenValue, int blueValue) {
  analogWrite(redPin, redValue);
  analogWrite(greenPin, greenValue);
  analogWrite(bluePin, blueValue);
}