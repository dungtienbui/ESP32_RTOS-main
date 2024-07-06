#include "Adafruit_NeoPixel.h";
#include "LiquidCrystal_I2C.h";
#include "DHT20.h";



// Define your tasks here
void TaskBlink(void *pvParameters);
void TaskTemperatureHumidity(void *pvParameters);

//Define your components here
Adafruit_NeoPixel rgb(4, D3, NEO_GRB + NEO_KHZ800);
// init LCD
LiquidCrystal_I2C lcd(0x21,16,2); // địa chỉ màn hình là 0x21 hexa
// init DHT20 - do do am va nhiet do
DHT20 DHT;


void setup() {
  rgb.begin();


  // Initialize serial communication at 115200 bits per second:
  Serial.begin(115200); 
  
  xTaskCreate( TaskBlink, "Task Blink" ,2048  ,NULL  ,2 , NULL);
  xTaskCreate( TaskTemperatureHumidity, "Task Temperature" ,2048  ,NULL  ,2 , NULL);
  xTaskCreate( TaskLebRGB, "Task LebRGB", 2048, NULL, 1, NULL);
  
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

  //Initialize the LCD
  lcd.init();
  // Turn on the blacklight
  lcd.backlight();

  int i = 0;

  while(1) {
    i = i + 1;                          
    Serial.println(i);
    if (i%5 == 0){
      Serial.print("Measure Temperature and Humidity");
      DHT.read();
      lcd.setCursor(12, 0);
      lcd.print(i);
      lcd.setCursor(0, 0);
      lcd.print("Temp: ");
      lcd.print((DHT.getTemperature()));
      lcd.setCursor(0, 1);
      lcd.print("Humi: ");
      lcd.print((DHT.getHumidity()));
      delay(1000);
    }
    lcd.setCursor(12, 0);
    lcd.print(i);
    delay(1000);
  }
}

void TaskLebRGB(void *pvParameters) {
  int i = 0;
  while(1){
    rgb.setPixelColor(i, rgb.Color(1,1,1));
    rgb.show();
    delay(1000);

    rgb.setPixelColor(i, rgb.Color(0,0,0));
    rgb.show();
    delay(1000);

    i = (i + 1)%4;
  }
}
