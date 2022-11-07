#include <config.h>

void sendData(float BMP_Temperature, float BMP_Pressure, float DHT_Temperature, float DHT_Humidity)
{
  HTTPClient http;
  String url = "https://script.google.com/macros/s/" + String(GOOGLE_SCRIPT_ID) + "/exec";
  Serial.println("Making POST request at:");
  Serial.println(url);
  http.begin(url, root_ca); // Specify the URL and certificate
  http.addHeader("Content-Type", "application/json");

  StaticJsonDocument<128> doc;
  doc["tag"] = "normal";
  doc["BMP_Temperature"] = BMP_Temperature;
  doc["BMP_Pressure"] = BMP_Pressure;
  doc["DHT_Temperature"] = DHT_Temperature;
  doc["DHT_Humidity"] = DHT_Humidity;

  String jsonOutput;
  serializeJson(doc, jsonOutput);
  Serial.println("Serialized JSON: ");
  Serial.println(jsonOutput);

  uint8_t httpCode = http.POST(String(jsonOutput));
  delay(5000);

  http.end();
  Serial.print("HTTP response code: ");
  Serial.println(httpCode);
}

void connectWiFi()
{
  while (WiFi.status() != WL_CONNECTED)
  { // runs the whole connection procedure again if connection wasn't estabilished within 20 seconds
    Serial.print("Connecting to WiFi");
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    int connectTimer = 0;
    while (WiFi.status() != WL_CONNECTED && connectTimer < 20)
    {
      Serial.print(".");
      delay(1000);
      connectTimer++; // to make sure you aren't just endlessly stuck here when connection doesn't happen for whatever reason
    }
  }

  Serial.println();
  Serial.println("Connected to WiFi");
}

void disconnectWiFi()
{
  WiFi.disconnect();
  delay(500);
  Serial.println("Disconnected from WiFi");
}

void setup()
{
  Serial.begin(115200);

  Serial.println();
  Serial.println("Initializing Sensors");

  Serial.print("Initializing BMP-280");
  delay(500);
  while (!bmp.begin(0x76))
  {                    // 0x76 is the default I2C adress for these sensors
    Serial.print("."); // check wiring or try different adress if you are stuck in loop
    delay(500);
  }
  delay(500);

  bmp.setSampling(Adafruit_BMP280::MODE_FORCED,     // Operating Mode
                  Adafruit_BMP280::SAMPLING_X16,    // Temp. oversampling (X2 is default)
                  Adafruit_BMP280::SAMPLING_X16,    // Pressure oversampling
                  Adafruit_BMP280::FILTER_X16,      // Filtering
                  Adafruit_BMP280::STANDBY_MS_500); // Standby time

  Serial.println();
  Serial.println("BMP-280 succesfully initialized.");

  Serial.println("Initializing DHT-22");
  delay(500);
  dht.begin();
  delay(500);
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  delay(500);
  dht.humidity().getSensor(&sensor);
  delay(500);
  Serial.println("DHT-22 succesfully initialized.");

  Serial.println("All sensors initialized.");
  Serial.println();
}

void loop()
{
  float BMP_Temperature;
  float BMP_Pressure;
  float DHT_Temperature;
  float DHT_Humidity;

  Serial.println("=========================");
  Serial.println("BMP-280 Measurements:");

  if (bmp.takeForcedMeasurement())
  {
    // store measurements in variables
    delay(500);
    BMP_Temperature = bmp.readTemperature();
    BMP_Temperature -= 1.2; // Made In China...
    delay(500);
    BMP_Pressure = bmp.readPressure();
    delay(500);

    Serial.print(F("Temperature = "));
    Serial.print(BMP_Temperature);
    Serial.println(" *C");

    Serial.print(F("Pressure = "));
    Serial.print(BMP_Pressure);
    Serial.println(" Pa");

    Serial.println();
  }
  else
  {
    Serial.println("Forced measurement failed! (BMP280)");
  }

  Serial.println();
  Serial.println("DHT-22 Measurements:");
  // Get temperature event and print its value.
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  delay(500);
  if (isnan(event.temperature))
  {
    Serial.println(F("Error reading temperature! (DHT-22)"));
  }
  else
  {
    DHT_Temperature = event.temperature;

    Serial.print(F("Temperature: "));
    Serial.print(DHT_Temperature);
    Serial.println(F("°C"));
  }
  // Get humidity event and print its value.
  dht.humidity().getEvent(&event);
  delay(500);
  if (isnan(event.relative_humidity))
  {
    Serial.println(F("Error reading humidity! (DHT-22)"));
  }
  else
  {
    DHT_Humidity = event.relative_humidity;
    Serial.print(F("Humidity: "));
    Serial.print(DHT_Humidity);
    Serial.println(F("%"));
  }

  Serial.println();
  connectWiFi();
  sendData(BMP_Temperature, BMP_Pressure, DHT_Temperature, DHT_Humidity);
  disconnectWiFi();
  Serial.println("End of loop, deep-sleeping for 5 minutes");
  Serial.println("=========================");

  esp_sleep_enable_timer_wakeup(5 * 60 * 1000000); // microseconds
  esp_deep_sleep_start();
}