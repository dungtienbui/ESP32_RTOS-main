void setup() {
    // Cấu hình chân PWM
    pinMode(D5, OUTPUT);
}

void loop() {
    // Thay đổi chu kỳ duty cycle PWM
    for (int dutyCycle = 0; dutyCycle <= 255; dutyCycle++) {
        analogWrite(D5, dutyCycle); // Điều chỉnh tốc độ quay của quạt
        delay(10); // Chờ 10 mili giây
    }

    // Thay đổi chu kỳ duty cycle PWM theo hướng ngược lại
    for (int dutyCycle = 255; dutyCycle >= 0; dutyCycle--) {
        analogWrite(D5, dutyCycle); // Điều chỉnh tốc độ quay của quạt
        delay(10); // Chờ 10 mili giây
    }
}