#include <Arduino.h>
#include "HCSR04.h";
#include "SoftServo.h";

UltraSonicDistanceSensor ultrasonic(D3, D4); //cảm biến nối với D3-D4
SoftServo myservo;

void setup() {
    myservo.attach(D2); // servo nối với cổng D2
    myservo.asyncMode();
    myservo.delayMode();

    myservo.tick();
    myservo.write(90);
}

void loop() {
    if ((ultrasonic.measureDistanceCm() < 10)) {
        myservo.tick();
        myservo.write(180);
        delay(3000);
        myservo.tick();
        myservo.write(90);
    } else {
        myservo.tick();
        myservo.write(90);
    }
}