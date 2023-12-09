#include <DHT.h>
#include <ESP32Servo.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <WiFiClientSecure.h>
#include "SystemLogic.h"

// Pin definitions
const int DHT_PIN = 17;
const int PHOTOCELL_PIN = 34;
const int SOIL_PIN = 26;
//const int SERVO_PIN = 2;
//const int SOLENOID_PIN = 4;

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

const int SERVO_OPEN_ANGLE = 160;
const int SERVO_CLOSE_ANGLE = 0;

void setup() {
  Serial.begin(115200);
  wifi_start();
  mqtt_start();
  enable_sensors();
  pinMode(DHT_PIN, INPUT);
  dht.begin();
}

const int publishInterval = 100000;

void loop() {
  mqtt_loop();
//  myservo.attach(SERVO_PIN, 1000, 2000); // attaches the servo on pin 2 to the servo object
//  myservo.write(SERVO_OPEN_ANGLE);
//  delay(500); //This delay contributes to open time
//  myservo.attach(SERVO_PIN, 1000, 2000); // attaches the servo on pin 2 to the servo object
//  myservo.write(SERVO_CLOSE_ANGLE);
//  delay(500); //This delay contributes to close time
 
  int photo = analogRead(PHOTOCELL_PIN);
  Serial.print("Photocell Value: ");
  Serial.println(photo);
 
  int humidity = dht.readHumidity();
  int temperature = dht.readTemperature();

  int soilhumidity = analogRead(SOIL_PIN);
  int soilPercent = map(soilhumidity, SOIL_MAXWET, SOIL_MAXDRY, 100, 0);
  Serial.print("Soil Humidity Reading: ");
  Serial.print(soilPercent);
  Serial.println(" %");
 
  char pubString1[8];
  char pubString2[8];
  char pubString3[8];
  char pubString4[8];
  dtostrf(soilPercent, 1, 2, pubString1);  //dtostrf(float_value, min_width, num_digits_after_decimal, where_to_store_string)
  String soilTopic = YEAR + "/" + CLASS + "/" + SECTION + "/" + GROUP_NUMBER + "/" + "soil_hum";
 
  dtostrf(temperature, 1, 2, pubString2);  //dtostrf(float_value, min_width, num_digits_after_decimal, where_to_store_string)
  String tempTopic = YEAR + "/" + CLASS + "/" + SECTION + "/" + GROUP_NUMBER + "/" + "temp";

  dtostrf(humidity, 1, 2, pubString3);  //dtostrf(float_value, min_width, num_digits_after_decimal, where_to_store_string)
  String humTopic = YEAR + "/" + CLASS + "/" + SECTION + "/" + GROUP_NUMBER + "/" + "hum";

  dtostrf(photo, 1, 2, pubString4);  //dtostrf(float_value, min_width, num_digits_after_decimal, where_to_store_string)
  String luxTopic = YEAR + "/" + CLASS + "/" + SECTION + "/" + GROUP_NUMBER + "/" + "lux";
  //Non-blocking MQTT publish & delay
  client.publish(soilTopic.c_str(), pubString1);
  client.publish(humTopic.c_str(), pubString2);
  client.publish(tempTopic.c_str(), pubString3);
  client.publish(luxTopic.c_str(), pubString4);

  delay(500); // 100secs delay for publishing

  myservo.detach();
  }

