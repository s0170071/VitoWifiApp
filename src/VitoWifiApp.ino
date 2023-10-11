#include "DPtypedef.h"
// wifi stup: 
#define WIFI_SSID "yourssid"
#define WIFI_PASS "yourpassowrd"
// MQTT setup
#define mqtt_server "192.168.1.30"
#define HOSTNAME "VitoWifiMqtt151"
//#define mqtt_user "mqtt"
//#define mqtt_password "mqtt"
 



/*
 for advertising the data points to be available for HomeAssistand autodiscovery
 This is only partially tested as I abandoned Homeassistant in favor of OpenHab.
 
 https://www.home-assistant.io/integrations/mqtt/#mqtt-discovery
 This is an example of what it looks like:
{
  "name": "Washing Machine Temperature 2",
  "unique_id": "washingmachine_temperature_2",
  "device": {
    "identifiers": "washingmachine",
    "name": "Washing Machine",
    "sw_version": "1.0",
    "model": "Model XYZ",
    "manufacturer": "Manufacturer ABC"
  },
  "state_topic": "mytopic",
  "value_template": "{{ value_json.temperature_2 }}",
  "json_attributes_topic": "mytopic",
  "unit_of_measurement": "°C",
  "device_class": "temperature",
    "command_topic": "mytopic/set",
  "read_only": true
}

Explanation of the fields:

    name: A friendly name for the sensor.
    unique_id: A unique identifier for the sensor.
    device: Information about the device that the sensor belongs to.
    state_topic: The MQTT topic that the sensor posts its value to.
    value_template: A template for extracting the sensor value from the JSON payload of the MQTT message.
    json_attributes_topic: The MQTT topic where the additional attributes for the sensor can be found.
    unit_of_measurement: The unit of measurement for the sensor value.
    device_class: The class of the device (in this case, temperature).
    read_only: Whether the sensor is read-only or not.
*/
// Name ,    Adress, Type ,  Factor, Read/Write , SensorType, UoM



void setup() {
  //generateThingsFile(); for (;;) delay(1);  // uncomment to dump the things file for OpenHab to the console.
  pinMode(4,OUTPUT); 
  digitalWrite(4,1); 
   
  WaitForVitoStarted(20000); // required for the heater to get past its bootloading phase
  WifiSetup();
  VitoWifiSetup();
  OTASetup();  
  WebserverSetup();
  MqttSetup(mqtt_server);
  advertiseDPs();       // push the topics Homeassistant autodiscovery 
  CheckAndPublish("log", "setup done"); // push a status message to MQTT for logging.
}

 
void loop() {
  ArduinoOTALoop();
  WifiMultiLoop();
  WebserverLoop();
  MqttLoop();
  VitoWifiLoop();
}
 
