#include <ESP8266WiFi.h>
#include <ThingSpeak.h>

const char* ssid = "WIFI_SSID";
const char* password = "WIFI_PASSWORD";
String newHostname = "IoT Solar Tree";

const char* apiWriteKey = "API_KEY"; // Replace with your actual API Key
const uint8_t channelId = 9999999; // Replace with your actual Channel ID

WiFiClient client;

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.hostname(newHostname.c_str());
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  // Receive data from Arduino Uno over serial communication
  if (Serial.available() > 0) {
    String dataString = Serial.readStringUntil('\n'); // Read data string sent by Arduino

    // Send data to ThingSpeak
    ThingSpeak.begin(client, apiWriteKey);
    int httpResponseCode = ThingSpeak.write(channelId, dataString);
    if(httpResponseCode == 200) {
      Serial.println("Data sent to ThingSpeak!");
    } else {
      Serial.println("Error sending data: ");
      Serial.println(httpResponseCode);
    }
    ThingSpeak.close();
  }
  delay(2000); // Adjust delay based on your data update frequency
}
