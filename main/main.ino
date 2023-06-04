// #include <OneWire.h>
// #include <DallasTemperature.h>

// // Data wire is plugged into digital pin 2 on the Arduino
// #define ONE_WIRE_BUS 2

// #define SERIAL_INPUT_INTRUPT digitalPinToInterrupt(3)

// #define M11 8
// #define M12 9
// #define M1E 10

// #define M21 7
// #define M22 6
// #define M2E 5

// // Setup a oneWire instance to communicate with any OneWire device
// OneWire oneWire(ONE_WIRE_BUS);

// // Pass oneWire reference to DallasTemperature library
// DallasTemperature sensors(&oneWire);


// long long unsigned time = 0;

// void command_to_motor(char);

// void setup(void) {
//   sensors.begin();  // Start up the library
//   Serial.begin(9600);
//   Serial1.begin(9600);

//   attachInterrupt(SERIAL_INPUT_INTRUPT, communicate, RISING);

//   for(int i=4; i<=10;i++)
//     pinMode(i, OUTPUT);
// }

// float get_temprature() {
//   // Send the command to get temperatures
//   sensors.requestTemperatures();
//   return sensors.getTempCByIndex(0);
// }

// void communicate() {
//   if (!Serial1.available())
//     return;

//   Serial.println("Communicating...");
//   const char command = Serial1.readStringUntil('\n')[0];
    
//   Serial.println(command);
//   command_to_motor(command);    
// }

// void command_to_motor(char command) {
//   analogWrite(M1E, 255);
//   analogWrite(M2E, 255);

//   if (command == 'U') {
//     digitalWrite(M11, HIGH);
//     digitalWrite(M12, LOW);
//     digitalWrite(M21, HIGH);
//     digitalWrite(M22, LOW);        
//   } else if (command == 'D') {
//     digitalWrite(M11, LOW);
//     digitalWrite(M12, HIGH);
//     digitalWrite(M21, LOW);
//     digitalWrite(M22, HIGH);
//   } else if (command == 'R') {
//     digitalWrite(M11, HIGH);
//     digitalWrite(M12, LOW);
//     digitalWrite(M21, LOW);
//     digitalWrite(M22, HIGH);
//   } else if (command == 'L') {
//     digitalWrite(M11, LOW);
//     digitalWrite(M12, HIGH);
//     digitalWrite(M21, HIGH);
//     digitalWrite(M22, LOW);
//   } else {
//     digitalWrite(M11, LOW);
//     digitalWrite(M12, LOW);
//     digitalWrite(M21, LOW);
//     digitalWrite(M22, LOW);
//     analogWrite(M1E, 0);
//     analogWrite(M2E, 0);
//   }
// }

// void loop(void) {
//   if(millis() - time < 1000)
//     return;

//   const float temp = get_temprature();     

//   if (Serial1.availableForWrite())
//     printWithBuffer(temp);

//   // Serial.println(temp);
//   time = millis();
// }