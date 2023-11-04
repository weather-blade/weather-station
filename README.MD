# weather-station-V1

## About this repository

Weather monitoring station that uses BMP-280 to read temperature + pressure and DHT-22 to read temperature + relative humidity.

Sensors are connected to NodeMcu ESP-32 - see [`wiring.jpg`](https://github.com/Bladesheng/weather-station-V1/blob/main/wiring.jpg)

The ESP reads data from both sensors once every 5 minutes, connects to WiFi and sends POST request to my [backend API](https://github.com/Bladesheng/weather-station-backend) with the collected data.
After that, ESP goes into deep sleep and wakes up after 5 minutes to repeat everything again.

The collected data is then displayed on my frontend [web app](https://github.com/Bladesheng/weather-station-frontend)
