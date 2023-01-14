#include <OneWire.h>
#include <DallasTemperature.h>

// Data wire is plugged into digital pin 2 on the Arduino
#define ONE_WIRE_BUS 2

#define SERIAL_INPUT_INTRUPT digitalPinToInterrupt(3)

#define TRIG0 5
#define ECHO0 6

#define TRIG1 7
#define ECHO1 8

#define M11 22
#define M12 24
#define M1E 9

#define M21 26
#define M22 28
#define M2E 10

#define MIN_DISTANCE 40

// Setup a oneWire instance to communicate with any OneWire device
OneWire oneWire(ONE_WIRE_BUS);

// Pass oneWire reference to DallasTemperature library
DallasTemperature sensors(&oneWire);

volatile bool connected = false;
volatile char command = '\0';

void command_to_motor(char);

void setup(void) {
  sensors.begin();  // Start up the library
  Serial.begin(9600);
  Serial1.begin(9600);

  attachInterrupt(SERIAL_INPUT_INTRUPT, communicate, RISING);

  pinMode(TRIG0, OUTPUT);
  pinMode(ECHO0, INPUT);

  pinMode(TRIG1, OUTPUT);
  pinMode(ECHO1, INPUT);

  pinMode(M11, OUTPUT);
  pinMode(M12, OUTPUT);
  pinMode(M1E, OUTPUT);

  pinMode(M21, OUTPUT);
  pinMode(M22, OUTPUT);
  pinMode(M2E, OUTPUT);
}

float get_temprature() {
  // Send the command to get temperatures
  sensors.requestTemperatures();
  return sensors.getTempCByIndex(0);
}

float get_distance(bool side) {
  intrupt(side ? TRIG0 : TRIG1);
  float distance = pulseIn(side ? ECHO0 : ECHO1, HIGH, 64000) / 58.2;

  return distance < 1 ? 10000 : distance;
}

void communicate() {
  Serial.println("Communicating...");

  if (Serial1.available()) {
    const char prev_command = command;
    command = Serial1.readStringUntil('\n')[0];

    Serial.println(command);

    if (command != 'c' || command != 'C')
      return;

    connected = command != 'c';

    command = connected ? prev_command : '\0';

    command_to_motor(command);    
  }
}

void set_motor(bool motor, int8_t direction, uint8_t speed = 128) {
  Serial.println(String("Motor: ") + motor + ", Direction: " + direction + ", Speed: " + speed);
  digitalWrite(motor ? M21 : M11, direction == 1 ? HIGH : LOW);
  digitalWrite(motor ? M22 : M12, direction == -1 ? HIGH : LOW);
  analogWrite(motor ? M2E : M1E, direction ? speed : 0);
}

void autopilot() {
  float distance0 = get_distance(0);
  float distance1 = get_distance(1);

  if (distance0 > MIN_DISTANCE && distance1 > MIN_DISTANCE) {
    set_motor(0, 1);
    set_motor(1, 1);
    return;
  }

  if (distance0 < MIN_DISTANCE * 0.8)
    set_motor(0, -1);
  else if (distance0 < MIN_DISTANCE)
    set_motor(0, 0);

  if (distance1 < MIN_DISTANCE * 0.8)
    set_motor(1, -1);
  else if (distance1 < MIN_DISTANCE)
    set_motor(1, 0);
}

void command_to_motor(char command) {
  if (command == 'U') {
    set_motor(0, 1);
    set_motor(1, 1);
  } else if (command == 'D') {
    set_motor(0, -1);
    set_motor(1, -1);
  } else if (command == 'R') {
    set_motor(0, -1);
    set_motor(1, 1);
  } else if (command == 'L') {
    set_motor(0, 1);
    set_motor(1, -1);
  } else {
    set_motor(0, 0);
    set_motor(1, 0);
  }
}

void loop(void) {
  // if (command == '\0' || !connected)
  //   return autopilot();

  delay(100);

  if (!connected)
    return;

  if (Serial1.availableForWrite())
    printWithBuffer(get_temprature());

  Serial.println(get_temprature());
  delay(400);
}