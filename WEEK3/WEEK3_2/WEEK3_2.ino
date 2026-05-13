#include <Servo.h> 

Servo myservo ;
const int potPin = A0;  //Potentiometer pin
const int ledPin = 7;
bool stopControl = false;
int potValue, angle;

void setup() {
  Serial.begin(9600);
  myservo.attach(9);
  pinMode(ledPin, OUTPUT);
}

void loop() {
  if (!stopControl) {
    potValue = analogRead(potPin);
    
    // Map the 0-1023 potentiometer value to a 0-180 degree angle
    angle = map(potValue, 0, 1023, 0, 180);
    myservo.write(angle);
    
    // LED control: Turn ON when the angle exceeds 90 degrees
    if (angle > 90) {
      digitalWrite(ledPin, HIGH);
    } else {
      digitalWrite(ledPin, LOW);
    }
    
    // Send the calculated angle to Python
    Serial.println(angle);
    delay(50);
  }
  
  // Listen for the stop command ('x') from Python
  if (Serial.available() > 0) {
    char command = Serial.read();
    if (command == 'x') {
      stopControl = true;
      myservo.detach();            // Safely detach the servo
      digitalWrite(ledPin, LOW);   // Turn off the LED
      Serial.println("STOP");
    }
  }
}
