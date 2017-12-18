#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

#define BUTTON_PIN 14

Adafruit_SSD1306 display = Adafruit_SSD1306();

#if (SSD1306_LCDHEIGHT != 32)
 #error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif


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
  Serial.println(state ? "pressed" : "released");
  while(digitalRead(BUTTON_PIN) == !state)
  {
    delay(100);
  }
  Serial.print("Button ");
  Serial.println(state ? "pressed" : "released");
}

void write_order()
{
  Serial.println("Sending order");
  display.println("Ordering pizza");
  display.display();
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
  display.println("Ordered");
  display.display();
}

void read_response()
{
  bool reading_started = false;
  Serial.println("Reading response");
  while(conn.connected())
  {
    while(conn.available())
    {
      reading_started = true;
      delay(2);
      Serial.write(conn.read());
    }
    delay(2);
    if(reading_started) break;
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
  display.clearDisplay();
  display.setCursor(0,0);
  display.setTextSize(1);
  display.println("Connecting to API");
  display.display();
  // Connect to API
  if(!conn.connect(ip, port))
  {
    Serial.println("Connection failed, trying again");
    display.println("Error, please try again");
    return;
  }
  Serial.println("Connected");
  display.println("Connected");
  display.display();
  // Write the order to the stream
  delay(500);
  write_order();
  read_response();
}

void setup() {
  Serial.begin(115200);
  Serial.println("PizzaButton");

  // initialize display and print initial message
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)
  display.clearDisplay();
  display.setCursor(0,0);
  display.setTextColor(WHITE);
  display.setTextSize(3);
  display.println("Dominos");
  display.display();
  delay(1000);
  
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

  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0,0);
  display.print("Connecting to ");
  display.println(ssid);
  display.display();
  
  while(WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.println("Connected");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
  display.println("Connected");
  display.display();

  // Don't leave setup until button is released
  wait_until(LOW);
  Serial.println("Setup done");
}

void loop() {
  display.clearDisplay();
  display.setTextSize(3);
  display.setCursor(0,0);
  display.print("Ready!");
  display.display();
  
  // wait for button press
  wait_until(HIGH);
  order_pizza();
  Serial.println("Pizza ordered");
  // wait for button release
  wait_until(LOW);
  // button cannot be pressed after release for 1 second
  delay(1000);
}
