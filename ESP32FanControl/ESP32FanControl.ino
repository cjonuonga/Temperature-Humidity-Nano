#include <WiFi.h>
#include <HTTPClient.h>
#include <DHT.h>
#include <LiquidCrystal.h>
#include "secrets.h"   // copy secrets.example.h to secrets.h and fill it in
#define Type DHT11

// Fan turns ON at or above FAN_ON_TEMP and back OFF at or below FAN_OFF_TEMP.
// The gap stops the fan from flicking on/off when the temp hovers near one value.
const float FAN_ON_TEMP  = 78.0;  // °F
const float FAN_OFF_TEMP = 75.0;  // °F

int sensePin = 4;
DHT HT(sensePin, Type);
int rs = 13;
int en = 14;
int d4 = 25;
int d5 = 26;
int d6 = 27;
int d7 = 33;
float humidity;
float tempF;
int wtime = 500;
int dt = 2000;           // DHT11 needs at least 1s between reads
bool fanOn = false;      // what we want the fan to be
bool plugSynced = false; // false until the plug confirms the last command
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void connectWiFi() {
  if (WiFi.status() == WL_CONNECTED) return;
  Serial.print("Connecting to Wi-Fi");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  for (int i = 0; i < 20 && WiFi.status() != WL_CONNECTED; i++) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(WiFi.status() == WL_CONNECTED ? " connected" : " failed, will retry");
}

// Sends the on/off command to the smart plug. Returns true if the plug answered OK.
bool setPlug(bool on) {
  if (WiFi.status() != WL_CONNECTED) return false;
  HTTPClient http;
  http.setTimeout(3000);
  http.begin(on ? PLUG_ON_URL : PLUG_OFF_URL);
  int code = http.GET();
  http.end();
  Serial.printf("Plug %s -> HTTP %d\n", on ? "ON" : "OFF", code);
  return code == 200;
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  lcd.begin(16,2);
  HT.begin();
  delay(wtime);
  WiFi.mode(WIFI_STA);
  connectWiFi();
}

void loop() {
  // put your main code here, to run repeatedly:
  connectWiFi();

  humidity = HT.readHumidity();
  tempF = HT.readTemperature(true);

  if (isnan(humidity) || isnan(tempF)) {
    // Bad read: show it, but leave the fan alone until we get a good one
    Serial.println("Failed to read from DHT11");
    lcd.setCursor(0,0);
    lcd.print("Sensor error    ");
  } else {
    if (!fanOn && tempF >= FAN_ON_TEMP) {
      fanOn = true;
      plugSynced = false;
    } else if (fanOn && tempF <= FAN_OFF_TEMP) {
      fanOn = false;
      plugSynced = false;
    }

    // Keeps retrying each loop until the plug actually accepts the command
    if (!plugSynced) {
      plugSynced = setPlug(fanOn);
    }

    char line[17];
    snprintf(line, sizeof(line), "Temp: %.1f F     ", tempF);
    lcd.setCursor(0,0);
    lcd.print(line);
    snprintf(line, sizeof(line), "Hum:%2.0f%% Fan:%-3s ", humidity, fanOn ? "ON" : "OFF");
    lcd.setCursor(0,1);
    lcd.print(line);

    Serial.printf("Temp: %.1f F  Humidity: %.0f%%  Fan: %s%s\n",
                  tempF, humidity, fanOn ? "ON" : "OFF", plugSynced ? "" : " (plug not reached)");
  }

  delay(dt);
}
