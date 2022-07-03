# weather-station-V1

Simple "weather" monitoring station.

Uses BMP-280 to log temperature + pressure and DHT-22 to log temperature + relative humidity.

Sensors are connected to NodeMcu ESP-32 (wiring.jpg).

The ESP logs data once every 5 minutes, connects to WiFi and sends GET request to google scripts app with the collected data.
After that, ESP goes into deep sleep and wakes up after 5 minutes to repeat the process.

The google scripts app handles the GET request and saves the received data to google sheets.

The collected data and relevant graphs are here:
https://docs.google.com/spreadsheets/d/1_YL1ZynR3oQEMSgbeh6L2CAQVNAri5ZykgG3BgKB_hA
