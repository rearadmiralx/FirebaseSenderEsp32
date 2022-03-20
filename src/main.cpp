#include "header.h"

void setup() {
  Serial.begin(115200);
  Ph_Init();
  Dht_Init();
  Lux_Init();
  Wifi_Init();
  Firebase_Init();
  
}

void loop() {
  firebaseLoop();
  // delay(2000);
  // Lux_Loop();
  // Serial.println();
  // // PhValue();
  // Ph_Loop();
  // TdsValue2();
  // waterTemp();
  // Serial.println(LuxValue());
}