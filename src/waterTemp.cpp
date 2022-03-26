// #include <OneWire.h>
// #include <DallasTemperature.h>

// // Data wire is conntec to the Arduino digital pin 4
// #define ONE_WIRE_BUS 4

// // Setup a oneWire instance to communicate with any OneWire devices
// OneWire oneWire(ONE_WIRE_BUS);

// // Pass our oneWire reference to Dallas Temperature sensor 
// DallasTemperature sensors(&oneWire);

// void Water_Init()
// {
//   sensors.begin();
// }

// float waterTemp(){ 
//   // Call sensors.requestTemperatures() to issue a global temperature and Requests to all devices on the bus
//   sensors.requestTemperatures(); 
  
// //   Serial.print("Celsius temperature: ");
// //   // Why "byIndex"? You can have more than one IC on the same bus. 0 refers to the first IC on the wire
// //   Serial.print(sensors.getTempCByIndex(0)); 
// //   Serial.print(" - Fahrenheit temperature: ");
// //   Serial.println(sensors.getTempFByIndex(0));
//   return sensors.getTempCByIndex(0);
// }