#include <config.h>

void sendData(String params) {
  HTTPClient http;
  String url="https://script.google.com/macros/s/" + String(GOOGLE_SCRIPT_ID) + "/exec?"+params;
  Serial.println("Making GET request at:");
  Serial.println(url);
  http.begin(url, root_ca); //Specify the URL and certificate
  int httpCode = http.GET();
  http.end();
  Serial.println("Done: " + String(httpCode));
}

void connectWiFi() {
  Serial.print("Connecting to WiFi");	
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("Connected to WiFi");
}

void disconnectWiFi() {
  WiFi.disconnect();
  Serial.println("Disconnected from WiFi");
}


void setup() {
  Serial.begin(115200);

  Serial.println();
  Serial.println("Initializing Sensors");


  Serial.println("Initializing BMP-280");
  if (!bmp.begin(0x76)) { //0x76 is the default I2C adress for these sensors
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring or try a different address!"));
    while (1) delay(10);
  }
  // Default settings from datasheet
  bmp.setSampling(Adafruit_BMP280::MODE_FORCED,     // Operating Mode
                  Adafruit_BMP280::SAMPLING_X2,     // Temp. oversampling
                  Adafruit_BMP280::SAMPLING_X16,    // Pressure oversampling
                  Adafruit_BMP280::FILTER_X16,      // Filtering
                  Adafruit_BMP280::STANDBY_MS_500); // Standby time

  Serial.println("BMP-280 succesfully initialized.");


  Serial.println("Initializing DHT-22");
  dht.begin();
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  dht.humidity().getSensor(&sensor);
  Serial.println("DHT-22 succesfully initialized.");


  Serial.println("All sensors initialized.");
  Serial.println();
}

void loop() {
  float BMP_Temperature;
  float BMP_Pressure;
  float DHT_Temperature;
  float DHT_Humidity;

  Serial.println("=========================");
  Serial.println("BMP-280 Measurements:");
  // must call this to wake sensor up and get new measurement data
  // it blocks until measurement is complete
  if (bmp.takeForcedMeasurement()) {
    // can now print out the new measurements

    // store measurements in variables
    BMP_Temperature = bmp.readTemperature();
    BMP_Pressure = bmp.readPressure();

    Serial.print(F("Temperature = "));
    Serial.print(BMP_Temperature);
    Serial.println(" *C");

    Serial.print(F("Pressure = "));
    Serial.print(BMP_Pressure);
    Serial.println(" Pa");

    Serial.println();
  } else {
    Serial.println("Forced measurement failed! (BMP280)");
  }


  Serial.println();
  Serial.println("DHT-22 Measurements:");
  // Get temperature event and print its value.
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    Serial.println(F("Error reading temperature! (DHT-22)"));
  }
  else {
    DHT_Temperature = event.temperature;

    Serial.print(F("Temperature: "));
    Serial.print(DHT_Temperature);
    Serial.println(F("°C"));
  }
  // Get humidity event and print its value.
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    Serial.println(F("Error reading humidity! (DHT-22)"));
  }
  else {
    DHT_Humidity = event.relative_humidity;

    Serial.print(F("Humidity: "));
    Serial.print(DHT_Humidity);
    Serial.println(F("%"));
  }


  String dataPayload = String(BMP_Temperature) + ";" + String(BMP_Pressure) + ";" + String(DHT_Temperature) + ";" + String(DHT_Humidity);

  Serial.println();
  connectWiFi();
  sendData("tag=adc_A0&value="+dataPayload);
  disconnectWiFi();
  Serial.println("End of loop, deep-sleeping for 5 minutes");
  Serial.println("=========================");

  esp_sleep_enable_timer_wakeup(5 * 60 * 1000000); // microseconds
  esp_deep_sleep_start();
}