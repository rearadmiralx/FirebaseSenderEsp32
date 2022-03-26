#include "header.h"

int FloatSensor=15;            
int buttonState = 1; //reads pushbutton status
String returnValue;

void WaterLevel_Init() 
{ 
    pinMode(FloatSensor , INPUT_PULLUP);
} 


String WaterLevelValue() 
{ 

    buttonState = digitalRead(FloatSensor); 
    buttonState == HIGH ? returnValue = "HIGH" :returnValue= "LOW";

    return returnValue;
}