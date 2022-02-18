#include <Arduino.h>
#include <WiFi.h>
#include <EEPROM.h>
#include <Wire.h>
#include <U8x8lib.h>
#include "graphics.h"
#include "secrets.h"

#define DEBOUNCE_TIME 50

#define BUT1 17 // Right
#define BUT2 12 // Middle
#define BUT3 13 // left


void display_ip();


U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(/* clock=*/ ESPSCL, /* data=*/ ESPSDA, /* reset=*/ RST);

int lastSteadyStateBut1 = LOW;
int lastFlickerableStateBut1 = LOW;
int but1State;

int lastSteadyStateBut2 = LOW;
int lastFlickerableStateBut2 = LOW;
int but2State;

int lastSteadyStateBut3 = LOW;
int lastFlickerableStateBut3 = LOW;
int but3State;

unsigned long lastDebounceTimeBut1 = 0;
unsigned long lastDebounceTimeBut2 = 0;
unsigned long lastDebounceTimeBut3 = 0;



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

  pinMode(BUT1, INPUT_PULLUP);
  pinMode(BUT2, INPUT_PULLUP);
  pinMode(BUT3, INPUT_PULLUP);

  u8x8.begin();
  u8x8.setFont(u8x8_font_chroma48medium8_r);

  scanNetworks();
  delay(5000);
  setupWifi();
}


void loop() {
  but1State = digitalRead(BUT1);
  but2State = digitalRead(BUT2);
  but3State = digitalRead(BUT3);

  // Button1
  if( but1State != lastFlickerableStateBut1) {
    lastDebounceTimeBut1 = millis();
    lastFlickerableStateBut1 = but1State;
  }

  if((millis() - lastDebounceTimeBut1) > DEBOUNCE_TIME) {
    if(lastSteadyStateBut1 == HIGH && but1State == LOW) {
      Serial.println("Button1 Pressed!");
      u8x8.clearDisplay();
      u8x8.drawString(0, 1, "Button 1 Pushed!");
    } else if(lastSteadyStateBut1 == LOW && but1State == HIGH) {
      Serial.println("Button1 is released");
    }
    lastSteadyStateBut1 = but1State;
  }

  // Button2
  if( but2State != lastFlickerableStateBut2) {
    lastDebounceTimeBut2 = millis();
    lastFlickerableStateBut2 = but2State;
  }

  if((millis() - lastDebounceTimeBut2) > DEBOUNCE_TIME) {
    if(lastSteadyStateBut2 == HIGH && but2State == LOW) {
      Serial.println("Button2 Pressed!");
      u8x8.clearDisplay();
      u8x8.drawString(0, 1, "Button 2 Pushed!");
    } else if(lastSteadyStateBut2 == LOW && but2State == HIGH) {
      Serial.println("Button2 is released");
    }
    lastSteadyStateBut2 = but2State;
  }

  // Button3
  if( but3State != lastFlickerableStateBut1) {
    lastDebounceTimeBut3 = millis();
    lastFlickerableStateBut3 = but3State;
  }

  if((millis() - lastDebounceTimeBut3) > DEBOUNCE_TIME) {
    if(lastSteadyStateBut3 == HIGH && but3State == LOW) {
      Serial.println("Button3 Pressed!");
      u8x8.clearDisplay();
      u8x8.drawString(0, 1, "Button 3 Pushed!");
    } else if(lastSteadyStateBut3 == LOW && but3State == HIGH) {
      Serial.println("Button3 is released");
    }
    lastSteadyStateBut3 = but3State;
  }


  if(WiFi.status() != WL_CONNECTED) {
    Serial.println("Wifi Disconnected!");
    u8x8.clearDisplay();
    u8x8.drawString(0, 0, "WiFi Disconnected.");
  }
}
