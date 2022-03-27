#include <Arduino.h>
#include "addons/TokenHelper.h" //Provide the token generation process info.
#include "addons/RTDBHelper.h"  //Provide the RTDB payload printing info and other helper functions.
#include "header.h"

unsigned long sendDataPrevMillis1;

FirebaseData fbdo;                                            // Firebase Realtime Database Object
FirebaseData stream, streamSensor, streamTimer, streamConfig; // Firebase Realtime Database Object
FirebaseAuth auth;                                            // Firebase Authentication Object
FirebaseConfig config;                                        // Firebase configuration Object
String fuid,Premix = "";                                             // Firebase Unique Identifier


bool isAuthenticated = false;
bool wakeup = true;

FirebaseJson relayPump_json, relayFan_json, relayNutrientB_json, relayNutrientA_json, // Firebase json
    relayLight_json, relayphUp_json, relayPhDown_json;

FirebaseJson sensorHumidity_json, sensorLux_json, sensorPhLevel_json, sensorTds_json,
    sensorTemp_json, sensorWaterTemp_json;

// Pin Declaration
int pinRelayPump = 16;
int pinRelayPumpReso = 5;
int pinRelayLight = 25;
int pinRelayPhUp = 27;
int pinRelayPhDown = 14;
int pinRelayNutrientA = 26;
int pinRelayNutrientB = 17;
int pinRelayFan = 33;
int pinRelayWater = 4;

boolean isSpraying = false;
boolean shutdown = false;
volatile bool dataChanged = false;
unsigned long sendDataPrevMillis = 0;
unsigned long interval = 10000;
unsigned long restartTimer = 2100000;

unsigned long previousMillis, previousMillis2, minute, second, check = 0;
int countTimer, countSensor, count, shutdownCount, ConfigCount,premixCount  = 0;

// location path
String pathRelay = "relays";
String pathTimer = "timer";
String pathSensor = "sensor";
String pathConfig = "controlConfig";

//  Global variables
unsigned long elapsedMillis2 = 0;       // Stores the elapsed time from device start ups
unsigned long update_interval2 = 10000; //
String temp , tempWaterLevel = "";
String waterLevelVal = "HIGH";
bool UserDefine, ControlRelay = "";
bool pumpResoValue, pumpValue, lightValue, nutrientaValue, nutrientbValue,nutriAddedValue,
    phUpValue, phDownValue, fanValue, waterValue, Mixing, relayConfigValue, defineValue, preMixValue;

float luxValue, humidityValue, waterTempValue, tempValue,
    tdsValue, phLevelValue, timerOnValue, timerOffValue;

float tempHum, tempTemp, tempLux, tempPh, tempWaterTemp, tempTds, tempMinVal, tempMaxVal, humMinVal, humMaxVal, luxMinVal, luxMaxVal,
    phLevelMinVal, phLevelMaxVal, ppmMinVal, ppmMaxVal, waterMinVal, waterMaxVal;

String childPath[9] = {"/relayPump/value", "/relayPumpReso/value", "/relayLight/value", "/relayPhUp/value", "/relayPhDown/value", "/relayNutrientA/value",
                       "/relayNutrientB/value", "/relayFan/value", "/relayWater/value"};

String childTimerPath[4] = {"/RelayOffTime/value", "/RelayOnTime/value", "/minute/value", "/second/value"};
String childSensorPath[6] = {"/humidity/value", "/lux/value", "/phLevel/value", "/tds/value", "/temperature/value",
                             "/waterTemperature/value"};

String controlConfig[4] = {"/relayControl/value", "/userDefine/value", "/preMix/value", "/preMix/nutriAdded"};

/**********************************************************************
 *
 * ***    Methods for Relay Initialization
 *
 **********************************************************************/
void Shutdown()
{
  digitalWrite(pinRelayFan, HIGH);
  digitalWrite(pinRelayPump, HIGH);
  digitalWrite(pinRelayLight, HIGH);
  digitalWrite(pinRelayPhUp, HIGH);
  digitalWrite(pinRelayPhDown, HIGH);
  digitalWrite(pinRelayNutrientA, HIGH);
  digitalWrite(pinRelayNutrientB, HIGH);
  digitalWrite(pinRelayPumpReso, HIGH);
  digitalWrite(pinRelayWater, HIGH);

  Serial.println("Shutdown Completed!");
}

void Firebase_RelayInit()
{
  pinMode(pinRelayFan, OUTPUT);
  pinMode(pinRelayPump, OUTPUT);
  pinMode(pinRelayLight, OUTPUT);
  pinMode(pinRelayPhUp, OUTPUT);
  pinMode(pinRelayPhDown, OUTPUT);
  pinMode(pinRelayNutrientA, OUTPUT);
  pinMode(pinRelayNutrientB, OUTPUT);
  pinMode(pinRelayPumpReso, OUTPUT);
  pinMode(pinRelayWater, OUTPUT);

  Shutdown();
  Serial.println("Done Relay Init");
}

// /**********************************************************************
//  *
//  * ***    Methods for Timer
//  *
//  **********************************************************************/

void FirebaseCheckWakeUp(float on, float off)
{
  float timeNow = getTimeRelay().toFloat();

  if (timeNow == on)
  {
    if (check == 0)
      Serial.println("andrew potsu");
    Wifi_Init();
    Firebase_Init();
    delay(56000);
    ESP.restart();
    check++;
  }
  else if (timeNow == off)
  {
    Shutdown();
    // Firebase.removeMultiPathStreamCallback(stream);
    // Firebase.removeMultiPathStreamCallback(streamSensor);
    // Firebase.removeMultiPathStreamCallback(streamTimer);
    delay(15000);
  }
  else if (on > off)
  {
    if (timeNow >= on && timeNow >= off)
    {
      // Serial.println("On parin1");
      shutdown = false;
    }
    else if (timeNow <= on && timeNow >= off)
    {
      // Serial.println("Off parin1");
      shutdown = true;
      check = 0;
    }
    else if (timeNow <= off && timeNow <= on)
    {
      // Serial.println("On parin2");
      shutdown = false;
    }
  }
  else
  {
    if (timeNow >= on && timeNow <= off)
    {
      // Serial.println("On parin3");
      shutdown = false;
    }

    else if (timeNow >= on && timeNow <= off)
    {
      // Serial.println("Off parin2");

      shutdown = true;
      check = 0;
    }

    else if (timeNow <= on && timeNow >= off)
    {
      // Serial.println("Off parin3");
      shutdown = true;
      check = 0;
    }
  }
}

void TimerSpray()
{
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis2 > minute)
  {
    previousMillis2 = currentMillis;
    digitalWrite(pinRelayPump, LOW);
    Serial.println("Spraying On..");
    isSpraying = true;
  }

  if (isSpraying)
  {
    if (currentMillis - previousMillis2 > second)
    {
      previousMillis2 = currentMillis;
      digitalWrite(pinRelayPump, HIGH);
      Serial.println("Spraying Off..");
      isSpraying = false;
    }
  }
}

void streamTimeoutCallback(bool timeout)
{
  if (timeout)
    Serial.println("stream timed out, resuming...\n");

  if (!stream.httpConnected())
    Serial_Printf("error code: %d, reason: %s\n\n", stream.httpCode(), stream.errorReason().c_str());
}


// /**********************************************************************
//  *
//  * ***    Methods for Firebase Callback For Changing of Values
//  *
//  **********************************************************************/

void printResult4(FirebaseData &data)
{
  Serial.printf("sream path, %s\nevent path, %s\ndata type, %s\nevent type, %s\n\n",
                data.streamPath().c_str(),
                data.dataPath().c_str(),
                data.dataType().c_str(),
                data.eventType().c_str());
  String pathLoc2 = data.dataPath().c_str();

  if (pathLoc2.equals("/relayPump"))
  {
    pumpValue = !pumpValue;
    pumpValue ? digitalWrite(pinRelayPump, LOW) : digitalWrite(pinRelayPump, HIGH);
    pumpValue ? Serial.println("Pump is ON") : Serial.println("Pump is OFF");
  }
  else if (pathLoc2.equals("/relayPumpReso"))
  {
    pumpResoValue = !pumpResoValue;
    pumpValue ? digitalWrite(pinRelayPumpReso, LOW) : digitalWrite(pinRelayPumpReso, HIGH);
    pumpValue ? Serial.println("PumpReso is ON") : Serial.println("PumpReso is OFF");
  }
  else if (pathLoc2.equals("/relayPhUp"))
  {
    phUpValue = !phUpValue;
    phUpValue ? digitalWrite(pinRelayPhUp, LOW) : digitalWrite(pinRelayPhUp, HIGH);
    phUpValue ? Serial.println("phUpValue is ON") : Serial.println("phUpValue is OFF");
  }
  else if (pathLoc2.equals("/relayPhDown"))
  {
    phDownValue = !phDownValue;
    phDownValue ? digitalWrite(pinRelayPhDown, LOW) : digitalWrite(pinRelayPhDown, HIGH);
    phDownValue ? Serial.println("phDownValue is ON") : Serial.println("phDownValue is OFF");
  }
  else if (pathLoc2.equals("/relayNutrientA"))
  {
    nutrientaValue = !nutrientaValue;
    nutrientaValue ? digitalWrite(pinRelayNutrientA, LOW) : digitalWrite(pinRelayNutrientA, HIGH);
    nutrientaValue ? Serial.println("nutrientaValue is ON") : Serial.println("nutrientaValue is OFF");
  }
  else if (pathLoc2.equals("/relayNutrientB"))
  {
    nutrientbValue = !nutrientbValue;
    nutrientbValue ? digitalWrite(pinRelayNutrientB, LOW) : digitalWrite(pinRelayNutrientB, HIGH);
    nutrientbValue ? Serial.println("nutrientbValue is ON") : Serial.println("nutrientbValue is OFF");
  }
  else if (pathLoc2.equals("/relayLight"))
  {
    lightValue = !lightValue;
    lightValue ? digitalWrite(pinRelayLight, LOW) : digitalWrite(pinRelayLight, HIGH);
    lightValue ? Serial.println("lightValue is ON") : Serial.println("lightValue is OFF");
  }
  else if (pathLoc2.equals("/relayWater"))
  {
    waterValue = !waterValue;
    waterValue ? digitalWrite(pinRelayWater, LOW) : digitalWrite(pinRelayWater, HIGH);
    waterValue ? Serial.println("waterValue is ON") : Serial.println("waterValue is OFF");
  }
  else if (pathLoc2.equals("/relayFan"))
  {
    fanValue = !fanValue;
    fanValue ? digitalWrite(pinRelayFan, LOW) : digitalWrite(pinRelayFan, HIGH);
    fanValue ? Serial.println("Fan is ON") : Serial.println("Fan is OFF");
  }

  Serial.println();
  // Serial.printf("Received stream payload size: %d (Max. %d)\n\n", data.payloadLength(), data.maxPayloadLength());
}

void printResultTimer(FirebaseData &data) // For Defined Value PathTimer
{
  Serial.printf("sream path, %s\nevent path, %s\ndata type, %s\nevent type, %s\n\n",
                data.streamPath().c_str(),
                data.dataPath().c_str(),
                data.dataType().c_str(),
                data.eventType().c_str());

  String pathLoc2 = data.dataPath().c_str();

  if (pathLoc2.equals("/RelayOffTime/value"))
  {
    temp = data.stringData().c_str();
    timerOffValue = temp.toFloat();
    Serial.print(timerOffValue);
    FirebaseCheckWakeUp(timerOnValue, timerOffValue);
  }
  else if (pathLoc2.equals("/RelayOnTime/value"))
  {
    temp = data.stringData().c_str();
    timerOnValue = temp.toFloat();
    Serial.print(timerOnValue);
    FirebaseCheckWakeUp(timerOnValue, timerOffValue);
  }
  else if (pathLoc2.equals("/minute/value"))
  {
    minute = (data.intData()) * 60 * 1000;
    Serial.println("Minutes path");
  }
  else if (pathLoc2.equals("/second/value"))
  {
    second = (data.intData()) * 1000;
    Serial.println("Seconds path");
  }

  Serial.println();
  Serial.printf("Received stream payload size: %d (Max. %d)\n\n", data.payloadLength(), data.maxPayloadLength());
}

void printConfig(FirebaseData &data) // For Configuration
{
  String pathLoc2 = data.dataPath().c_str();

  if (pathLoc2.equals("/preMix"))
  {
    preMixValue = !preMixValue;
    preMixValue ? Serial.print("preMix is ON") : Serial.print("preMix is OFF");
  }
  else if (pathLoc2.equals("/relayControl"))
  {
    relayConfigValue = !relayConfigValue;
    relayConfigValue ? Serial.print("Relay Control is ON") : Serial.print("Relay Control is OFF");
  }
  else if (pathLoc2.equals("/userDefine"))
  {
    defineValue = !defineValue;
    defineValue ? Serial.print("User Define is ON") : Serial.print("User Define is OFF");
  }
  else if(pathLoc2.equals("/nutriAdded"))
  {
    nutriAddedValue = !nutriAddedValue;
    defineValue ? Serial.print("User Define is ON") : Serial.print("User Define is OFF");
  }

  Serial.println();
  Serial.printf("Received stream payload size4: %d (Max. %d)\n\n", data.payloadLength(), data.maxPayloadLength());
}

void printResultSensor(FirebaseData &data)
{
  // Serial.printf("sream path, %s\nevent path, %s\ndata type, %s\nevent type, %s\n\n",
  //             data.streamPath().c_str(),
  //             data.dataPath().c_str(),
  //             data.dataType().c_str(),
  //             data.eventType().c_str());

  String pathLoc3 = data.dataPath().c_str();

  // if (pathLoc3.equals("/humidity/value"))
  // {
  //   humidityValue = data.floatData();
  // }
  // else if (pathLoc3.equals("/lux/value"))
  // {
  //   Serial.println("lux path");
  // }
 if (pathLoc3.equals("/phLevel/value"))
  {
    Serial.println("phLvel path");

    if (phLevelValue >= phLevelMinVal && phLevelValue <= phLevelMaxVal)
    {
      Premix = "OFF";
      Firebase.setString(streamSensor, "/controlConfig/nutriAdded/value", "ON");
      Serial.println("Done");
    }
    else if (phLevelValue < phLevelMinVal)
    {
      digitalWrite(pinRelayPhUp, LOW);
      delay(700);
      digitalWrite(pinRelayPhUp, HIGH);
    }
    else if (phLevelValue > phLevelMaxVal)
    {
      digitalWrite(pinRelayPhDown, LOW);
      delay(700);
      digitalWrite(pinRelayPhDown, HIGH);
    }
  }
  // else if (pathLoc3.equals("/ppm/value"))
  // {
  //   Serial.println("ppm path");
  // }
  // else if (pathLoc3.equals("/temperature/value"))
  // {
  //   Serial.println("temperature path");
  // }
  // else if (pathLoc3.equals("/waterTemperature/value"))
  // {
  //   Serial.println("waterTemperature path");
  // }

  // //   Value Max Checker
  // else if (pathLoc3.equals("/humidity/valueMax"))
  // {
  //   Serial.println("humidity max path");
  // }
  // else if (pathLoc3.equals("/lux/valueMax"))
  // {
  //   Serial.println("lux max path");
  // }
  // else if (pathLoc3.equals("/phLevel/valueMax"))
  // {
  //   Serial.println("phLevel max path");
  // }
  // else if (pathLoc3.equals("/ppm/valueMax"))
  // {
  //   Serial.println("ppm max path");
  // }
  // else if (pathLoc3.equals("/temperature/valueMax"))
  // {
  //   Serial.println("temperature max path");
  // }
  // else if (pathLoc3.equals("/waterTemperature/valueMax"))
  // {
  //   Serial.println("water temperature max path");
  // }

  // //    Value Min Checker
  // else if (pathLoc3.equals("/humidity/valueMin"))
  // {
  //   Serial.println("humidity min path");
  // }
  // else if (pathLoc3.equals("/lux/valueMin"))
  // {
  //   Serial.println("lux min path");
  // }
  // else if (pathLoc3.equals("/phLevel/valueMin"))
  // {
  //   Serial.println("phLevel min path");
  // }
  // else if (pathLoc3.equals("/ppm/valueMin"))
  // {
  //   Serial.println("ppm min path");
  // }
  // else if (pathLoc3.equals("/temperature/valueMin"))
  // {
  //   Serial.println("temperature min path");
  // }
  // else if (pathLoc3.equals("/waterTemperature/valueMin"))
  // {
  //   Serial.println("water temperature min path");
  // }

  Serial.println();
  Serial.printf("Received stream payload size: %d (Max. %d)\n\n", data.payloadLength(), data.maxPayloadLength());
}

// /**********************************************************************
//  *
//  * ***    Methods for Firebase Callbacks
//  *
//  **********************************************************************/

void streamConfigCallback(MultiPathStreamData streamVal4)
{
  if (ConfigCount == 0)
  {
    size_t numChild4 = sizeof(controlConfig) / sizeof(controlConfig[0]);
    Serial.println();
    for (size_t i = 0; i < numChild4; i++)
    {
      if (streamVal4.get(controlConfig[i]))
      {
        if (controlConfig[i].equalsIgnoreCase("/relayControl/value"))
        {
          streamVal4.value.equalsIgnoreCase("ON") ? Serial.println("ON RelayControl") : Serial.println("OFF RelayControl");
          streamVal4.value.equalsIgnoreCase("ON") ? relayConfigValue = true : relayConfigValue = false;
        }
        else if (controlConfig[i].equalsIgnoreCase("/userDefine/value"))
        {
          streamVal4.value.equalsIgnoreCase("ON") ? Serial.println("ON defineValue") : Serial.println("OFF defineValue");
          streamVal4.value.equalsIgnoreCase("ON") ? defineValue = true : defineValue = false;
        }
        else if (controlConfig[i].equalsIgnoreCase("/preMix/value"))
        {
          streamVal4.value.equalsIgnoreCase("ON") ? Serial.println("ON Premix") : Serial.println("OFF Premix");
          streamVal4.value.equalsIgnoreCase("ON") ? Premix = "ON" : Premix = "OFF";
        }
        else if (controlConfig[i].equalsIgnoreCase("/nutriAdded/value"))
        {
          streamVal4.value.equalsIgnoreCase("ON") ? Serial.println("ON nutriAdded") : Serial.println("OFF nutriAdded");
          streamVal4.value.equalsIgnoreCase("ON") ? nutriAddedValue = true : nutriAddedValue = false;
        }
      }
    }
    Serial.printf("Received stream payload size4: %d (Max. %d)\n\n", stream.payloadLength(), stream.maxPayloadLength());
    dataChanged = true;
    ConfigCount++;
  }
  else
  {
    printConfig(streamConfig);
    dataChanged = true;
  }
}

void streamCallbackSensor(MultiPathStreamData streamVal3)
{
  if (countSensor == 0)
  {
    size_t numChild3 = sizeof(childSensorPath) / sizeof(childSensorPath[0]);
    Serial.println();
    for (size_t i = 0; i < numChild3; i++)
    {
      if (streamVal3.get(childSensorPath[i]))
      {
        if (streamVal3.dataPath.equals(childSensorPath[i]))
        {
          if (streamVal3.dataPath.equals("/humidity/value"))
          {
            humidityValue = streamVal3.value.toFloat();
            Serial.println("humidity");
          }
          else if (streamVal3.dataPath.equals("/lux/value"))
          {
            luxValue = streamVal3.value.toFloat();
            Serial.println("lux");
          }
          else if (streamVal3.dataPath.equals("/phLevel/value"))
          {
            phLevelValue = streamVal3.value.toFloat();
            Serial.println("phLevel");
          }
          else if (streamVal3.dataPath.equals("/ppm/value"))
          {
            tdsValue = streamVal3.value.toFloat();
            Serial.println("ppm");
          }
          else if (streamVal3.dataPath.equals("/temperature/value"))
          {
            tempValue = streamVal3.value.toFloat();
            Serial.println("temperature");
          }
          else if (streamVal3.dataPath.equals("/waterTemperature/value"))
          {
            waterTempValue = streamVal3.value.toFloat();
            Serial.println("water temperature");
          }

          //   Value Max Checker
          else if (streamVal3.dataPath.equals("/humidity/valueMax"))
          {
            humMaxVal = streamVal3.value.toFloat();
            Serial.println("humidity max");
          }
          else if (streamVal3.dataPath.equals("/lux/valueMax"))
          {
            luxMaxVal = streamVal3.value.toFloat();
            Serial.println("lux max");
          }
          else if (streamVal3.dataPath.equals("/phLevel/valueMax"))
          {
            phLevelMaxVal = streamVal3.value.toFloat();
            Serial.println("phLevel max");
          }
          else if (streamVal3.dataPath.equals("/ppm/valueMax"))
          {
            ppmMaxVal = streamVal3.value.toFloat();
            Serial.println("ppm max");
          }
          else if (streamVal3.dataPath.equals("/temperature/valueMax"))
          {
            tempMaxVal = streamVal3.value.toFloat();
            Serial.println("temperature max");
          }
          else if (streamVal3.dataPath.equals("/waterTemperature/valueMax"))
          {
            waterMaxVal = streamVal3.value.toFloat();
            Serial.println("water temperature max");
          }

          //    Value Min Checker
          else if (streamVal3.dataPath.equals("/humidity/valueMin"))
          {
            humMinVal = streamVal3.value.toFloat();
            Serial.println("humidity min");
          }
          else if (streamVal3.dataPath.equals("/lux/valueMin"))
          {
            luxMinVal = streamVal3.value.toFloat();
            Serial.println("lux min");
          }
          else if (streamVal3.dataPath.equals("/phLevel/valueMin"))
          {
            phLevelMinVal = streamVal3.value.toFloat();
            Serial.println("phLevel min");
          }
          else if (streamVal3.dataPath.equals("/ppm/valueMin"))
          {
            ppmMinVal = streamVal3.value.toFloat();
            Serial.println("ppm min");
          }
          else if (streamVal3.dataPath.equals("/temperature/valueMin"))
          {
            tempMinVal = streamVal3.value.toFloat();
            Serial.println("temperature min");
          }
          else if (streamVal3.dataPath.equals("/waterTemperature/valueMin"))
          {
            waterMinVal = streamVal3.value.toFloat();
            Serial.println("water temperature min");
          }
          //  Serial.printf("path: %s, event: %s, type: %s, value: %s%s", streamVal3.dataPath.c_str(), streamVal3.eventType.c_str(), streamVal3.type.c_str(), streamVal3.value.c_str(), i < numChild3 - 1 ? "\n" : "");
          //  Serial.printf("\nInside if(stream.get(childPath[i])) with index: %d\n\n", i);
        }
      }
    }
    Serial.printf("Received stream payload size1: %d (Max. %d)\n\n", stream.payloadLength(), stream.maxPayloadLength());
    // dataChanged = true;
    countSensor++;
  }
  else
  {
    printResultSensor(streamSensor);
    dataChanged = true;
  }
}

void streamCallbackTimer(MultiPathStreamData streamVal2)
{
  if (countTimer == 0)
  {
    size_t numChildTimer = sizeof(childTimerPath) / sizeof(childTimerPath[0]);
    Serial.println();
    for (size_t i = 0; i <= numChildTimer; i++)
    {
      if (streamVal2.get(childTimerPath[i]))
      {
        if (streamVal2.dataPath.equals(childTimerPath[i]))
        {
          if (childTimerPath[i].equalsIgnoreCase("/RelayOffTime/value"))
          {
            temp = streamVal2.value.c_str();
            timerOffValue = temp.toFloat();
            Serial.print(timerOffValue);
            Serial.println(" is set as Sleep time");
          }
          else if (childTimerPath[i].equalsIgnoreCase("/RelayOnTime/value"))
          {
            temp = streamVal2.value.c_str();
            timerOnValue = temp.toFloat();
            Serial.print(timerOnValue);
            Serial.println(" is set as Wake up time");
          }
          else if (childTimerPath[i].equalsIgnoreCase("/minute/value"))
          {
            temp = streamVal2.value.c_str();
            minute = temp.toInt() * 60 * 1000;
            Serial.print(minute);
            Serial.println(" is set to minutes");
          }
          else if (childTimerPath[i].equalsIgnoreCase("/second/value"))
          {
            temp = streamVal2.value.c_str();
            second = temp.toInt() * 1000;
            Serial.print(second);
            Serial.println(" is set to seconds");
          }
        }
      }
    }
    Serial.printf("Received stream payload size1: %d (Max. %d)\n\n", streamTimer.payloadLength(), streamTimer.maxPayloadLength());
    dataChanged = true;
    countTimer++;
  }
  else
  {
    printResultTimer(streamTimer);
    dataChanged = true;
    countTimer++;
  }
}

void streamCallback(MultiPathStreamData streamVal)
{
  if (count == 0)
  {
    size_t numChild = sizeof(childPath) / sizeof(childPath[0]);
    Serial.println();
    for (size_t i = 0; i < numChild; i++)
    {
      if (streamVal.get(childPath[i]))
      {
        if (childPath[i].equalsIgnoreCase("/relayLight/value"))
        {
          streamVal.value.equalsIgnoreCase("ON") ? digitalWrite(pinRelayLight, LOW) : digitalWrite(pinRelayLight, HIGH);
          streamVal.value.equalsIgnoreCase("ON") ? Serial.println("ON Light") : Serial.println("OFF Light");
          streamVal.value.equalsIgnoreCase("ON") ? lightValue = true : lightValue = false;
        }
        if (childPath[i] == "/relayPump/value")
        {
          streamVal.value.equalsIgnoreCase("ON") ? digitalWrite(pinRelayPump, LOW) : digitalWrite(pinRelayPump, HIGH);
          streamVal.value.equalsIgnoreCase("ON") ? Serial.println("ON Pump") : Serial.println("OFF Pump");
          streamVal.value.equalsIgnoreCase("ON") ? pumpValue = true : pumpValue = false;
        }
        if (childPath[i] == "/relayPumpReso/value")
        {
          streamVal.value.equalsIgnoreCase("ON") ? digitalWrite(pinRelayPumpReso, LOW) : digitalWrite(pinRelayPumpReso, HIGH);
          streamVal.value.equalsIgnoreCase("ON") ? Serial.println("ON Pump Reso") : Serial.println("OFF Pump Reso");
          streamVal.value.equalsIgnoreCase("ON") ? pumpResoValue = true : pumpResoValue = false;
        }
        if (childPath[i] == "relayPhDown/value")
        {
          streamVal.value.equalsIgnoreCase("ON") ? digitalWrite(pinRelayPhDown, LOW) : digitalWrite(pinRelayPhDown, HIGH);
          streamVal.value.equalsIgnoreCase("ON") ? Serial.println("ON phDown") : Serial.println("OFF phDown");
          streamVal.value.equalsIgnoreCase("ON") ? phDownValue = true : phDownValue = false;
        }
        if (childPath[i] == "/relayPhUp/value")
        {
          streamVal.value.equalsIgnoreCase("ON") ? digitalWrite(pinRelayPhUp, LOW) : digitalWrite(pinRelayPhUp, HIGH);
          streamVal.value.equalsIgnoreCase("ON") ? Serial.println("ON ph Up") : Serial.println("OFF ph Up");
          streamVal.value.equalsIgnoreCase("ON") ? phUpValue = true : phUpValue = false;
        }
        if (childPath[i] == "/relayNutrientA/value")
        {
          streamVal.value.equalsIgnoreCase("ON") ? digitalWrite(pinRelayNutrientA, LOW) : digitalWrite(pinRelayNutrientA, HIGH);
          streamVal.value.equalsIgnoreCase("ON") ? Serial.println("ON Nutrient A") : Serial.println("OFF Nutrient A");
          streamVal.value.equalsIgnoreCase("ON") ? nutrientaValue = true : nutrientaValue = false;
        }
        if (childPath[i] == "/relayNutrientB/value")
        {
          streamVal.value.equalsIgnoreCase("ON") ? digitalWrite(pinRelayNutrientB, LOW) : digitalWrite(pinRelayNutrientB, HIGH);
          streamVal.value.equalsIgnoreCase("ON") ? Serial.println("ON Nutrient B") : Serial.println("OFF Nutrient B");
          streamVal.value.equalsIgnoreCase("ON") ? nutrientbValue = true : nutrientbValue = false;
        }
        if (childPath[i] == "/relayWater/value")
        {
          streamVal.value.equalsIgnoreCase("ON") ? digitalWrite(pinRelayWater, LOW) : digitalWrite(pinRelayWater, HIGH);
          streamVal.value.equalsIgnoreCase("ON") ? Serial.println("ON Water Relay") : Serial.println("OFF Water Relay");
          streamVal.value.equalsIgnoreCase("ON") ? waterValue = true : waterValue = false;
        }
        if (childPath[i].equalsIgnoreCase("/relayFan/value"))
        {
          streamVal.value.equalsIgnoreCase("ON") ? digitalWrite(pinRelayFan, LOW) : digitalWrite(pinRelayFan, HIGH);
          fanValue ? Serial.println(true) : Serial.println("false");
          streamVal.value.equalsIgnoreCase("ON") ? fanValue = true : fanValue = false;
          streamVal.value.equalsIgnoreCase("ON") ? Serial.println("ON Fan") : Serial.println("OFF Fan");
        }
        // Serial.printf("path: %s, event: %s, type: %s, value: %s%s", streamVal.dataPath.c_str(), streamVal.eventType.c_str(), streamVal.type.c_str(), streamVal.value.c_str(), i < numChild - 1 ? "\n" : "");
        // Serial.printf("Inside if(stream.get(childPath[i])) with index: %d\n\n", i);
      }
    }
    Serial.printf("Received stream payload size1: %d (Max. %d)\n\n", stream.payloadLength(), stream.maxPayloadLength());
    // dataChanged = true;
    count++;
  }
  else
  {
    printResult4(stream);
    dataChanged = true;
    count++;
  }
}

// /**********************************************************************
//  *
//  * ***    Methods for Firebase Upload Sensor
//  *
//  **********************************************************************/

void uploadTemp()
{
  tempTemp = tempValue;
  tempValue = TemperatureValue();
  if (tempValue != tempTemp && tempValue > 1)
    Firebase.RTDB.setFloat(&fbdo, "sensor/temperature/value", tempValue);
  delay(1000);
}

void uploadHum()
{
  tempHum = humidityValue;
  humidityValue = HumidityValue();
  if (humidityValue != tempHum && humidityValue > 1)
    tempHum == humidityValue ? humidityValue : Firebase.RTDB.setFloat(&fbdo, "sensor/humidity/value", humidityValue);
  delay(1000);
}

void uploadLux()
{
  tempLux = luxValue;
  luxValue = LuxValue();
  luxValue == tempLux ? luxValue : Firebase.RTDB.setFloat(&fbdo, "sensor/lux/value", luxValue);
  delay(1000);
}

void uploadPpm()
{
  tempTds = tdsValue;
  tdsValue = TdsValue2();
  tdsValue == tempTds ? tempTds : Firebase.RTDB.setFloat(&fbdo, "sensor/ppm/value", tdsValue);
  delay(1000);
}

void uploadPh()
{
  tempPh = phLevelValue;
  phLevelValue = PhValue();
  phLevelValue == tempPh ? tempPh : Firebase.RTDB.setFloat(&fbdo, "sensor/phLevel/value", phLevelValue);
  delay(1000);
}

void uploadWaterTemp()
{
  tempWaterTemp = waterTempValue;
  waterTempValue = waterTemp();
  Serial.println(waterTemp());
  waterTempValue == tempWaterTemp ? waterTempValue : Firebase.RTDB.setFloat(&fbdo, "sensor/waterTemperature/value", waterTempValue);
  delay(1000);
}

void uploadWaterLevel()
{
  tempWaterLevel = waterLevelVal;
  waterLevelVal = WaterLevelValue();
  waterLevelVal == tempWaterLevel ? waterLevelVal : Firebase.RTDB.setString(&fbdo, "sensor/waterLevel/value", waterLevelVal);
  delay(1000);
}

// void temperatureRelayComputation(){

//   tempTemp = TemperatureValue();
//   tempHum = HumidityValue();

//   if (tempTemp != tempValue)
//   {
//     Firebase.setFloatAsync(fbdo, "sensor/temperature/value", tempTemp);
//     delay(500);
//     if (tempTemp > 31)
//     {
//       if(!fanValue){
//         Firebase.setString(fbdo, "relay/relayFan/value", "ON");
//       }
//     }
//   }
// }

// void Computation(){

//   tempTemp = TemperatureValue();
//   tempHum = HumidityValue();

//   if (tempTemp != tempValue)
//   {
//     Firebase.setFloatAsync(fbdo, "sensor/temperature/value", tempTemp);
//     delay(500);
//     if (tempTemp > 31)
//     {
//       if(!fanValue){
//         Firebase.setString(fbdo, "relay/relayFan/value", "ON");
//       }
//     }
//   }
// }


// /**********************************************************************
//  *
//  * ***    Methods for Calling Sensor Readings
//  *
//  **********************************************************************/

void sensorReading()
{
  delay(5000);
  uploadTemp();
  uploadHum();
  uploadLux();
  uploadPh();
  uploadPpm();
  uploadWaterTemp();
  uploadWaterLevel();
}

// /**********************************************************************
//  *
//  * ***    Methods for Firebase INIT and LOOP Function
//  *
//  **********************************************************************/

void Firebase_Init()
{

  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

  /* Assign the api key (required) */
  config.api_key = API_KEY;

  /* Assign the user sign in credentials */
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; // see addons/TokenHelper.h

  // Or use legacy authenticate method
  // config.database_url = DATABASE_URL;
  // config.signer.tokens.legacy_token = "<database secret>";

  // To connect without auth in Test Mode, see Authentications/TestMode/TestMode.ino
  //  Firebase.setReadTimeout(fbdo, 90000);
  Firebase.begin(&config, &auth);
  Firebase.allowMultipleRequests(true);
  Firebase.setwriteSizeLimit(fbdo, "large");

  Firebase.reconnectWiFi(true);

  // if (!Firebase.beginMultiPathStream(stream, pathConfig))
  //   Serial.printf("sream begin error, %s\n\n", stream.errorReason().c_str());

  // if (!Firebase.beginMultiPathStream(stream, pathRelay))
    // Serial.printf("stream begin error, %s\n\n", stream.errorReason().c_str());

  // if (!Firebase.beginMultiPathStream(streamTimer, pathTimer))
  //   Serial.printf("stream begin error, %s\n\n", streamTimer.errorReason().c_str());

  // if (!Firebase.beginMultiPathStream(streamSensor, pathSensor))
  //   Serial.printf("stream begin error, %s\n\n", streamSensor.errorReason().c_str());

  // // if (!Firebase.beginMultiPathStream(stream3, pathSensor))
  // // Serial.printf("sream begin error, %s\n\n", stream3.errorReason().c_str());

  if (!Firebase.beginMultiPathStream(streamConfig, pathConfig))
    Serial.printf("stream begin error, %s\n\n", streamSensor.errorReason().c_str());

  Firebase.setMultiPathStreamCallback(streamConfig, streamConfigCallback, streamTimeoutCallback);

  // Firebase.setMultiPathStreamCallback(stream, streamConfigCallback, streamTimeoutCallback);
  // Firebase.setMultiPathStreamCallback(streamTimer, streamCallbackTimer, streamTimeoutCallback);
  // Firebase.setMultiPathStreamCallback(streamSensor, streamCallbackSensor, streamTimeoutCallback);

  // Firebase_RelayInit();

  /** Timeout options, below is default config.

  //WiFi reconnect timeout (interval) in ms (10 sec - 5 min) when WiFi disconnected.
  config.timeout.wifiReconnect = 10 * 1000;

  //Socket begin connection timeout (ESP32) or data transfer timeout (ESP8266) in ms (1 sec - 1 min).
  config.timeout.socketConnection = 30 * 1000;

  //ESP32 SSL handshake in ms (1 sec - 2 min). This option doesn't allow in ESP8266 core library.
  config.timeout.sslHandshake = 2 * 60 * 1000;

  //Server response read timeout in ms (1 sec - 1 min).
  config.timeout.serverResponse = 10 * 1000;

  //RTDB Stream keep-alive timeout in ms (20 sec - 2 min) when no server's keep-alive event data received.
  config.timeout.rtdbKeepAlive = 45 * 1000;

  //RTDB Stream reconnect timeout (interval) in ms (1 sec - 1 min) when RTDB Stream closed and want to resume.
  config.timeout.rtdbStreamReconnect = 1 * 1000;

  //RTDB Stream error notification timeout (interval) in ms (3 sec - 30 sec). It determines how often the readStream
  //will return false (error) when it called repeatedly in loop. */
  config.timeout.rtdbStreamError = 3 * 1000;
}

void firebaseLoop()
{

  unsigned long currentMillis = millis();
  // if WiFi is down, try reconnecting every CHECK_WIFI_TIME seconds
 if ((currentMillis - previousMillis >= restartTimer))
  {
    Serial.print(millis());
    Serial.println("Restarting.");
    ESP.restart();  
    previousMillis = currentMillis;
  }

  if(Premix == "ON"){

    if(premixCount == 0){
      Serial.println("Pasok Dito Premix");
      for (int i = 0; i < 2; i++)
      {
        delay(5000);
      }
      premixCount++;
    }
    else{
      if(PhValue() >=5.5 && PhValue() <=6){
        Firebase.setString(fbdo, "/controlConfig/nutriAdded/value", "ON");
        Premix == "OFF";
      }
    }
  }
  else{
    delay(300);
    // Firebase.readStream(streamConfig);
    sensorReading();
    if (dataChanged)
    {
      Serial.println("change happen");
      dataChanged = false;
    }
    premixCount == 0;
  }
}
