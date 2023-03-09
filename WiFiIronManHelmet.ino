#include <ESP8266WiFi.h>

#define APSSID "jarvis"
#define APPSK  "iamironman"
#define LED_EYES 0
#define MASK     2

IPAddress myIP;
const char* ssid = APSSID;
const char* password = APPSK;

WiFiServer server(80);

void setup() {
  Serial.begin(115200);


  // Prepare pins
  pinMode(LED_EYES, OUTPUT);
  pinMode(MASK, OUTPUT);
  digitalWrite(LED_EYES, LOW);
  digitalWrite(MASK, LOW);

  // Setup AP
  Serial.println();
  Serial.println();
  Serial.print(F("Setting up AP, ssid: "));
  Serial.println(ssid);
  WiFi.softAP(ssid, password);

  // Start the web server
  server.begin();
  Serial.println(F("Web server started"));

  // Print the IP address
  myIP = WiFi.softAPIP();
  Serial.println(myIP);
}

void loop() {
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) { return; }
  Serial.println(F("new client"));
  client.setTimeout(5000);  // default is 1000

  // Read the first line of the request
  String req = client.readStringUntil('\r');
  Serial.println(F("request: "));
  Serial.println(req);

  // Match the request
  if (req.indexOf(F("/eyes")) != -1) {
    digitalWrite(LED_EYES, HIGH);
    delay(500);
    digitalWrite(LED_EYES, LOW);   
  } else if (req.indexOf(F("/mask")) != -1) {
    digitalWrite(MASK, HIGH);
    delay(500);
    digitalWrite(MASK, LOW);
  } else {
    Serial.println(F("invalid request"));
  }
  while (client.available()) {
    client.read();
  }
  client.print(F("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\n Iron Man Helmet"));
  client.print(F("<br><br>Click <a href='http://"));
  client.print(myIP);
  client.print(F("/eyes'>here</a> to switch LED eyes on, or off. <br><br> Click <a href='http://"));
  client.print(myIP);
  client.print(F("/mask'>here</a> to open or close mask.</html>"));
  Serial.println(F("Disconnecting from client"));
}
