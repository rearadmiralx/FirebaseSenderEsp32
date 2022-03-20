#include "header.h"

String getLocalTime(){
  struct tm timeinfo2;
  if(!getLocalTime(&timeinfo2)){
    Serial.println("Failed to obtain time");
    return "";
  }
  // day , month date year hour:mins:sec
  // Serial.println(&timeinfo2, "%H:%M:%S %p");
  String dateVal = (&timeinfo2, "%A, %B %d %Y %H:%M:%S");
  return dateVal;
}

String getTimeRelay(){
  delay(500);
  struct tm timeinfo2;
  if(!getLocalTime(&timeinfo2)){
    Serial.println("Failed to obtain time");
    return "";
  }
  // day , month date year hour:mins:sec
  
  char timeStringBuff[50]; //50 chars should be enough
  strftime(timeStringBuff, sizeof(timeStringBuff), "%H.%M", &timeinfo2);
  //print like "const char*"
  // Serial.println(timeStringBuff);

  //Optional: Construct String object 
  return timeStringBuff;
}

