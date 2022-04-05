#include <FirebaseESP32.h> 
#include <LiquidCrystal_I2C.h>

// For header Initialization
extern void Wifi_Init();
extern void Firebase_Init();
extern void printLocalTime();
extern void Dht_Init();
extern void Ph_Init();
extern void Lux_Init();
extern void LCDsetup();
extern void Water_Init();
extern void WaterLevel_Init();

// For header getting sensor value
extern float TemperatureValue();
extern float HumidityValue();
extern float PhValue();
extern float Ph_Loop();
extern float TdsValue2();
extern float waterTemp();
extern String WaterLevelValue();
extern float LuxValue();
extern String getTimeRelay();

// For header Test purpose
extern void Lux_Loop();
extern void firebaseLoop();
extern void relayTimer(String pathTimeStart, String pathTimeEnd);
extern void FirebaseCheckWakeUp(float on, float off);


#define USER_EMAIL "qcurban.iot@gmail.com"
#define USER_PASSWORD "Password123"
#define API_KEY "AIzaSyCLrKZG6RqtlHyyPL2ndZG0mcxQxZCOTxs"
#define DATABASE_URL "aeroponics-iot-8e4f4-default-rtdb.asia-southeast1.firebasedatabase.app" //<databaseName>.firebaseio.com or <databaseName>.<region>.firebasedatabase.app
#define FIREBASE_HOST "aeroponics-iot-8e4f4.firebaseio.com"