#include <ArduinoWebsockets.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>

#define INTRUPT_OUT_PIN 5 // D1


const char* ssid = ".";                                  //Enter SSID
const char* password = "12345678";                          //Enter Password
const char* websockets_server = "wss://boat.cusat.me/ws?client=boat&&key=boat";  //Enter server adress
const char* sha1_server = "CE C4 21 73 B1 DF F2 C5 A4 90 EB B7 59 CE 68 DA 70 EC 3A 18";

using namespace websockets;

WebsocketsClient client;

StaticJsonDocument<100> doc;
char serial_json[30];

void connectSocket() {
  blink(LED_BUILTIN, HIGH);

  if (client.connect(websockets_server))
    pinMode(LED_BUILTIN, LOW);
}


void handle_message(String message) {
  DeserializationError error = deserializeJson(doc, message);

  // Test if parsing succeeds.
  if (error) {
    client.send(toErrorJSON(error.f_str()));
    return;
  }

  const char* data = doc["data"];

  if (data != nullptr) {
    printWithIntrupt(data, INTRUPT_OUT_PIN);
    blink(LED_BUILTIN, LOW);

  } else if (!strcmp(doc["type"], "connection")) {
    if (!strcmp(doc["status"], "connected"))
      printWithIntrupt("C", INTRUPT_OUT_PIN);
    else
      printWithIntrupt("c", INTRUPT_OUT_PIN);

  } else {
    blink(LED_BUILTIN, LOW);
    blink(LED_BUILTIN, LOW);
    blink(LED_BUILTIN, LOW);
  }
}


void communicate()
{
  if(Serial.available())
  {
    if (client.available())  
      client.send(toDataJSON(Serial.readStringUntil('\n')));
    else
      printWithIntrupt("c", INTRUPT_OUT_PIN);
  }       
}


void setup() {
  Serial.begin(9600);

  pinMode(LED_BUILTIN, LOW);
  pinMode(INTRUPT_OUT_PIN, OUTPUT);

  // Connect to wifi
  WiFi.begin(ssid, password);

  // Wait some time to connect to wifi
  for (int i = 0; i < 10 && WiFi.status() != WL_CONNECTED; i++) {
    pinMode(LED_BUILTIN, HIGH);
    delay(900);
    pinMode(LED_BUILTIN, LOW);
    delay(100);
  }

  // Check if connected to wifi
  if (WiFi.status() != WL_CONNECTED) {
    for (bool i = true; true; i = !i) {
      pinMode(LED_BUILTIN, i ? HIGH : LOW);
      delay(1000);
    }
  }

  // run callback when messages are received
  client.onMessage([&](WebsocketsMessage message) {
    if (message.data().length())
      handle_message(message.data());
  });

  client.setFingerprint(sha1_server);

  // try to connect to Websockets server
  connectSocket();
}

void loop() {
  // let the websockets client check for incoming messages
  if (client.available())
    client.poll();
  else {
    printWithIntrupt("c", INTRUPT_OUT_PIN);
    connectSocket();
  }

  communicate();
}
