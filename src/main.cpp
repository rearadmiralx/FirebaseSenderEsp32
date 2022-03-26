#include "header.h"

void setup() {
  Serial.begin(115200);
  WaterLevel_Init();
  Ph_Init();
  Dht_Init();
  Lux_Init();
  Wifi_Init();
  Firebase_Init();
}

void loop() {
  delay(5000);
  firebaseLoop();
}