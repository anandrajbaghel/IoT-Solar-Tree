#include <ESP8266WiFi.h>
#include "ThingSpeak.h"
#include <DHT.h>

#define DHTPIN D5 // Pin where the DHT sensor is connected
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// Defining sensor pins
const int currentSolarPV = D0;
const int voltageSolarPV = D1;
const int currentBattery = D2;
const int voltageBattery = D3;
const int currentLED = D4;

const char* ssid = "Wifi__00";
const char* password = "cctv4020";
String newHostname = "IoT Solar Tree";
WiFiClient  client;

unsigned long myChannelNumber = 1234567; // Channel Number
const char * myWriteAPIKey = "JKDBRF89675497"; // API Key

int currentSolarPV_raw;
float voltageSolarPV_raw;
int currentBattery_raw;
float voltageBattery_raw;
int currentLED_raw;
float temperature_raw;
float humidity_raw;
String myStatus = "";

void setup() {
  Serial.begin(115200); 
  WiFi.hostname(newHostname.c_str());
  WiFi.begin(ssid, password);
  dht.begin(); // Initialize DHT sensor
  ThingSpeak.begin(client);  // Initializing ThingSpeak

  pinMode(currentSolarPV, INPUT);
  pinMode(voltageSolarPV, INPUT);
  pinMode(currentBattery, INPUT);
  pinMode(voltageBattery, INPUT);
  pinMode(currentLED, INPUT);
}

void loop() {

  // Connect or reconnect to WiFi
  if(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
    while(WiFi.status() != WL_CONNECTED){
      Serial.print(".");
      delay(5000);     
    } 
    Serial.println("\nWiFi connected!");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  }

  // Reading sensor values
  int currentSolarPV_raw = analogRead(currentSolarPV);
  float voltageSolarPV_raw = analogRead(voltageSolarPV) * (5.0 / 1023.0);
  int currentBattery_raw = analogRead(currentBattery);
  float voltageBattery_raw = analogRead(voltageBattery) * (5.0 / 1023.0);
  int currentLED_raw = analogRead(currentLED);
  float temperature_raw = dht.readTemperature();
  float humidity_raw = dht.readHumidity();

  ThingSpeak.setField(1, currentSolarPV_raw);
  ThingSpeak.setField(2, voltageSolarPV_raw);
  ThingSpeak.setField(3, currentBattery_raw);
  ThingSpeak.setField(4, voltageBattery_raw);
  ThingSpeak.setField(5, currentLED_raw);
  ThingSpeak.setField(6, temperature_raw);
  ThingSpeak.setField(7, humidity_raw);      

  Serial.println(currentSolarPV_raw);
  Serial.println(voltageSolarPV_raw);
  Serial.println(currentBattery_raw);
  Serial.println(voltageBattery_raw);
  Serial.println(currentLED_raw);
  Serial.println(temperature_raw);
  Serial.println(humidity_raw);
  
  // write to the ThingSpeak channel
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if(x == 200){
    Serial.println("Channel update successful.");
    myStatus = String("Data Updated");
  }
  else{
    Serial.println("Problem updating channel. HTTP error code " + String(x));
    myStatus = String("Error! updating");
  }

  // set the status
  ThingSpeak.setStatus(myStatus);

  delay(15000); // Wait for 15 second before reading again
}
