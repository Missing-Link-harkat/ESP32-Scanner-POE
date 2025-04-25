#include "Arduino.h"
#include "ElegantOTAPro.h"
#include "middleware.h"
#include "SPI.h"
#include "ETH.h"

#define USE_TWO_ETH_PORTS 0

#define ETH_PHY_TYPE ETH_PHY_W5500
#define ETH_PHY_ADDR 1
#define CS_PIN       14
#define RST_PIN      9
#define INT_PIN      10

#define MOSI_PIN     11
#define MISO_PIN     12
#define CLK_PIN      13

#define TXD1_PIN     17
#define RXD1_PIN     18

#define GPS_BAUD     9600

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

AsyncWebServer server(80);

HardwareSerial gpsSerial(1);

unsigned long lastTime = 0;

void setup() {
  Serial.begin(9600);
  delay(5000);
  gpsSerial.begin(GPS_BAUD, SERIAL_8N1, RXD1_PIN, TXD1_PIN);
  Serial.println("Starting...");

  SPI.begin(CLK_PIN, MISO_PIN, MOSI_PIN);
  Serial.println("Initializing Ethernet...");
  ETH.begin(ETH_PHY_TYPE, ETH_PHY_ADDR, CS_PIN, INT_PIN, RST_PIN, SPI);
  Serial.println("Ethernet initialized");

  ElegantOTA.setID(Middleware.getDeviceID());
  ElegantOTA.setFWVersion(Middleware.getDeviceVersion());
  ElegantOTA.setDeveloperMode(Middleware.isDeveloperMode());

  ElegantOTA.begin(&server);

  server.begin();
  Serial.println("Starting server...");
  delay(10000);
}

void loop() {
  if (millis() - lastTime > 5000) {
    while (gpsSerial.available() > 0) {
      char gpsData = gpsSerial.read();
      Serial.print(gpsData);
    }
    lastTime = millis();
  }
  ElegantOTA.loop();
}