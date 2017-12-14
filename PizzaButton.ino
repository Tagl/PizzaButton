#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

#define BUTTON_PIN 14

const char* host = "api.dominos.is"; // Hostname for HTTP header
const char* url = "/api/Orders";
const IPAddress ip(191, 235, 177, 30); // Using hostname for connection doesn't work, using ip does
const int port = 443; // HTTPS port

extern const char* ssid; // SSID for WiFi
extern const char* password; // Password for WiFi
extern const char* token; // API token
extern const char* order_json; // JSON string for order

String request;
WiFiClientSecure conn; // Client for HTTPS connection

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

void write_order()
{
  Serial.println("Sending order");

  // Send headers
  Serial.println("Sending headers");
  conn.println(request);
  Serial.println("Headers sent");
  delay(100);
  
  // Send payload
  Serial.println("Sending payload");
  conn.println(order_json);
  conn.println();
  Serial.println("Payload sent");
}

void read_response()
{
  Serial.println("Reading response");
  while(conn.connected())
  {
    while(conn.available())
    {
      delay(2);
      Serial.write(conn.read());
    }
    delay(2);
  }
  Serial.println();
  Serial.println("Response read");
  delay(1000);
  Serial.println("Closing connection");
  conn.stop();
  Serial.println("Connection closed");
  delay(100);
}

void order_pizza()
{
  Serial.print("Connecting to: ");
  Serial.println(host);
  
  // Connect to API
  while(!conn.connect(ip, port))
  {
    Serial.println("Connection failed, trying again");
    delay(500);
  }
  Serial.println("Connected");

  // Write the order to the stream
  delay(500);
  write_order();
  read_response();
}

void setup() {
  Serial.begin(115200);
  Serial.println("PizzaButton");

  // prepare request string
  request = "POST " + String(url) + " HTTP/1.1\n"\
            "Host: " + String(host) + "\n"\
            "Authorization: bearer " + String(token) +  "\n"\
            "Content-Type: application/json\n"\
            "Content-Length: " +  String(strlen(order_json)) + "\n";

  // Set pin mode
  pinMode(BUTTON_PIN, INPUT_PULLUP);

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

  // Don't leave setup until button is released
  wait_until(LOW);
  Serial.println("Setup done");
}

void loop() {
  // wait for button press
  wait_until(HIGH);
  order_pizza();
  Serial.println("Pizza ordered");
  // wait for button release
  wait_until(LOW);
  // button cannot be pressed after release for 1 second
  delay(1000);
}
