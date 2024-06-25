#include <SoftwareSerial.h>
#include <stdlib.h>

// Define sensor pins
const int currentSolarPV = A0;
const int voltageSolarPV = A1;
const int currentBattery = A2;
const int voltageBattery = A3;
const int currentLED = A4;
const int temperatureSensor = A5;

// Define ESP8266 communication pins
const int espRx = 2;
const int espTx = 3;

// Define variables
SoftwareSerial ESP8266(espRx, espTx);
String dataString;

void setup() {
  Serial.begin(115200);
  ESP8266.begin(115200);

  // Configuring sensor pins as inputs

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
  float voltageSolarPV_raw = analogRead(voltageSolarPV) * (5.0 / 1023.0); // Convert to voltage (adjust based on sensor range)
  int currentBattery_raw = analogRead(currentBattery);
  float voltageBattery_raw = analogRead(voltageBattery) * (5.0 / 1023.0); // Convert to voltage (adjust based on sensor range)
  int currentLED_raw = analogRead(currentLED);
  float temperature_raw = analogRead(temperatureSensor) * (5.0 / 1023.0) * (100.0 / 5.0); // Convert to temperature (adjust based on sensor range)

  // Prepare data string (e.g., JSON format)
  dataString = "{";
  dataString += "\"currentSolarPV\":" + String(currentSolarPV_raw, 2); // Display voltage with 2 decimal places
  dataString += ",\"voltageSolarPV\":" + String(voltageSolarPV_raw, 2);
  dataString += ",\"currentBattery\":" + String(currentBattery_raw, 2);
  dataString += ",\"voltageBattery\":" + String(voltageBattery_raw, 2);
  dataString += ",\"currentLED\":" + String(currentLED_raw, 2);
  dataString += ",\"temperatureSensor\":" + String(temperature_raw, 2);
  dataString += "}";

  Serial.println(dataString); // Send data string to ESP8266 over serial
  delay(2000); // Read sensors and transmit data every alternate second
}
