#include <Arduino.h>
#include "DFRobot_ESP_PH_WITH_ADC.h"
#include "EEPROM.h"


DFRobot_ESP_PH_WITH_ADC ph;
#define ESPADC 4096.0   //the esp Analog Digital Convertion value
#define ESPVOLTAGE 3000 //the esp voltage supply value
#define PH_PIN 35    //the esp gpio data pin number
float voltage, phValue, temperature = 25;

#define SERIAL Serial
#define sensorPin 34

int sensorValue = 0;
float tdsValue3 = 0;
float Voltage2 = 0;

void Ph_Init()
{
  EEPROM.begin(32);//needed to permit storage of calibration value in eeprom
  ph.begin();
}

void Ph_Loop()
{
  static unsigned long timepoint = millis();
  if (millis() - timepoint > 1000U) //time interval: 1s
  {
    timepoint = millis();
    //voltage = rawPinValue / esp32ADC * esp32Vin
    voltage = analogRead(PH_PIN) / ESPADC * ESPVOLTAGE; // read the voltage
    Serial.print("voltage:");
    Serial.println(voltage);
    
    //temperature = readTemperature();  // read your temperature sensor to execute temperature compensation
    Serial.print("temperature:");
    Serial.print(temperature);
    Serial.println("^C");

    phValue = ph.readPH(voltage, temperature); // convert voltage to pH with temperature compensation
    Serial.print("pH:");
    Serial.println(phValue);
  }
  ph.calibration(voltage, temperature); // calibration process by Serail CMD
}

float readTemperature()
{
  //add your code here to get the temperature from your temperature sensor
  
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
    
    // temperature = readTemperature();  // read your temperature sensor to execute temperature compensation
    // Serial.print("temperature:");
    // Serial.print(temperature);
    // Serial.println("^C");

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
    SERIAL.print("TDS Value = "); 
    SERIAL.print(tdsValue3);
    SERIAL.println(" ppm");
    return tdsValue3;
}