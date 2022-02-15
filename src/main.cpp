#include <Arduino.h>
#include <WiFi.h>
#include <EEPROM.h>
#include <Wire.h>
#include <U8x8lib.h>
#include "graphics.h"
#include "secrets.h"

#define BUT 17 // One button to rule them all.

void display_ip();


U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(/* clock=*/ ESPSCL, /* data=*/ ESPSDA, /* reset=*/ RST);


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
  Serial.print("\nWiFi Connected with IP: "); Serial.println(WiFi.localIP());
  display_ip();
}

void display_ip() {
  u8x8.clearDisplay();
  u8x8.drawString(0, 1, "IP: ");
  u8x8.setCursor(0, 2);
  u8x8.print(WiFi.localIP().toString());
}

static void scanNetworks() {
  int n = WiFi.scanNetworks();

  if (n == 0) {
    Serial.println("Searching networks.");
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
}


void setup() {
  
  Serial.begin(115200);
  EEPROM.begin(512); // 512 bytes out of 4096Bytes (4KB)

  pinMode(BUT, INPUT_PULLUP);

  u8x8.begin();
  u8x8.setFont(u8x8_font_chroma48medium8_r);

  scanNetworks();
  delay(5000);
  setupWifi();
}

int lastState = HIGH;
int currentState;

void loop() {
  currentState = digitalRead(BUT);

  if(lastState == LOW && currentState == HIGH) {
    Serial.println("One Button To Rule them ALL!!!!!\n");
    u8x8.clearDisplay();
    u8x8.drawString(0, 1, "Button Pushed!");
  }
  lastState = currentState;

  if(WiFi.status() != WL_CONNECTED) {
    Serial.println("Wifi Disconnected!");
    u8x8.clearDisplay();
    u8x8.drawString(0, 0, "WiFi Disconnected.");
  }
}
