#include <WiFi.h>
#include <MQTTClient.h>
#include <ArduinoJson.h>

#include "LiquidCrystal_I2C.h"
#include "DHT20.h"

const char WIFI_SSID[] = "ACLAB";     // CHANGE TO YOUR WIFI SSID
const char WIFI_PASSWORD[] = "ACLAB2023";  // CHANGE TO YOUR WIFI PASSWORD

const char MQTT_BROKER_ADDRESS[] = "broker.hivemq.com";  // CHANGE TO MQTT BROKER'S ADDRESS
const int MQTT_PORT = 1883;
const char MQTT_CLIENT_ID[] = "dung-esp32-mqtt";  

const char MQTT_USERNAME[] = "";                        
const char MQTT_PASSWORD[] = "";                        


// Define your tasks here

void TemperatureHumiditySensor(void *pvParameters);

void TaskLightSensor(void *pvParameters);


// Define your components here
DHT20 DHT;
LiquidCrystal_I2C lcd(0x21, 16, 2);

WiFiClient network;
MQTTClient mqtt = MQTTClient(256);

void setup() {

  Serial.begin(115200);
  Serial.println("Setup started");

  pinMode(D13, OUTPUT);

  DHT.begin();
  lcd.init();
  lcd.backlight();

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.println("ESP32 - Connecting to Wi-Fi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to Wi-Fi");

  connectToMQTT();

  xTaskCreate(TemperatureHumiditySensor, "Task Temperature" ,4096, NULL, 1, NULL); // Increased stack size
  xTaskCreate(TaskLightSensor, "Task Light and LED" ,4096, NULL, 2, NULL); // Increased stack size

  Serial.println("Setup completed");
}

void loop() {
}


void TemperatureHumiditySensor(void *pvParameters) {
  while(1) {
    mqtt.loop();

    Serial.println("Task Temperature and Humidity");

    DHT.read();

    StaticJsonDocument<200> message;
    message["timestamp"] = millis();
    message["Temp"] = DHT.getTemperature();
    message["Humi"] = DHT.getHumidity();

    char messageBuffer[512];
    serializeJson(message, messageBuffer);

    mqtt.publish("dung/MQTT/TemperatureHumiditySensor", messageBuffer);

    delay(5000);
  }
}


void TaskLightSensor(void *pvParameters) {
  while(1) {
    mqtt.loop();

    Serial.println("Task Light and LED");

    Serial.println(analogRead(A0));

    StaticJsonDocument<200> message;
    message["timestamp"] = millis();
    message["data"] = analogRead(A0);
    char messageBuffer[512];
    serializeJson(message, messageBuffer);

    mqtt.publish("dung/MQTT/LightSensor", messageBuffer);


    delay(3000);
  }
}


void connectToMQTT() {
  mqtt.begin(MQTT_BROKER_ADDRESS, MQTT_PORT, network);

  mqtt.onMessage(messageHandler);

  Serial.print("ESP32 - Connecting to MQTT broker");

  while (!mqtt.connect(MQTT_CLIENT_ID, MQTT_USERNAME, MQTT_PASSWORD)) {
    Serial.print(".");
    delay(100);
  }
  Serial.println("\nConnected to MQTT broker");

  if (!mqtt.connected()) {
    Serial.println("ESP32 - MQTT broker Timeout!");
    return;
  }

  if (mqtt.subscribe("dung/MQTT/TemperatureHumiditySensor"))
    Serial.print("ESP32 - Subscribed to the topic: ");
  else
    Serial.print("ESP32 - Failed to subscribe to the topic: ");

  Serial.println("dung/MQTT/TemperatureHumiditySensor");

  if (mqtt.subscribe("dung/MQTT/LightSensor"))
    Serial.print("ESP32 - Subscribed to the topic: ");
  else
    Serial.print("ESP32 - Failed to subscribe to the topic: ");

  Serial.println("dung/MQTT/LightSensor");
}


void messageHandler(String &topic, String &payload) {
  
  // Parse the payload
  StaticJsonDocument<200> doc;
  DeserializationError error = deserializeJson(doc, payload);

  if (error) {
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.c_str());
    return;
  }


  if (topic == "dung/MQTT/LightSensor"){
    int data = doc["data"];

    if(data > 500){
      digitalWrite(D13, HIGH);
    }
    if(data < 250){
      digitalWrite(D13, LOW);
    }
  }else if (topic == "dung/MQTT/TemperatureHumiditySensor") {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(int(doc["Temp"]));
    lcd.setCursor(0, 1);
    lcd.print(int(doc["Humi"]));
  }
  
}
