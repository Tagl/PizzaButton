#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

const char* host = "api.dominos.is"; // Hostname for HTTP header
const IPAddress ip(191, 235, 177, 30); // Using hostname for connection doesn't work, using ip does

extern const char* ssid; // SSID for WiFi
extern const char* password; // Password for WiFi
extern const char* token; // API token
extern const char* order_json; // JSON string for order

void setup() {
  Serial.begin(115200);
  Serial.println("PizzaButton");

  // Connect to WiFi
  delay(100);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to SSID: ");
  Serial.println(ssid);
  while(WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.println("Connected");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  // put your main code here, to run repeatedly:

}
