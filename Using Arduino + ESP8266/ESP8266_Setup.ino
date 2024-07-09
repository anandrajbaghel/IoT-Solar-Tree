#include <ESP8266WiFi.h>
#include "ThingSpeak.h"
#include <ArduinoJson.h>

const char* ssid = "Wifi__00";
const char* password = "cctv4020";
String newHostname = "IoT Solar Tree";
WiFiClient  client;

unsigned long myChannelNumber = 0000000;
const char * myWriteAPIKey = "JKKDSFKKJ786";

StaticJsonDocument<512> jsonDoc; // Allocate memory for JSON data (adjust capacity as needed)

int currentSolarPV;
float voltageSolarPV;
int currentBattery;
float voltageBattery;
int currentLED;
float temperature;
float humidity;
String myStatus = "";

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect
  }
  
  WiFi.hostname(newHostname.c_str());
  WiFi.begin(ssid, password);
  ThingSpeak.begin(client);  // Initializing ThingSpeak
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

  // Check for available data from Arduino
  if (Serial.available() > 0) {
    // Read JSON data string from Arduino
    String jsonString = Serial.readStringUntil('\n');
    jsonString.trim();

    // Parse JSON data
    deserializeJson(jsonDoc, jsonString);

    // Access sensor values from the JSON object
    JsonObject root = jsonDoc.as<JsonObject>();
    currentSolarPV = root["currentSolarPV"];
    voltageSolarPV = root["voltageSolarPV"];
    currentBattery = root["currentBattery"];
    voltageBattery = root["voltageBattery"];
    currentLED = root["currentLED"];
    temperature = root["temperature"];
    humidity = root["humidity"];

    ThingSpeak.setField(1, currentSolarPV);
    ThingSpeak.setField(2, voltageSolarPV);
    ThingSpeak.setField(3, currentBattery);
    ThingSpeak.setField(4, voltageBattery);
    ThingSpeak.setField(5, currentLED);
    ThingSpeak.setField(6, temperature);
    ThingSpeak.setField(7, humidity);

    myStatus = String("Data Updated");
  } else
    myStatus = String("Error - ;(");
      
  
  // set the status
  ThingSpeak.setStatus(myStatus);

  Serial.println(currentSolarPV);
  Serial.println(voltageSolarPV);
  Serial.println(currentBattery);
  Serial.println(voltageBattery);
  Serial.println(currentLED);
  Serial.println(temperature);
  Serial.println(humidity);
  
  // write to the ThingSpeak channel
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if(x == 200){
    Serial.println("Channel update successful.");
  }
  else{
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }

  delay(15000); // Check for data every 15 seconds
  
}
