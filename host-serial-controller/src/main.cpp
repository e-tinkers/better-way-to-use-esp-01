// Host Serial Controller
// This sketch send sensor data in serialized JSON to an ESP01 web client via serial port

#include <Arduino.h>
#include <SoftwareSerial.h>
#define MAX_LEN 50
#define ESP_RESET 5    // D5 connect to ESP01 CHIP_EN (CH_PD) pin
#define SENSING_INTERVAL 600000

char incomingMsg[MAX_LEN];
uint8_t count = 0;

SoftwareSerial espSerial(2, 3);  // rx, tx

void processData() {
  Serial.println(incomingMsg);
  if (strcmp(incomingMsg, "Wifi Ready") == 0) {
    char data[55] = "{\"temp\":23.3, \"ph\":5.8, \"ec\":1.8, \"battery\":3.3}\n";
    espSerial.print(data);
    return;
  }
  if (strcmp(incomingMsg, "Going Sleep") == 0) {
    delay(SENSING_INTERVAL);
    digitalWrite(ESP_RESET, LOW);    // send a pulse to wake up ESP01
    delay(10);
    digitalWrite(ESP_RESET, HIGH);
  }
}

void setup() {
  Serial.begin(74880);
  espSerial.begin(74880);
  pinMode(ESP_RESET, OUTPUT);
  digitalWrite(ESP_RESET, HIGH);
}

void loop() {
  while (espSerial.available() > 0) {
    char c = espSerial.read();
    switch (c) {
      case '\r':
        break;
      case '\n':
        incomingMsg[count] = '\0';
        processData();
        count = 0;
        break;
      default:
        if (count < (MAX_LEN - 1)) {
          incomingMsg[count++] = c;
        }
        break;
    }
  }
}
