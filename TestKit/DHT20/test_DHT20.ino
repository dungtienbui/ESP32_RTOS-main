// Kiểm tra cảm biến nhiệt độ và độ ẩm môi trường.

#include "LiquidCrystal_I2C.h";
#include "DHT20.h"

#include <Arduino.h>

// Khởi tạo màn hình LCD
LiquidCrystal_I2C lcd(0x21, 16, 2);

// Khởi tạo đối tượng DHT20
DHT20 DHT;

void setup() {
    //Initialize the LCD
    lcd.init();
    // Turn on the blacklight
    lcd.backlight();
}

void loop() {
    DHT.read(); // Đọc dữ liệu từ cảm biến

    // In giá trị ra màn hình LCD
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Temp: ");
    lcd.print((DHT.getTemperature()));
    lcd.setCursor(0, 1);
    lcd.print("Humi: ");
    lcd.print((DHT.getHumidity()));

    delay(5000); // 5s đo một lần
}