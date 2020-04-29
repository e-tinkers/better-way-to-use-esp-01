// Serial WebClient for ESP01
// This sketch get JSON data via serial port from host controller and send
// https POST request to a web server

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>
#define MAX_LEN 55    // make sure it is sufficient for your data

char incomingMsg[MAX_LEN];
uint8_t count = 0;

// configure those parameters for your network
const char *ssid = "your-wifi-ssid";
const char *password = "your-wifi-password";
const char *url = "https://httpbin.org/post";
const uint8_t fingerprint[20] = {0xBF, 0x5C, 0x1E, 0x24, 0xA1, 0xB7, 0x73, 0xCF, 0xE6, 0xBF, 0xBC, 0xF5, 0x8A, 0x58, 0xF3, 0xC2, 0xE4, 0xB6, 0x98, 0xE1};

void sendPostRequest() {
  std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
  client->setFingerprint(fingerprint);

  // send https POST request to server
  HTTPClient https;
  https.begin(*client, url);
  https.addHeader("Content-Type", "application/json");
  int statusCode = https.POST(incomingMsg);

  // send server response http status code through serial
  char statusAscii[5];
  itoa(statusCode, statusAscii, 10);
  Serial.print(statusAscii);
  Serial.print('\n');

  https.end();
}

void setup(void) {
  Serial.begin(74880);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
  }
  Serial.print("Wifi Ready\n");
}

void loop(void) {
  while (Serial.available() > 0) {
    char c = Serial.read();
    if (c == '\n') {
      incomingMsg[count] = '\0';         // terminating null byte
      sendPostRequest();
      count = 0;                      // reset buffer pointer
      Serial.print("Going Sleep\n");
      delay(50);                      // for Serial to finish sending
      ESP.deepSleep(0);
    }
    else {
      if (count < (MAX_LEN - 1)) {
        incomingMsg[count++] = c;
      }
    }
  }
}
