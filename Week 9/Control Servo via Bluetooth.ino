#include "BluetoothSerial.h"
#include <ESP32Servo.h>

BluetoothSerial SerialBT;
Servo myServo;

const int potPin = 34;      // Potentiometer on GPIO 34
const int servoPin = 18;    // Servo on GPIO 18
const int LED = 2;          // Built-in LED

char command;
int potValue = 0;
int angle = 0;
bool manualMode = false;    // false = pot controls servo, true = BT controls servo

void setup() 
{
  Serial.begin(9600);
  SerialBT.begin("Group 3A");
  Serial.println("Bluetooth initialized... Ready to receive commands.");
  
  pinMode(LED, OUTPUT);
  myServo.attach(servoPin);
  myServo.write(0);
}

void loop() 
{
  // Check for Bluetooth commands
  if (SerialBT.available())
  {
    command = SerialBT.read();
    Serial.print("Command received: ");
    Serial.println(command);

    switch (command)
    {
      case 'O':  // LED ON
        digitalWrite(LED, HIGH);
        break;
      case 'F':  // LED OFF
        digitalWrite(LED, LOW);
        break;
      case 'M':  // Switch to Manual mode (BT controls servo)
        manualMode = true;
        SerialBT.println("Mode: Manual (send 0-180)");
        Serial.println("Mode: Manual");
        break;
      case 'P':  // Switch to Pot mode (potentiometer controls servo)
        manualMode = false;
        SerialBT.println("Mode: Potentiometer");
        Serial.println("Mode: Potentiometer");
        break;
      default:
        // If in manual mode, parse angle from BT
        if (manualMode && command >= '0' && command <= '9')
        {
          String angleStr = String(command);
          delay(50);
          while (SerialBT.available())
          {
            char c = SerialBT.read();
            if (c >= '0' && c <= '9') angleStr += c;
            else break;
          }
          int manualAngle = angleStr.toInt();
          manualAngle = constrain(manualAngle, 0, 180);
          myServo.write(manualAngle);
          SerialBT.print("Servo set to: ");
          SerialBT.println(manualAngle);
          Serial.print("Servo set to: ");
          Serial.println(manualAngle);
        }
        break;
    }
  }

  // Potentiometer controls servo (when not in manual mode)
  if (!manualMode)
  {
    potValue = analogRead(potPin);
    angle = map(potValue, 0, 4095, 0, 180);  // ESP32 ADC = 12-bit (0-4095)
    myServo.write(angle);
  }

  // Send sensor data to phone and serial monitor
  potValue = analogRead(potPin);
  angle = map(potValue, 0, 4095, 0, 180);

  SerialBT.print("Pot: ");
  SerialBT.print(potValue);
  SerialBT.print(" | Angle: ");
  SerialBT.println(angle);

  Serial.print("Pot: ");
  Serial.print(potValue);
  Serial.print(" | Angle: ");
  Serial.println(angle);

  delay(500);
}