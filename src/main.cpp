#include <Arduino.h>
#include <WiFi.h>
#include <EEPROM.h>
#include <Wire.h>
#include <U8x8lib.h>
#include <Adafruit_I2CDevice.h>


const char WIFI_SSID[] = "Thor";
const char WIFI_PASSWORD[] = "drink&fight";

U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(/* clock=*/ 15, /* data=*/ 4, /* reset=*/ 16);


void setupWifi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.print("Connecting to WiFi: "); Serial.println(WIFI_SSID);

  int counter = 0;
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    counter++;
    Serial.print(".");
    if(counter >= 8) {
      Serial.println();
      counter = 0;
    }
  }
  Serial.print("\nWIFI Connected with IP:"); Serial.println(WiFi.localIP());
  u8x8.clearDisplay();
  u8x8.drawString(0, 1, "IP: ");
  u8x8.setCursor(0, 2);
  u8x8.print(WiFi.localIP().toString());
}


static void scanNetworks() {
  int n = WiFi.scanNetworks();

  if (n == 0) {
    u8x8.drawString(0, 0, "Searching networks.");
  } else {
    u8x8.drawString(0, 0, "Networks found: ");
    for (int i = 0; i < n; ++i) {
      // Print SSID for each network found
      char currentSSID[64];
      WiFi.SSID(i).toCharArray(currentSSID, 64);
      u8x8.drawString(0, i + 1, currentSSID);
    }
  }

  // Wait a bit before scanning again
  delay(5000);
}

void setup() {
  Serial.begin(115200);
  EEPROM.begin(512); // 512 bytes out of 4096Bytes (4KB)

  u8x8.begin();
  u8x8.setFont(u8x8_font_chroma48medium8_r);

  scanNetworks();
  setupWifi();
}

void loop() {
  if(WiFi.status() != WL_CONNECTED) {
    Serial.println("Wifi Disconnected!");
    u8x8.clearDisplay();
    u8x8.drawString(0, 0, "WiFi Disconnected.");
  }
}