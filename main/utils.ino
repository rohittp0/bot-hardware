String toDataJSON(String data)
{
  return "{\"type\": \"data\", \"data\": \"" + data + "\"}";
}

String toErrorJSON(String error)
{
  return "{\"type\": \"error\", \"data\": \"" + error + "\"}";  
}

void blink(uint8_t pin, bool final)
{
  pinMode(pin, !final);
  delay(50);
  pinMode(pin, final);
}

void intrupt(uint8_t pin)
{
  digitalWrite(pin, LOW);
  delay(5);
  digitalWrite(pin, HIGH);
  delay(10);
  digitalWrite(pin, LOW); 
}

void printWithIntrupt(const char* data, uint8_t pin){
  Serial.println(data);
   intrupt(pin);
}

void printWithIntrupt1(const char* data, uint8_t pin){
  Serial1.println(data);
   intrupt(pin);
}

void printWithBuffer(const float data){
  Serial1.print(data);
  Serial1.print('\n');
}