#include <WiFi.h> // Wifi library
#include "time.h"
#include "header.h"

// Your WiFi credentials
// #define WIFI_SSID "GlobeAtHome_76C92"
// #define WIFI_PASSWORD "AB35RM6L4T1"

// #define WIFI_SSID "Redmitest"
// #define WIFI_PASSWORD "123456780"

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 28800;
const int   daylightOffset_sec = 0;
int wificount = 0;
// #define WIFI_SSID "Sabihin_mo_please_master"
// #define WIFI_PASSWORD "iSeeDEADpeople!@#123"

#define WIFI_SSID "guest"
#define WIFI_PASSWORD "EzzySudo,.CHMOD"


//User sa HarveyBahay "GlobeAtHome_E1DB2"
//Pass sa Bahay "passwordnatin"

//User kila Jim "Pisonet"
//Pass kila Jim "nicopanget"

void printLocalTime()
{
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %I:%M:%S %p");
}

void Wifi_Init() {
    WiFi.disconnect();
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("Connecting to Wi-Fi");

    while (WiFi.status() != WL_CONNECTED){
      Serial.print(".");
      delay(300);
      wificount++;
      if(wificount > 16)
        ESP.restart();
    }

    Serial.println();
    Serial.print("Connected with IP: ");
    Serial.println(WiFi.localIP());
    Serial.println();

    //init and get the time
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    printLocalTime();
    // Serial.println(getTimeRelay());

    //disconnect WiFi as it's no longer needed
    // WiFi.disconnect(true);
    // WiFi.mode(WIFI_OFF);
}