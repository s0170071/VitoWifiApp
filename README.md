# VitoWifiApp
 ESP app for the VitoWifi library

WitiWifi (https://github.com/bertmelis/VitoWiFi) is a library to access Viessmann heaters via the optical interface. 
This project provides a ESP8266 program that features: 

- include a set of data points (DP) for the WO1B Vitotronic control device
- probe high priority DPs every 500 ms and publish them to MQTT
- proble low priority DPs every 5s and also publish them to MQTT
- provide a http access (192.168.1.151/read and ..../write)
- provide a http web page to probe unknown DPs
- provide ESP I/O access through MQTT (for safety chain control relays)
- OTA and MultiWifi
- HomeAssistant auto discovery (untested)
