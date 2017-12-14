#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

#define BUTTON_PIN 14

const char* host = "api.dominos.is"; // Hostname for HTTP header
const IPAddress ip(191, 235, 177, 30); // Using hostname for connection doesn't work, using ip does

extern const char* ssid; // SSID for WiFi
extern const char* password; // Password for WiFi
extern const char* token; // API token
extern const char* order_json; // JSON string for order

void wait_until(int state)
{
  Serial.print("Waiting for button to be ");
  Serial.println(state ? "pressed" : "released" );
  while(digitalRead(BUTTON_PIN) == !state)
  {
    delay(100);
  }
  Serial.print("Button ");
  Serial.println(state ? "pressed" : "released" );
}

void setup() {
  Serial.begin(115200);
  Serial.println("PizzaButton");

  // Set pin mode
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  // Connect to WiFi
  delay(100);
  /*WiFi.begin(ssid, password);
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
  Serial.println(WiFi.localIP());*/

  // Don't leave setup until button is released
  wait_until(LOW);
  Serial.println("Setup done");
}

void loop() {
  wait_until(HIGH);
  wait_until(LOW);
}
