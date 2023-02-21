# weather-station-V1

## About this repository

Weather monitoring station that uses BMP-280 to read temperature + pressure and DHT-22 to read temperature + relative humidity.

Sensors are connected to NodeMcu ESP-32 (see `wiring.jpg`).

The ESP reads data from both sensors once every 5 minutes, connects to WiFi and sends POST request to my [backend API](https://github.com/Bladesheng/weather-station-backend) with the collected data.
After that, ESP goes into deep sleep and wakes up after 5 minutes to do everything again.

The first version used [google sheets](https://docs.google.com/spreadsheets/d/1_YL1ZynR3oQEMSgbeh6L2CAQVNAri5ZykgG3BgKB_hA) as databasse and to display graphs before i learned how to do proper backend server - see `app.js` (I know...)
