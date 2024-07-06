#include "LiquidCrystal_I2C.h";
#include <Arduino.h>

LiquidCrystal_I2C lcd(0x21,16,2); // địa chỉ màn hình là 0x21 hexa

void setup() {
    //Initialize the LCD
    lcd.init();
    // Turn on the blacklight
    lcd.backlight();
}

void loop() {
    lcd.clear();
    // đặt con trỏ văn bản ở hàng 0 cột 0
    lcd.setCursor(0, 0);
    lcd.print("hello OhStem");

    // đặt con trỏ văn bản ở hàng 1 cột 0
    lcd.setCursor(0, 1);
    lcd.print(123);
    delay(5000);
}