# VitoWifiApp
 ESP app for the VitoWifi library

WitiWifi (https://github.com/bertmelis/VitoWiFi) is a library to access Viessmann heaters via the optical interface.
This project provides a ESP8266 program to poll the VO1B values and publish them to MQTT
It features: 

- includes a set of data points (DP) for the WO1B Vitotronic control device
- probe high priority DPs every 500 ms and publish them to MQTT
- proble low priority DPs every 5s and also publish them to MQTT
- provide a http access (e.g. http://192.168.1.18/read?DP=0x2000&Type=TempL and ..../write)
- provide a http web page to probe unknown DPs (note: adapt local IP)
- provide ESP I/O access through MQTT (for safety chain control relays)
- OTA and MultiWifi
- HomeAssistant auto discovery (untested)
- can generate a OpenHAB .things file



Future work: beautyfy the webpage and make it adopt the IP as dynamic not static in the code
 

![grafik](https://github.com/s0170071/VitoWifiApp/assets/33855380/33d6f335-ce6f-45c0-b095-62cf4fb4beeb)
