const int potPin = A0;
const int ledPin = 7;
const float Vin = 5.0;
const float R_total = 10000.0;
String command = "";

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
}

void loop() {
  int potValue = analogRead(potPin);
  float Vout = (potValue / 1023.0) * Vin;
  float R_wiper = (Vout / Vin) * R_total;

  Serial.print(Vout);
  Serial.print(" / ADC: ");
  Serial.print(potValue);
  Serial.print(" / R: ");
  Serial.println(R_wiper);

  if (Serial.available() > 0) {
    command = Serial.readStringUntil('\n');
    command.trim();

    if (command == "LED ON") {
      digitalWrite(ledPin, HIGH);
    }
    else if (command == "LED OFF") {
      digitalWrite(ledPin, LOW);
    }
  }
  delay(500);
}
