/*
 * ============================================================
 *  Linear Actuator with Limit Switch + Button Switch & LED
 * ============================================================
 *  BEHAVIOR:
 *    - Actuator extends until the LIMIT SWITCH is triggered
 *    - LED turns ON, actuator STOPS
 *    - After a pause, actuator retracts until BUTTON is pressed
 *    - LED turns ON, actuator STOPS
 *    - Cycle repeats
 *
 *  WIRING:
 *    Relay 1 (IN1)         -> Pin 7
 *    Relay 2 (IN2)         -> Pin 8
 *    Limit Switch (extend) -> Pin 2  (NO contact to pin, COM to GND)
 *    Button (retract)      -> Pin 3  (one leg to pin, other to GND)
 *    LED (with 220 ohm)    -> Pin 13
 *
 *  NOTE: Both switches use INPUT_PULLUP (pressed = LOW)
 * ============================================================
 */

const int relay1 = 7;           // Relay IN1 - controls direction
const int relay2 = 8;           // Relay IN2 - controls direction
const int limitSwitch = 2;      // Limit switch - detects fully extended
const int buttonSwitch = 9;     // Button - detects fully retracted
const int ledPin = 13;          // LED indicator

void setup() 
{
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(ledPin, OUTPUT);
  
  // Both switches use internal pull-up resistors
  // Pressed = LOW, Not pressed = HIGH
  pinMode(limitSwitch, INPUT_PULLUP);
  pinMode(buttonSwitch, INPUT_PULLUP);

  // Start with everything off
  stopActuator();
  digitalWrite(ledPin, LOW);

  Serial.begin(9600);
  Serial.println("Linear Actuator + Limit Switch + Button System Ready");
}

void loop() 
{
  // --- EXTEND until limit switch is hit ---
  Serial.println("Extending...");
  digitalWrite(ledPin, LOW);
  extendActuator();

  while (digitalRead(limitSwitch) == HIGH) {
    delay(10);  // Keep extending
  }

  // Limit switch triggered!
  stopActuator();
  digitalWrite(ledPin, HIGH);
  Serial.println("Extend limit reached! LED ON, actuator stopped.");
  delay(2000);

  // --- RETRACT until button is pressed ---
  Serial.println("Retracting...");
  digitalWrite(ledPin, LOW);
  retractActuator();

  while (digitalRead(buttonSwitch) == HIGH) {
    delay(10);  // Keep retracting
  }

  // Button pressed!
  stopActuator();
  digitalWrite(ledPin, HIGH);
  Serial.println("Retract limit reached! LED ON, actuator stopped.");
  delay(2000);
}

// --- Actuator Control Functions ---

void extendActuator()
{
  digitalWrite(relay1, HIGH);
  digitalWrite(relay2, LOW);
}

void retractActuator()
{
  digitalWrite(relay1, LOW);
  digitalWrite(relay2, HIGH);
}

void stopActuator()
{
  digitalWrite(relay1, LOW);
  digitalWrite(relay2, LOW);
}
