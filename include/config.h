#include <Arduino.h>
#include <ArduinoJson.h>

// BMP280 libs
#include <Adafruit_BMP280.h>

// DHT22 libs
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

// BMP280
#define BMP_SCK (13)
#define BMP_MISO (12)
#define BMP_MOSI (11)
#define BMP_CS (10)
Adafruit_BMP280 bmp; // I2C

// DHT22
#define DHTPIN 14     // GPIO pin 14
#define DHTTYPE DHT22 // Type of sensor - DHT 22 (AM2302)
DHT_Unified dht(DHTPIN, DHTTYPE);
uint32_t delayMS;

// WiFi
#include <WiFi.h>
#include <HTTPClient.h>
#include <secrets.h> // ssid, password, GSCRIPT ID, certificate
WiFiClientSecure client;
