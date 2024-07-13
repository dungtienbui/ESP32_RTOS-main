#include <WiFi.h>
#include <MQTTClient.h>
#include <ArduinoJson.h>

#include <stdio.h>

#include "LiquidCrystal_I2C.h"
#include "DHT20.h"

const char WIFI_SSID[] = "ACLAB";         // CHANGE TO YOUR WIFI SSID
const char WIFI_PASSWORD[] = "ACLAB2023"; // CHANGE TO YOUR WIFI PASSWORD

const char MQTT_BROKER_ADDRESS[] = "broker.hivemq.com"; // CHANGE TO MQTT BROKER'S ADDRESS
const int MQTT_PORT = 1883;
const char MQTT_CLIENT_ID[] = "dung-esp32-mqtt";

const char MQTT_USERNAME[] = "";
const char MQTT_PASSWORD[] = "";

const unsigned short NUM_PUBLISH = 2;
const char *const MQTT_PUBLISH[NUM_PUBLISH] = {
    "dung/MQTT/TemperatureHumiditySensor",
    "dung/MQTT/LightSensor"};

const unsigned short NUM_SUBCRIBE = 2;
const char *const MQTT_SUBCRIBE[NUM_SUBCRIBE] = {
    "dung/MQTT/TemperatureHumiditySensor",
    "dung/MQTT/LightSensor"};

// define struct data type
struct ScreenData
{
  char *line0;
  char *line1;
};

// Define your tasks here

// read sensor
void TemperatureHumiditySensor(void *pvParameters);
void TaskLightSensor(void *pvParameters);

// control device
void lebSwitching(void *pvParameters);
void screenShowing(void *pvParameters);

// Define your components here
DHT20 DHT;
LiquidCrystal_I2C lcd(0x21, 16, 2);

WiFiClient network;
MQTTClient mqtt = MQTTClient(256);

void setup()
{

  Serial.begin(115200);
  Serial.println("Setup started");

  pinMode(D13, OUTPUT);

  DHT.begin();
  lcd.init();
  lcd.backlight();

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.println("ESP32 - Connecting to Wi-Fi");

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to Wi-Fi");

  connectToMQTT();

  xTaskCreate(TemperatureHumiditySensor, "Task read DH20 sensor (temperature and huminity).", 4096, NULL, 1, NULL); // Increased stack size
  xTaskCreate(TaskLightSensor, "Task read light sensor.", 4096, NULL, 1, NULL);                                     // Increased stack size

  Serial.println("Setup completed");
}

void loop()
{
}

void TemperatureHumiditySensor(void *pvParameters)
{
  while (1)
  {
    mqtt.loop();

    Serial.println("Task read Temperature&Humidity-sensor.");

    DHT.read();

    StaticJsonDocument<200> message;
    message["timestamp"] = millis();
    message["Temperature"] = DHT.getTemperature();
    message["humidity"] = DHT.getHumidity();

    char messageBuffer[512];
    serializeJson(message, messageBuffer);

    mqtt.publish("dung/MQTT/TemperatureHumiditySensor", messageBuffer);

    delay(5000);
  }
}

void TaskLightSensor(void *pvParameters)
{
  while (1)
  {
    mqtt.loop();

    Serial.println("Task read light-sensor.");

    Serial.println(analogRead(A0));

    StaticJsonDocument<200> message;
    message["timestamp"] = millis();
    message["lightIntensity"] = analogRead(A0);
    char messageBuffer[512];
    serializeJson(message, messageBuffer);

    mqtt.publish("dung/MQTT/LightSensor", messageBuffer);

    delay(3000);
  }
}

void connectToMQTT()
{
  mqtt.begin(MQTT_BROKER_ADDRESS, MQTT_PORT, network);

  mqtt.onMessage(messageHandler);

  Serial.print("ESP32 - Connecting to MQTT broker");

  while (!mqtt.connect(MQTT_CLIENT_ID, MQTT_USERNAME, MQTT_PASSWORD))
  {
    Serial.print(".");
    delay(100);
  }
  Serial.println("\nConnected to MQTT broker");

  if (!mqtt.connected())
  {
    Serial.println("ESP32 - MQTT broker Timeout!");
    return;
  }
  for (int i = 0; i < NUM_SUBSCRIBE; i++)
  {
    if (mqtt.subscribe(MQTT_SUBSCRIBE[i]))
    {
      Serial.print("ESP32 - Subscribed to the topic: ");
      Serial.println(MQTT_SUBSCRIBE[i]);
    }
    else
    {
      Serial.print("ESP32 - Failed to subscribe to the topic: ");
      Serial.println(MQTT_SUBSCRIBE[i]);
    }
  }
}

void messageHandler(String &topic, String &payload)
{

  // Parse the payload
  StaticJsonDocument<200> doc;
  DeserializationError error = deserializeJson(doc, payload);

  if (error)
  {
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.c_str());
    return;
  }

  switch (topic)
  {
  case "dung/MQTT/LightSensor":
  {
    int lightIntensity = doc["lightIntensity"];
    // lebSwitching(void *(&lightIntensity));
    xTaskCreate(lebSwitching, "turn leb.", 4096, (void *(&lightIntensity), 2, NULL);
    
    break;
  }
  case "dung/MQTT/TemperatureHumiditySensor":
  {
    ScreenData screenData;
    char line0[17];
    snprintf(str, 17, "%d", int(doc["Temperature"]));
    char line1[17];
    snprintf(str, 17, "%d", int(doc["humidity"]));
    screenData.line0 = &line0;
    screenData.line1 = &line1;
    // screenShowing(void *(&screenData));
    xTaskCreate(screenShowing, "showing data in screen.", 4096, (void *(&screenData), 2, NULL);
    break;
  }
  default:
  {
    Serial.print("Do not handle for topic: ");
    Serial.print(topic);
  }
  }
}

void lebSwitching(void *pvParameters)
{
  if (*((int *)pvParameters) > 500)
  {
    digitalWrite(D13, HIGH);
  }
  else if (data < 250)
  {
    digitalWrite(D13, LOW);
  }
}

void screenShowing(void *pvParameters)
{
  ScreenData* data = (ScreenData*)pvParameters;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(*((*data)[0]));
  lcd.setCursor(0, 1);
  lcd.print(*((*data)[1]));
}
