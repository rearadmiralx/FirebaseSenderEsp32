#include "header.h"
LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display
void LCDsetup()
{
  lcd.init();                      // initialize the lcd 
  // Print a message to the LCD.
  lcd.backlight();
  // lcd.setCursor(3,0);
  // lcd.print("AndRew Potsu!");
  // lcd.setCursor(2,1);
  // lcd.print("Love Buche!");
  //  lcd.setCursor(0,2);
  // lcd.print("Arduino LCM IIC 2004");
  //  lcd.setCursor(2,3);
  // lcd.print("Power By Ec-yuan!");
}