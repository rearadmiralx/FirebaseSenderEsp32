#include "header.h"

void setup() {
  Serial.begin(115200);

  Wifi_Init();
  Firebase_Init();
  // Ph_Init();
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