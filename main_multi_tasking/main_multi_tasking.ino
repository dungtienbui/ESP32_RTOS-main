#include "Adafruit_NeoPixel.h";
#include "LiquidCrystal_I2C.h";
#include "DHT20.h";


// Define your tasks here
void TaskBlink(void *pvParameters);
void TaskTemperatureHumidity(void *pvParameters);
void TaskLightSensorAndLebRGB(void *pvParameters);
void TaskSoilMoistureAndRelay(void *pvParameters);
void TaskPirSensorAndLebRGB(void *pvParameters);


//Define your components here
Adafruit_NeoPixel rgb(4, D3, NEO_GRB + NEO_KHZ800);
LiquidCrystal_I2C lcd(0x21,16,2); // địa chỉ màn hình là 0x21 hexa
DHT20 DHT;


void setup() {
  

  // Initialize serial communication at 115200 bits per second:
  Serial.begin(115200); 

  rgb.begin();
  //Initialize the LCD
  
  lcd.init();
  // Turn on the blacklight
  lcd.backlight();
  
  xTaskCreate( TaskBlink, "Task Blink" ,2048  ,NULL  ,2 , NULL);
  xTaskCreate( TaskTemperatureHumidity, "Task Temperature and LCD screen" ,2048  ,NULL  ,2 , NULL);
  xTaskCreate( TaskLightSensorAndLebRGB, "Task light sensor and LebRGB", 2048, NULL, 1, NULL);
  xTaskCreate( TaskSoilMoistureAndRelay, "Task Soil Moisture and Relay", 2048, NULL, 1, NULL);
  xTaskCreate( TaskPirSensorAndLebRGB, "Task pir sensor and LebRGB", 2048, NULL, 1, NULL);
  
  //Now the task scheduler is automatically started.
  Serial.printf("Basic Multi Threading Arduino Example\n");

}

void loop() {
}

/*--------------------------------------------------*/
/*---------------------- Tasks ---------------------*/
/*--------------------------------------------------*/



void TaskBlink(void *pvParameters) {  // This is a task.
  //uint32_t blink_delay = *((uint32_t *)pvParameters);

  // initialize digital LED_BUILTIN on pin 13 as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  

  while(1) {                          
    digitalWrite(LED_BUILTIN, HIGH);  // turn the LED ON
    Serial.println("Leb on");
    delay(2000);
    digitalWrite(LED_BUILTIN, LOW);  // turn the LED OFF
    Serial.println("Leb off");
    delay(2000);
  }
}


void TaskTemperatureHumidity(void *pvParameters) {  // This is a task.
  //uint32_t blink_delay = *((uint32_t *)pvParameters);

  int i = 0;

  while(1) {                      
    lcd.setCursor(12, 0);
    lcd.print(i);
    if (i%5 == 0){
      Serial.print("Measure Temperature and Humidity time: ");
      Serial.println(i);
      DHT.read();
      lcd.setCursor(0, 0);
      lcd.print("Temp: ");
      lcd.print((DHT.getTemperature()));
      lcd.setCursor(0, 1);
      lcd.print("Humi: ");
      lcd.print((DHT.getHumidity()));
      delay(1000);

    }
    delay(1000);

    i = (i + 1);
  }
}

void TaskLightSensorAndLebRGB(void *pvParameters) {
  pinMode(A0, INPUT);

  while(1) {
    int sensorValue = analogRead(A0);

    Serial.print("Task Light and LED - value: ");
    Serial.println(sensorValue);
    
    if (sensorValue <= 350) {
      rgb.setPixelColor(0, rgb.Color(255,102,0));
      // rgb.setPixelColor(1, rgb.Color(255,102,0));
      rgb.setPixelColor(2, rgb.Color(255,102,0));
      // rgb.setPixelColor(3, rgb.Color(255,102,0));
      rgb.show();
    }

    if (sensorValue >= 650) {
      rgb.setPixelColor(0, rgb.Color(0,0,0));
      // rgb.setPixelColor(1, rgb.Color(0,0,0));
      rgb.setPixelColor(2, rgb.Color(0,0,0));
      // rgb.setPixelColor(3, rgb.Color(0,0,0));
      rgb.show();
    }

    delay(1000);
  }
}

void TaskSoilMoistureAndRelay(void *pvParameters) {
  pinMode(A1, INPUT);
  pinMode(D5, OUTPUT);

  while (1) {
    Serial.println("Task Soild and Relay");
    
    if(analogRead(A1) > 500){
      digitalWrite(D5, LOW);
    }
    if(analogRead(A1) < 50){
      digitalWrite(D5, HIGH);
    }

    delay(1000);
  }

}

void TaskPirSensorAndLebRGB(void *pvParameters) {
  pinMode(D7, INPUT);
  
  while (1) {
    Serial.print("Task pir sensor - value: ");
    Serial.println(digitalRead(D7));

    if (digitalRead(D7)) {
      // rgb.setPixelColor(0, rgb.Color(255,102,0));
      rgb.setPixelColor(1, rgb.Color(255,102,0));
      // rgb.setPixelColor(2, rgb.Color(255,102,0));
      rgb.setPixelColor(3, rgb.Color(255,102,0));
      rgb.show();

      delay(3000);
    }

    if (digitalRead(D7) == 0) {
      // rgb.setPixelColor(0, rgb.Color(255,102,0));
      rgb.setPixelColor(1, rgb.Color(0,0,0));
      // rgb.setPixelColor(2, rgb.Color(255,102,0));
      rgb.setPixelColor(3, rgb.Color(0,0,0));
      rgb.show();
    }

    
    delay(1000);
  }
}
