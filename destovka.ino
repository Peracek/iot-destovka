#include <Adafruit_GFX.h>
#include <Adafruit_GrayOLED.h>
#include <Adafruit_SPITFT.h>
#include <Adafruit_SPITFT_Macros.h>
#include <gfxfont.h>

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#include "arduino_secrets.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32

#define TRIG_PIN D6
#define ECHO_PIN D5

const char ssid[] = SECRET_SSID;
const char password[] = SECRET_PASS;

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
ESP8266WebServer server(80);

long distance = 0;

// Wi-Fi reconnection configuration
unsigned long lastConnectAttemptTime = 0;
const unsigned long connectAttemptInterval = 5000;

void setup()
{
  Wire.begin(D2, D1);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  WiFi.mode(WIFI_STA);
  // configure mDNS to it's accessible at http://destovka.local
  MDNS.begin("destovka");
  connectToWiFi();

  // Create a route to handle incoming GET requests
  server.on("/get_level", HTTP_GET, []()
            {
    // Send the response with measured distance only
    String response = "Distance: " + String(distance) + " cm";
    server.send(200, "text/plain", response); });

  // Start the server
  server.begin();
}

void loop()
{
  // Clear the trigger
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(5);

  // Set the trigger high for 10 microseconds
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Read the signal from the sensor
  long duration = pulseIn(ECHO_PIN, HIGH, 25000); // 25ms timeout

  // Calculate the distance (using speed of sound = 34300 cm/s)
  distance = (duration / 2.0) * 0.0343;

  displayTextOnOLED();
  connectToWiFi();
  MDNS.update();

  server.handleClient();

  delay(500); // Wait half a second before the next loop
}

void connectToWiFi()
{
  if (WiFi.status() != WL_CONNECTED)
  {
    if (millis() - lastConnectAttemptTime > connectAttemptInterval)
    {
      lastConnectAttemptTime = millis();
      WiFi.begin(ssid, password);
    }
  }
}

void displayTextOnOLED()
{
  String displayText = "Dist: ";
  if (distance > 25 && distance < 300)
  {
    displayText += String(distance) + " cm\n";
  }
  else
  {
    displayText += "Out of range\n";
  }
  displayText += "Wi-Fi: " + wifiStatusString() + "\n";
  displayText += "Signal: " + wifiSignalStrength() + "\n";

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println(displayText);
  display.display();
}

String wifiStatusString()
{
  switch (WiFi.status())
  {
  case WL_IDLE_STATUS:
    return "Idle";
  case WL_CONNECTED:
    return "Connected";
  default:
    return "Connecting...";
  }
}

String wifiSignalStrength()
{
  int strength = WiFi.RSSI();
  if (WiFi.status() != WL_CONNECTED)
  {
    return "N/A";
  }
  if (strength >= -50)
  {
    return "Excellent"; // You can customize the icons here
  }
  else if (strength >= -60)
  {
    return "Good";
  }
  else if (strength >= -70)
  {
    return "Fair";
  }
  else if (strength >= -80)
  {
    return "Weak";
  }
  else if (strength >= -90)
  {
    return "Poor";
  }
  else
  {
    return "No Signal";
  }
}