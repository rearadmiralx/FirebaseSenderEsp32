
// #include <Arduino.h>
// #include "DFRobot_ESP_PH_WITH_ADC.h"
// #include "EEPROM.h"

// #define SERIAL Serial
// #define sensorPin 25

// DFRobot_ESP_PH_WITH_ADC ph;
// #define ESPADC 4096.0   //the esp Analog Digital Convertion value
// #define ESPVOLTAGE 3000 //the esp voltage supply value
// #define PH_PIN 34    //the esp gpio data pin number
// float voltage, phValue, temperature = 25;
// int sensorValue = 0;
// float tdsValue3 = 0;
// float Voltage2 = 0;
// void Ph_Init()
// {
//   EEPROM.begin(32);//needed to permit storage of calibration value in eeprom
//   ph.begin();
// }

// void Ph_Loop()
// {
//   static unsigned long timepoint = millis();
//   if (millis() - timepoint > 1000U) //time interval: 1s
//   {
//     timepoint = millis();
//     //voltage = rawPinValue / esp32ADC * esp32Vin
//     voltage = analogRead(PH_PIN) / ESPADC * ESPVOLTAGE; // read the voltage
//     Serial.print("voltage:");
//     Serial.println(voltage, 4);
    
//     //temperature = readTemperature();  // read your temperature sensor to execute temperature compensation
//     Serial.print("temperature:");
//     Serial.print(temperature, 1);
//     Serial.println("^C");

//     phValue = ph.readPH(voltage, temperature); // convert voltage to pH with temperature compensation
//     Serial.print("pH:");
//     Serial.println(phValue, 4);
//   }
//   ph.calibration(voltage, temperature); // calibration process by Serail CMD
// }

// float readTemperature()
// {
//   //add your code here to get the temperature from your temperature sensor
// }

// float PhValue(){
//       static unsigned long timepoint = millis();
//   if (millis() - timepoint > 1000U) //time interval: 1s
//   {
//     timepoint = millis();
//     //voltage = rawPinValue / esp32ADC * esp32Vin
//     voltage = analogRead(PH_PIN) / ESPADC * ESPVOLTAGE; // read the voltage
//     Serial.print("voltage:");
//     Serial.println(voltage);
    
//     //temperature = readTemperature();  // read your temperature sensor to execute temperature compensation
//     Serial.print("temperature:");
//     Serial.print(temperature, 1);
//     Serial.println("^C");

//     phValue = ph.readPH(voltage, temperature); // convert voltage to pH with temperature compensation
//     Serial.print("pH:");
//     Serial.println(phValue);
//   }
//   ph.calibration(voltage, temperature); // calibration process by Serail CMD
//   return (phValue);
// }


// float TdsValue2(){
//     sensorValue = analogRead(sensorPin);
//     Voltage2 = sensorValue*5/1024.0; //Convert analog reading to Voltage
//     tdsValue3=(133.42*Voltage2*Voltage2*Voltage2 - 255.86*Voltage2*Voltage2 + 857.39*Voltage2)*0.5; //Convert voltage value to TDS value
//     SERIAL.print("TDS Value = "); 
//     SERIAL.print(tdsValue3);
//     SERIAL.println(" ppm");
//     return tdsValue3;
// }




// // #include <Arduino.h>
// // #include <FirebaseESP32.h>  // Firebase Library server
// // #include "addons/TokenHelper.h" //Provide the token generation process info.
// // #include "addons/RTDBHelper.h" //Provide the RTDB payload printing info and other helper functions.
// // #include "header.h"



// // unsigned long sendDataPrevMillis1;


// // #define API_KEY "AIzaSyD-0Z0xs0y_3da04nFrfjFnw1Tys7MAnpc"
// // #define DATABASE_URL "https://esp32-aeroponics-default-rtdb.asia-southeast1.firebasedatabase.app" //<databaseName>.firebaseio.com or <databaseName>.<region>.firebasedatabase.app
// // #define FIREBASE_HOST "esp32-aeroponics.firebaseio.com"
// // FirebaseData fbdo; // Firebase Realtime Database Object
// // FirebaseData fbFan, fbPump, fbPhUp, fbPhDown, fbNutA, fbNutB, fbLight; // Firebase Realtime Database Object
// // FirebaseData fbHumidity, fbLux, fbPhLevel, fbTds, fbTemperature, fbWaterTemp;
// // FirebaseAuth auth;     // Firebase Authentication Object
// // FirebaseConfig config; // Firebase configuration Object
// // String fuid = ""; // Firebase Unique Identifier

// // #define USER_EMAIL "frankgavina@yahoo.com"
// // #define USER_PASSWORD "Password@123"
// // bool isAuthenticated = false;
// // bool wakeup = true;

// // FirebaseJson relayPump_json, relayFan_json, relayNutrientB_json, relayNutrientA_json, // Firebase json
// //              relayLight_json, relayphUp_json, relayPhDown_json;

// // FirebaseJson sensorHumidity_json, sensorLux_json, sensorPhLevel_json, sensorTds_json,
// //              sensorTemp_json, sensorWaterTemp_json;

// // //Pin Declaration
// // int pinRelayPump = 16;
// // int pinRelayLight = 25;
// // int pinRelayPhUp = 27;
// // int pinRelayPhDown = 14;
// // int pinRelayNutrientA = 26;
// // int pinRelayNutrientB = 17;
// // int pinRelayFan = 33;
// // long previousMillis = 0;

// // boolean isSpraying = false;
// // boolean shutdown = false;

// // // location path
// // String pathRelayPump = "relays/relayPump/value";
// // String pathRelayLight = "relays/relayLight/value";
// // String pathRelayPhUp = "relays/relayPhUp/value";
// // String pathRelayPhDown = "relays/relayPhDown/value";
// // String pathRelayNutrientA= "relays/relayNutrientA/value";
// // String pathRelayNutrientB = "relays/relayNutrientB/value";  
// // String pathRelayFan = "relays/relayFan/value";
// // String pathTimer = "definedValue/relay/";
// // String pathSensor = "definedValue/sensor/";

// // //  Global variables
// // unsigned long elapsedMillis2 = 0; // Stores the elapsed time from device start ups
// // unsigned long update_interval2 = 10000; //
// // String pumpValue, lightValue, nutrientaValue, nutrientbValue,    // ON OFF lang kasi to
// //        phUpValue, phDownValue, fanValue;

// // float humidityValue, luxValue, phLevelValue, tdsValue, temperatureValue, waterTemperatureValue;
// // float timeOnValue, timeOffValue;

// // // class SessionCommentsChildListener : public firebase::database::ChildListener {
// // //     public:
// // //     void OnChildAdded(const firebase::database::DataSnapshot& snapshot,
// // //                 const char* previous_sibling) override {
// // //     // Do something with the data in snapshot ...
// // //     }
// // //     void OnChildChanged(const firebase::database::DataSnapshot& snapshot,
// // //                 const char* previous_sibling) override {
// // //     // Do something with the data in snapshot ...
// // //     }
// // //     void OnChildRemoved(
// // //     const firebase::database::DataSnapshot& snapshot) override {
// // //     // Do something with the data in snapshot ...
// // //     }
// // //     void OnChildMoved(const firebase::database::DataSnapshot& snapshot,
// // //                 const char* previous_sibling) override {
// // //     // Do something with the data in snapshot ...
// // //     }
// // //     void OnCancelled(const firebase::database::Error& error_code,
// // //                 const char* error_message) override {
// // //     LogMessage("ERROR: SessionCommentsChildListener canceled: %d: %s",
// // //             error_code, error_message);
// // //     }
// // // };

// // /**********************************************************************
// //  * 
// //  * ***    Methods for Relay Initialization
// //  * 
// //  **********************************************************************/ 
// // void Firebase_RelayInit(){
// //   pinMode(pinRelayFan , OUTPUT);
// //   pinMode(pinRelayPump , OUTPUT);
// //   pinMode(pinRelayLight , OUTPUT);
// //   pinMode(pinRelayPhUp , OUTPUT);
// //   pinMode(pinRelayPhDown , OUTPUT);
// //   pinMode(pinRelayNutrientA , OUTPUT);
// //   pinMode(pinRelayNutrientB, OUTPUT);

// //   Serial.println("\nDone Relay Init");
// // }

// // // long getMinutes(){
// // //     Firebase.getString(fbdo, pathTimer +"min");
// // //     long minute = fbdo.intData() * 60000;
// // //     Serial.println(pathTimer +"status is :" + minute + "\n");
// // //     return minute;
// // // }

// // // long getSeconds(){
// // //     Firebase.getString(fbdo, pathTimer +"sec");
// // //     long second = fbdo.intData() * 1000;
// // //     Serial.println(pathTimer +"status is :" + second + "\n");
// // //     return second;
// // // }


// // // void Relay_getStatus(String path, int pin){
// // //   if (millis() - elapsedMillis2 > update_interval2 && isAuthenticated && Firebase.ready())
// // //   {
// // //     elapsedMillis2 = millis();
// // //     Firebase.getString(fbdo, path);
// // //     String value = fbdo.stringData();
// // //     Serial.println(path +"status is :" + value + "\n");
// // //     if(path == "relays/relayPump" ){
// // //         value == "ON" ? isSpraying = true : isSpraying = false;
// // //     }
// // //     value == "ON" ? digitalWrite(pin, HIGH)  : digitalWrite(pin, LOW);
// // //   }
// // // }

// // // void Relay_setStatus(String path, int pin){
// // //     elapsedMillis2 = millis();
// // //     Relay_getStatus(pathRelayPump+"/value", pinRelayPump);
// // //     Firebase.setString(fbdo, path, "OFF");
// // //     digitalWrite(pin, LOW);
// // // }

// // // void Relay_Shutdown(){
// // //     shutdown = true;
// // //     Relay_setStatus(pathRelayLight + "/value", pinRelayPump);
// // //     Relay_setStatus(pathRelayPump + "/value", pinRelayPump);

// // // }

// // // void RelayRollCallback(){
// // //   Relay_getStatus(pathRelayPump+"/value", pinRelayPump);
// // //   Relay_getStatus(pathRelayLight+"/value", pinRelayLight);
// // // }

// // // /**********************************************************************
// // //  * 
// // //  * ***    Methods for Setting up timer
// // //  * 
// // //  **********************************************************************/ 
// // // float FirebaseGetTime(String pathTime){
// // //   Firebase.getString(fbdo, pathTime);
// // //   String timer = fbdo.stringData().c_str();
// // //   timer.replace(":",".");
// // //   float timeOnFloat = timer.toFloat();

// // //   return timeOnFloat;
// // // }

// // // /*
// // // *           GET TIME START AND TIME OFF TO CHECK THE PROTOTYPE STATUS    
// // // *                       TO TURN OFF OR TURN ON
// // // */
// // // void FirebaseCheckWakeUp(float timeOntoFloat , float timeOfftoFloat ){
  
// // //   float timeNow = getTimeRelay().toFloat();
// // //   Serial.println(timeOntoFloat);
// // //   Serial.println(timeOfftoFloat);
// // //   Serial.println(timeNow);

// // //   if (timeOntoFloat > timeOfftoFloat)
// // //   { // off 16  , on 17 , now 15
// // //     if(timeNow >= timeOntoFloat){
// // //         Serial.println("On Na1");
// // //         RelayRollCallback(); 
// // //     }
// // //     else{
// // //         Serial.println("Off Na1");
// // //         Relay_Shutdown();
// // //     }
// // //   }
// // //   else{ // off 17  , on 16 , now 16:36
// // //     if(timeNow >= timeOfftoFloat){
// // //       Serial.println("Off Na2");
// // //       Relay_Shutdown();
      
// // //     }
// // //     else{
// // //       Serial.println("On Parin2");
// // //       RelayRollCallback();
// // //     }
// // //   }
// // // }

// // // /**********************************************************************
// // //  * 
// // //  * ***    Methods for Relay Initialization
// // //  * 
// // //  **********************************************************************/ 
// // // void Firebase_CallLoop(){
// // //   RelayRollCallback();
// // // }

// // // long sprayOnInterval = getMinutes();
// // // long sprayOffInterval = getSeconds();

// // // void Firebase_TimerLoop(){
// // //   delay(500);
// // //   if(shutdown == isSpraying){
// // //     float timeOn = FirebaseGetTime(pathTimer + "RelayTimerOn");
// // //     float timeOff = FirebaseGetTime(pathTimer + "RelayTimerOff");
// // //     FirebaseCheckWakeUp(timeOn, timeOff);
// // //   }
// // //   else{
// // //       Relay_Shutdown();
// // //   }
// // // }


// // // void TimerSpray(){
// // //     unsigned long currentMillis = millis();
// // //     digitalWrite(pinRelayPump, HIGH);
// // //     if(currentMillis - previousMillis > sprayOnInterval) {
// // //       previousMillis = currentMillis; 
// // //       Serial.println("Spraying Off.."); 
// // //       isSpraying = true;
// // //     }

// // //     if(isSpraying) {
// // //       if(currentMillis - previousMillis > sprayOffInterval) {
// // //         previousMillis = currentMillis; 
// // //         Serial.println("Spraying..");
// // //         isSpraying = false;
// // //       }
// // //     }
// // // // }

// // // void Firebase_Init() {
    
// // //     config.api_key = API_KEY;   // configure firebase API Key
// // //     config.database_url = DATABASE_URL;     // configure firebase realtime database url
// // //     Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);  // Enable WiFi reconnection 

// // //     /* Assign the user sign in credentials */
// // //     auth.user.email = USER_EMAIL;
// // //     auth.user.password = USER_PASSWORD;

// // //     config.database_url = DATABASE_URL;     //Assign the RTDB URL (required) 

// // //     /* Assign the callback function for the long running token generation task */
// // //     config.token_status_callback = tokenStatusCallback;
// // //     Firebase.begin(&config, &auth);
// // //     Firebase.beginStream(fbdo, pathRelayPump);
// // //     Firebase.reconnectWiFi(true);
// // //     Firebase.setReadTimeout(fbdo, 6000 * 60);
// // //     Firebase.setwriteSizeLimit(fbdo, "large");

// // //     Serial.println("Getting User UID");
// // //     while ((auth.token.uid) == "") {
// // //       Serial.print('.');
// // //       delay(1000);
// // //     }
// // //     fuid = auth.token.uid.c_str();
// // //     Serial.print("User UID: ");
// // //     Serial.print(fuid);
// // //     isAuthenticated = true;

// // //     Firebase_RelayInit();
// // //     delay(500);

// // // }

// // // void streamCallback(StreamData data)
// // // {
// // //    Firebase.getString(fbdo, pathRelayPump);
// // //    String value = fbdo.stringData().c_str();

// // //    value == "ON" ? Serial.println("ON NA 1"): Serial.println("OFFF 1");
// // // }

// // // //Global function that notifies when stream connection lost
// // // //The library will resume the stream connection automatically
// // // void streamTimeoutCallback(bool timeout)
// // // {
// // //   if(timeout){
// // //     //Stream timeout occurred
// // //     Serial.println("Stream timeout, resume streaming...");
// // //   }  
// // // }

// // // void CheckChange(){

// // // }

// // // void firebaseLoop(){
// // //     delay(5000);
// // //     // Firebase.setStreamCallback(fbdo, streamCallback, false);
     
// // // }


// // void Firebase_Init()
// // {

// //   Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

// //   /* Assign the api key (required) */
// //   config.api_key = API_KEY;

// //   /* Assign the user sign in credentials */
// //   auth.user.email = USER_EMAIL;
// //   auth.user.password = USER_PASSWORD;

// //   /* Assign the RTDB URL (required) */
// //   config.database_url = DATABASE_URL;

// //   /* Assign the callback function for the long running token generation task */
// //   config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h

// //   //Or use legacy authenticate method
// //   //config.database_url = DATABASE_URL;
// //   //config.signer.tokens.legacy_token = "<database secret>";

// //   //To connect without auth in Test Mode, see Authentications/TestMode/TestMode.ino

// //   Firebase.begin(&config, &auth);

// //   Firebase.reconnectWiFi(true);

// //   if (!Firebase.beginStream(fbPump, pathRelayPump))
// //     Serial.printf("sream begin error, %s\n\n", fbPump.errorReason().c_str());

// //   if (!Firebase.beginStream(fbLight, pathRelayLight))
// //     Serial.printf("sream begin error, %s\n\n", fbLight.errorReason().c_str());

// //   if (!Firebase.beginStream(fbPhDown, pathRelayPhDown))
// //     Serial.printf("sream begin error, %s\n\n", fbPhDown.errorReason().c_str());

// //   if (!Firebase.beginStream(fbPhUp, pathRelayPhUp))
// //     Serial.printf("sream begin error, %s\n\n", fbPhUp.errorReason().c_str());

// //   // if (!Firebase.beginStream(fbNutA, pathRelayNutrientA))
// //   //   Serial.printf("sream begin error, %s\n\n", fbNutA.errorReason().c_str());

// //   // if (!Firebase.beginStream(fbNutB, pathRelayNutrientB))
// //   //   Serial.printf("sream begin error, %s\n\n", fbNutB.errorReason().c_str());

// //   // if (!Firebase.beginStream(fbLux, pathSensor + "lux/value"))
// //   //   Serial.printf("sream begin error, %s\n\n", fbLux.errorReason().c_str());

// //   // if (!Firebase.beginStream(fbHumidity, pathSensor + "humidity/value"))
// //   //   Serial.printf("sream begin error, %s\n\n", fbHumidity.errorReason().c_str());

// //   // if (!Firebase.beginStream(fbPhLevel, pathSensor + "lux/value"))
// //   //   Serial.printf("sream begin error, %s\n\n", fbPhLevel.errorReason().c_str());

// //   // if (!Firebase.beginStream(fbTds, pathSensor + "tds/value"))
// //   //   Serial.printf("sream begin error, %s\n\n", fbTds.errorReason().c_str());

// //   // if (!Firebase.beginStream(fbTemperature, pathSensor + "temperature/value"))
// //   //   Serial.printf("sream begin error, %s\n\n", fbTemperature.errorReason().c_str());

// //   // if (!Firebase.beginStream(fbWaterTemp, pathSensor + "waterTemperature/value"))
// //   //   Serial.printf("sream begin error, %s\n\n", fbWaterTemp.errorReason().c_str());

  
// //   /** Timeout options, below is default config.

// //   //WiFi reconnect timeout (interval) in ms (10 sec - 5 min) when WiFi disconnected.
// //   config.timeout.wifiReconnect = 10 * 1000;

// //   //Socket begin connection timeout (ESP32) or data transfer timeout (ESP8266) in ms (1 sec - 1 min).
// //   config.timeout.socketConnection = 30 * 1000;

// //   //ESP32 SSL handshake in ms (1 sec - 2 min). This option doesn't allow in ESP8266 core library.
// //   config.timeout.sslHandshake = 2 * 60 * 1000;

// //   //Server response read timeout in ms (1 sec - 1 min).
// //   config.timeout.serverResponse = 10 * 1000;

// //   //RTDB Stream keep-alive timeout in ms (20 sec - 2 min) when no server's keep-alive event data received.
// //   config.timeout.rtdbKeepAlive = 45 * 1000;

// //   //RTDB Stream reconnect timeout (interval) in ms (1 sec - 1 min) when RTDB Stream closed and want to resume.
// //   config.timeout.rtdbStreamReconnect = 1 * 1000;

// //   //RTDB Stream error notification timeout (interval) in ms (3 sec - 30 sec). It determines how often the readStream
// //   //will return false (error) when it called repeatedly in loop.
// //   config.timeout.rtdbStreamError = 3 * 1000;

// //   */
// // }

// // // void streamCallback(StreamData data)
// // // {
// // //    data == "ON" ? Serial.println("ON NA 1"): Serial.println("OFFF 1");
// // // }

// // String printResult2(FirebaseData &data){
// //     if (data.dataTypeEnum() == fb_esp_rtdb_data_type_string)
// //         // Serial.println(data.to<String>());
// //     return data.to<String>();
// // }


// // /* *******************************************************
// // *   Methods for callback Function 
// // *  *******************************************************/
// // void callbackCheck(FirebaseData &data, int pin){
// //     if (!Firebase.ready())
// //       return;

// //   //Please avoid to use delay or any third party libraries that use delay internally to wait for hardware to be ready in this loop.

// //   if (!Firebase.readStream(data))
// //     Serial.printf("sream read error, %s\n\n", data.errorReason().c_str());


// //   if (data.streamTimeout())
// //   {
// //     Serial.println("stream timed out, resuming...\n");

// //     if (!data.httpConnected())
// //       Serial.printf("error code: %d, reason: %s\n\n", data.httpCode(), data.errorReason().c_str());
// //   }

// //   if (data.streamAvailable())
// //   {
// //     Serial.printf("sream path, %s\nevent path, %s\ndata type, %s\nevent type, %s\n\n",
// //                   data.streamPath().c_str(),
// //                   data.dataPath().c_str(),
// //                   data.dataType().c_str(),
// //                   data.eventType().c_str());
// //     printResult2(data); //see addons/RTDBHelper.h
// //     // printResult2(data) == "ON" ? Serial.println("ON1") : Serial.println("OFF1");
// //     if(printResult2(data) == "ON"){
// //       digitalWrite(pin, HIGH);
// //       Serial.println("ON");
// //     }
// //     else{
// //       digitalWrite(pin, LOW);
// //       Serial.println("OFF");
// //     }

// //     Serial.println();
    

// //     //This is the size of stream payload received (current and max value)
// //     //Max payload size is the payload size under the stream path since the stream connected
// //     //and read once and will not update until stream reconnection takes place.
// //     //This max value will be zero as no payload received in case of ESP8266 which
// //     //BearSSL reserved Rx buffer size is less than the actual stream payload.
// //     Serial.printf("Received stream payload size: %d (Max. %d)\n\n", data.payloadLength(), data.maxPayloadLength());
// //   }
// // }


// // void firebaseLoop()
// // {
// //     callbackCheck(fbPump, pinRelayPump);
// //     callbackCheck(fbLight, pinRelayLight);
// //     callbackCheck(fbPhDown, pinRelayPhDown);
// //     callbackCheck(fbPhUp, pinRelayPhUp);
// //     // callbackCheck(fbFan, pinRelayFan);
// //     // callbackCheck(fbNutA, pinRelayNutrientA);
// //     // callbackCheck(fbNutB, pinRelayNutrientB);
// // }


#include <Arduino.h>
#include "DFRobot_ESP_PH_WITH_ADC.h"
#include "EEPROM.h"


DFRobot_ESP_PH_WITH_ADC ph;
#define ESPADC 4096.0   //the esp Analog Digital Convertion value
#define ESPVOLTAGE 3000 //the esp voltage supply value
#define PH_PIN 35    //the esp gpio data pin number
float voltage, phValue, temperature = 25;

#define SERIAL Serial
#define sensorPin 25

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
    Serial.println(voltage, 4);
    
    //temperature = readTemperature();  // read your temperature sensor to execute temperature compensation
    Serial.print("temperature:");
    Serial.print(temperature, 1);
    Serial.println("^C");

    phValue = ph.readPH(voltage, temperature); // convert voltage to pH with temperature compensation
    Serial.print("pH:");
    Serial.println(phValue, 4);
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
    Serial.print("voltage:");
    Serial.println(voltage, 4);
    
    //temperature = readTemperature();  // read your temperature sensor to execute temperature compensation
    Serial.print("temperature:");
    Serial.print(temperature, 1);
    Serial.println("^C");

    phValue = ph.readPH(voltage, temperature); // convert voltage to pH with temperature compensation
    Serial.print("pH:");
    Serial.println(phValue, 4);
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