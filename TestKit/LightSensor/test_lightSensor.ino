#include "LiquidCrystal_I2C.h";

// khởi tạo màn hình LCD
LiquidCrystal_I2C lcd(0x21, 16, 2);


void setup() {

    Serial.begin(115200); // Khởi tạo Serial
    //Initialize the LCD
    lcd.init();
    // Turn on the blacklight
    lcd.backlight();

    // Cau hinh chan analog
    pinMode(A0, INPUT);
}

void loop() {

    // Doc gia tri cam bien
    int sensorValue = analogRead(A0);

    // In ra gia tri anh sang
    Serial.print("Light: ");
    Serial.println(sensorValue); // In voi 2 chu so thap phan

    // in giá trị cảm biến ra màn hình LCD
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(sensorValue);

    delay(1000); // Cho 1 giay
}