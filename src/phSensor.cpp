#include <Arduino.h>
#include "DFRobot_ESP_PH_WITH_ADC.h"
#include "EEPROM.h"
#include <OneWire.h>
#include <DallasTemperature.h>

// Data wire is conntec to the Arduino digital pin 4
#define ONE_WIRE_BUS 4

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);


DFRobot_ESP_PH_WITH_ADC ph;
#define ESPADC 4096.0   //the esp Analog Digital Convertion value
#define ESPVOLTAGE 3000 //the esp voltage supply value
#define PH_PIN 35    //the esp gpio data pin number
float voltage, phValue, temperature = 25;
#define sensorPin 34

int sensorValue = 0;
float tdsValue3 = 0;
float Voltage2 = 0;

void Ph_Init()
{
  EEPROM.begin(32);//needed to permit storage of calibration value in eeprom
  ph.begin();
  sensors.begin();
}

float waterTemp(){ 
  // Call sensors.requestTemperatures() to issue a global temperature and Requests to all devices on the bus
  sensors.requestTemperatures(); 
  
//   Serial.print("Celsius temperature: ");
//   // Why "byIndex"? You can have more than one IC on the same bus. 0 refers to the first IC on the wire
//   Serial.print(sensors.getTempCByIndex(0)); 
//   Serial.print(" - Fahrenheit temperature: ");
//   Serial.println(sensors.getTempFByIndex(0));
  return sensors.getTempCByIndex(0);
}

float PhValue(){
      static unsigned long timepoint = millis();
  if (millis() - timepoint > 1000U) //time interval: 1s
  {
    timepoint = millis();
    //voltage = rawPinValue / esp32ADC * esp32Vin
    voltage = analogRead(PH_PIN) / ESPADC * ESPVOLTAGE; // read the voltage
    // Serial.print("voltage:");
    // Serial.println(voltage);
    
    temperature = waterTemp();  // read your temperature sensor to execute temperature compensation
    Serial.print("temperature:");
    Serial.print(temperature);
    Serial.println("^C");

    phValue = ph.readPH(voltage, temperature); // convert voltage to pH with temperature compensation
    Serial.print("pH:");
    Serial.println(phValue);
  }
  ph.calibration(voltage, temperature); // calibration process by Serail CMD
  return (phValue);
}

float TdsValue2(){
    sensorValue = analogRead(sensorPin);
    Voltage2 = sensorValue*3/1024.0; //Convert analog reading to Voltage
    tdsValue3=(133.42*Voltage2*Voltage2*Voltage2 - 255.86*Voltage2*Voltage2 + 857.39*Voltage2)*0.3; //Convert voltage value to TDS value
    Serial.print("TDS Value = "); 
    Serial.print(tdsValue3);
    Serial.println(" ppm");
    return tdsValue3;
}