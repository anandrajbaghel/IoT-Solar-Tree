#include <ESP8266WiFi.h>

const char* ssid = "WIFI_SSID";
const char* password = "WIFI_PASSWORD";
String newHostname = "New name for the IoT device (optional)";

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
  // Your other code here
}
