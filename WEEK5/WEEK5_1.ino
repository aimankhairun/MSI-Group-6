const int ENA = 6; // Control speed of motor A
const int INA = 7; // Control direction of motor A

void setup() 
{
  pinMode(ENA, OUTPUT);
  pinMode(INA, OUTPUT);
}

void loop() 
{
  digitalWrite(INA, LOW);
  analogWrite(ENA, 150);  
  delay(3000);
  digitalWrite(INA, HIGH);
  analogWrite(ENA, 150);
  delay(3000);
}
