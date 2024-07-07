#include <SoftwareSerial.h>
#include <ArduinoJson.h> // Include ArduinoJson library

// Define sensor pins (adjust based on your setup)
const int currentSolarPV = A0;
const int voltageSolarPV = A1;
const int currentBattery = A2;
const int voltageBattery = A3;
const int currentLED = A4;
const int temperatureSensor = A5;

// Define ESP8266 communication pins
const int espRx = 2;
const int espTx = 3;

SoftwareSerial ESP8266(espRx, espTx);
StaticJsonDocument<256> jsonDoc; // Allocate memory for JSON data (adjust capacity as needed)

void setup() {
  Serial.begin(9600);
  ESP8266.begin(9600);

  // Configure sensor pins as inputs
  pinMode(currentSolarPV, INPUT);
  pinMode(voltageSolarPV, INPUT);
  pinMode(currentBattery, INPUT);
  pinMode(voltageBattery, INPUT);
  pinMode(currentLED, INPUT);
  pinMode(temperatureSensor, INPUT);
}

void loop() {
  // Read sensor values
  int currentSolarPV_raw = analogRead(currentSolarPV);
  float voltageSolarPV_raw = analogRead(voltageSolarPV) * (5.0 / 1023.0);
  int currentBattery_raw = analogRead(currentBattery);
  float voltageBattery_raw = analogRead(voltageBattery) * (5.0 / 1023.0);
  int currentLED_raw = analogRead(currentLED);
  float temperature_raw = analogRead(temperatureSensor) * (5.0 / 1023.0) * (100.0 / 5.0);

  // Create a JSON object
  JsonObject root = jsonDoc.to<JsonObject>();
  root["currentSolarPV"] = currentSolarPV_raw;
  root["voltageSolarPV"] = voltageSolarPV_raw;
  root["currentBattery"] = currentBattery_raw;
  root["voltageBattery"] = voltageBattery_raw;
  root["currentLED"] = currentLED_raw;
  root["temperatureSensor"] = temperature_raw;

  // Serialize JSON data to a String
  String jsonString;
  serializeJson(jsonDoc, jsonString);

  // Print JSON data to serial monitor for debugging (optional)
  Serial.println(jsonString);

  // Send JSON data to ESP8266
  ESP8266.println(jsonString);

  delay(15000); // Read sensor data every 15 seconds
}
