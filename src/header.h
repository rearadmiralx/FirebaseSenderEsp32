#include <FirebaseESP32.h> 
#include <LiquidCrystal_I2C.h>



extern void Wifi_Init();
extern void printLocalTime();
extern void Firebase_Init();
extern void Firebase_RelayInit();
extern void Firebase_CallLoop();
extern void TimerSpray();
extern void LCDsetup();

extern void Dht_Init();
extern float TemperatureValue();
extern float HumidityValue();

extern void Firebase_TimerLoop();
extern void TimerSpray();
extern void firebaseLoop();
extern float PhValue();
extern float Ph_Loop();
extern void Ph_Init();
extern float TdsValue2();
extern float waterTemp(void);

extern void Lux_Init();
extern float LuxValue();
extern void Lux_Loop();

extern String getTimeRelay();
extern void relayTimer(String pathTimeStart, String pathTimeEnd);
extern void FirebaseCheckWakeUp(float on, float off);

extern void relayON(FirebaseJson relayJson);
extern void relayOFF(FirebaseJson relayJson);
extern void Firebase_RelayLoop();
extern void relayUpload(String path, String relayStatus);
extern void uploadSensorData(String status, String locationPath);