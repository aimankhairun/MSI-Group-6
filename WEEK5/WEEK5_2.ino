const int relay1 = 7;
const int relay2 = 8;

void setup() 
{
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
}
void loop() 
{
  extendActuator();
  delay(5000);
  retractActuator();
  delay(5000);
}

void extendActuator()
{
  digitalWrite(relay1,HIGH);
  digitalWrite(relay2,LOW);
}

void retractActuator()
{
  digitalWrite(relay1,LOW);
  digitalWrite(relay2,HIGH);
}
