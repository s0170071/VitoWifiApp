// Includes for OTA
#include <ArduinoOTA.h>
#include <WiFiUdp.h>
#include <ESP8266mDNS.h>         // MDNS makes the ESP available as VitoWifiMqtt.local


#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WiFiMulti.h>    // allows to hanldle multiple APs to connect to
//#define LOG(s) Serial.print(s);
#define LOG
ESP8266WiFiMulti wifiMulti;     // Create an instance of the ESP8266WiFiMulti class, called 'wifiMulti'

void WifiSetup()
{
//    Serial.begin(115200);         // Start the Serial communication to send messages to the computer
  WiFi.mode(WIFI_STA);   //To Turn on WiFi in Specific Mode like WIFI_STA or WIFI_AP of WIFI_AP_STA or WIFI_OFF
  wifiMulti.addAP(WIFI_SSID, WIFI_PASS);   // add Wi-Fi networks you want to connect to
  //wifiMulti.addAP("ssid_from_AP_2", "your_password_for_AP_2");
  //wifiMulti.addAP("ssid_from_AP_3", "your_password_for_AP_3");

  LOG("Connecting ...");
  int i = 0;
  while (wifiMulti.run() != WL_CONNECTED) { // Wait for the Wi-Fi to connect: scan for Wi-Fi networks, and connect to the strongest of the networks above
    delay(250);
    LOG('.');
  }
  LOG('\n');
  LOG("Connected to ");
  LOG(WiFi.SSID());              // Tell us what network we're connected to
  LOG("IP address:\t");
  LOG(WiFi.localIP());           // Send the IP address of the ESP8266 to the computer
}

void OTASetup()
{
  // ArduinoOTA.setPassword((const char *)"whateveryoulikehere");
  ArduinoOTA.setHostname(HOSTNAME);
  ArduinoOTA.begin();
  MDNS.begin(HOSTNAME);               // Start the mDNS responder for VitoWifi.local
}

void ArduinoOTALoop(void) {
  ArduinoOTA.handle(); 
  MDNS.update();  
  }

void WifiMultiLoop(void) {
    wifiMulti.run(); 
}
