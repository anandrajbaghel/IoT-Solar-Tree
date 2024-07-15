#include <SoftwareSerial.h>
#include <ArduinoJson.h>
#include <DHT.h>

#define DHTPIN 3 // Pin where the DHT sensor is connected
#define DHTTYPE DHT11

// Define sensor pins (adjust based on your setup)
const int currentSolarPV = A0;
const int voltageSolarPV = A1;
const int currentBattery = A2;
const int voltageBattery = A3;
const int currentLED = A4;
const float voltage_factor = 5.1;

DHT dht(DHTPIN, DHTTYPE);

// Define ESP8266 communication pins
const int espRx = 2;
const int espTx = 3;

SoftwareSerial ESP8266(espRx, espTx);
StaticJsonDocument<512> jsonDoc; // Allocate memory for JSON data (adjust capacity as needed)

void setup() {
  dht.begin();
  Serial.begin(9600);
  ESP8266.begin(9600);

  // Configure sensor pins as inputs
  pinMode(currentSolarPV, INPUT);
  pinMode(voltageSolarPV, INPUT);
  pinMode(currentBattery, INPUT);
  pinMode(voltageBattery, INPUT);
  pinMode(currentLED, INPUT);
}

void loop() {

  // Read sensor values
  // Solar
  int currentSolarPV_raw = (analogRead(currentSolarPV) * (5.0 / 1023.0) - 2.5) / 0.185;
  float voltageSolarPV_raw = voltage_factor * analogRead(voltageSolarPV) * (5.0 / 1023.0);
  // Battery
  int currentBattery_raw = (analogRead(currentBattery) * (5.0 / 1023.0) - 2.5) / 0.185;
  float voltageBattery_raw = voltage_factor * analogRead(voltageBattery) * (5.0 / 1023.0);
  // LED
  int currentLED_raw = (analogRead(currentLED) * (5.0 / 1023.0) - 2.5) / 0.185;
  // Temperature and Humidity
  float temperature_raw = dht.readTemperature();
  float humidity_raw = dht.readHumidity();

  // Create a JSON object
  JsonObject root = jsonDoc.to<JsonObject>();
  root["currentSolarPV"] = currentSolarPV_raw;
  root["voltageSolarPV"] = voltageSolarPV_raw;
  root["currentBattery"] = currentBattery_raw;
  root["voltageBattery"] = voltageBattery_raw;
  root["currentLED"] = currentLED_raw;
  root["temperature"] = temperature_raw;
  root["humidity"] = humidity_raw;

  // Serialize JSON data to a String
  String jsonString;
  serializeJson(jsonDoc, jsonString);

  // Print JSON data to serial monitor for debugging (optional)
  Serial.println(jsonString);

  // Send JSON data to ESP8266
  ESP8266.println(jsonString);

  delay(15000); // Read sensor data every 15 seconds
}
