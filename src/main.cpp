#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

#ifndef APSSID
#define APSSID "Ampel"
#define APPSK "ampelpasswort"
#endif

const char *ssid = APSSID;
const char *password = APPSK;

const int led = LED_BUILTIN;
bool currentLedStatus = false;

const String staticHTML = "<!DOCTYPE html>\r\n<html lang=\"en\">\r\n<head>\r\n    <meta charset=\"UTF-8\">\r\n    <meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\">\r\n    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\r\n    <title>Ampel Kontrollzentrum</title>\r\n    <style>\r\n        html, body{\r\n            margin: 0;\r\n            width: 100vw;\r\n            height: 100vh;\r\n            background: linear-gradient(90deg, #7F7FD5, #86A8E7, #91EAE4);\r\n        }\r\n        .wrapper{\r\n            margin: 0 auto;\r\n            width: 70%;\r\n            height: 100%;\r\n            display: flex;\r\n            justify-content: center;\r\n            align-items: center;\r\n        }\r\n        #toggleButton{\r\n            padding: 1em 2em;\r\n            border: none;\r\n            border-radius: 10px;\r\n            background-color: rgb(255, 255, 255, .25);\r\n            font-size: 1.5rem;\r\n            box-shadow: 5px 5px 15px 5px rgba(0, 0, 0, 0.089);\r\n            transition: all 200ms ease;\r\n        }\r\n        #toggleButton:hover{\r\n            background-color: rgba(255, 255, 255, 1);\r\n            cursor: pointer;\r\n        }\r\n    </style>\r\n</head>\r\n<body>\r\n    <div class=\"wrapper\">\r\n        <form method=\"POST\" action=\"/toggle_led/\">\r\n            <input type=\"submit\" value=\"Toggle LED\" id=\"toggleButton\">\r\n        </form>\r\n    </div>\r\n</body>\r\n</html>";

ESP8266WebServer server(80);

void toggleLed(){
  if(currentLedStatus){
    digitalWrite(led, HIGH);
    Serial.println("LED off!");
  }else{
    digitalWrite(led, LOW);
    Serial.println("LED on!");
  }

  currentLedStatus = !currentLedStatus;
}

void handleRootRequest() {
  server.send(200, "text/html", staticHTML);
}

void handleToggleRequest(){
  if (server.method() != HTTP_POST){
    return;
  }
  toggleLed();
  server.send(200, "text/html", staticHTML);
}

void setup() {
  pinMode(led, OUTPUT);
  Serial.begin(115200);
  WiFi.softAP(ssid, password);

  IPAddress serverIP = WiFi.softAPIP();
  Serial.print("Server listening on: ");
  Serial.println(serverIP);

  server.on("/", handleRootRequest);
  server.on("/toggle_led/", handleToggleRequest);

  server.begin();
  Serial.println("Server started!");

  digitalWrite(led, HIGH);
}

void loop(void) {
  server.handleClient();
}