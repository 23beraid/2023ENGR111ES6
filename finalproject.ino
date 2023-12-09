#include <DHT.h>
#include <ESP32Servo.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <WiFiClientSecure.h>
#include "SystemLogic.h"

// Pin definitions
const int DHT_PIN = 18;
const int PHOTOCELL_PIN = 34;
const int SOIL_PIN = 35;
const int SERVO_PIN = 4;

// WiFi credentials
const char* WIFI_NAME = "Stevens-IoT";
const char* WIFI_PASS = "nMN882cmg7";

// MQTT configuration
const char* MQTT_SERVER = "98db5050a791439c98eac188febfecbe.s2.eu.hivemq.cloud";
const char* MQTT_USER = "stevens";
const char* MQTT_PASS = "Stevens@1870";
const int MQTT_PORT = 8883;

// Team information
const String YEAR = "2023";
const String CLASS = "ENGR111";
const String SECTION = "E";
const String GROUP_NUMBER = "S6";
String PAYLOAD;

// Sensor configuration
const int SOIL_MAXWET = 2640;
const int SOIL_MAXDRY = 3260;

const int SERVO_OPEN_ANGLE = 110;
const int SERVO_CLOSE_ANGLE = 20;

void setup() {
  wifi_start();
  mqtt_start();

  pinMode(DHT_PIN, INPUT);
  pinMode(PHOTOCELL_PIN, INPUT);
  pinMode(SOIL_PIN, INPUT);
  pinMode(SERVO_PIN, INPUT);

  dht.begin();

  Serial.begin(115200);
}

const int publishInterval = 10000;

void loop() {
 
  myservo.attach(SERVO_PIN, 500, 2500);
  myservo.write(SERVO_CLOSE_ANGLE);

  int photo = analogRead(PHOTOCELL_PIN);
  Serial.print("Photocell Value: ");
  Serial.println(photo);
  
  int soilhumidity = analogRead(SOIL_PIN);
  int soilPercent = map(soilhumidity, SOIL_MAXWET, SOIL_MAXDRY, 100, 0);
  Serial.print("Soil Humidity Reading: ");
  Serial.print(soilhumidity);
  Serial.println("");
  myservo.write(SERVO_CLOSE_ANGLE);
  delay(2000);
  char pubString[8];
  dtostrf(soilPercent, 1, 2, pubString);  //dtostrf(float_value, min_width, num_digits_after_decimal, where_to_store_string)
  String soilTopic = YEAR + "/" + CLASS + "/" + SECTION + "/" + GROUP_NUMBER + "/" + "soil_hum";
  
  //Non-blocking MQTT publish & delay
  client.publish(soilTopic.c_str(), pubString);

while(soilhumidity >= 2600 )
{
  myservo.write(SERVO_OPEN_ANGLE);
  delay(3000);
  soilhumidity = analogRead(SOIL_PIN);
  soilPercent = map(soilhumidity, SOIL_MAXWET, SOIL_MAXDRY, 100, 0);
  Serial.print("Soil Humidity Reading: ");
  Serial.print(soilhumidity);
  Serial.println(""); }
  
 
  myservo.detach();
  }
