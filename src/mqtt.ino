#include <PubSubClient.h>
#include <ESP8266WiFi.h>

WiFiClient espClient;
PubSubClient MqttClient(espClient);

void MqttSetup(String server){
  MqttClient.setServer(mqtt_server,  1883);
  MqttClient.setCallback(MQTTcallback);
  MqttClient.setBufferSize(1024);
  reconnect() ;  // and subscribe
  
}



void reconnect() {
  static unsigned long MQTTtimeout = 0; 
  MqttClient.disconnect();
  if (millis() <  (MQTTtimeout + 1000UL)) return;
    MQTTtimeout = millis();  
    if (MqttClient.connect(HOSTNAME)) {
        MqttClient.subscribe(getSubscribeTopic().c_str());
        CheckAndPublish("log", "mqtt reconnected: " );
    }  
}


void CheckAndPublish(String topic, String message){
  unsigned int timeout =0;
  while (1){
      MqttClient.loop();
      delay(1);
      if (!MqttClient.connected())  reconnect();
      if ( MqttClient.publish(topic.c_str(), (uint8_t*)message.c_str(),message.length(),true)) {  return; }
      if (timeout++ == 60000) ESP.reset();
   }
}


void MqttLoop(void) {
  MqttClient.loop();
  if (!MqttClient.connected()) { reconnect();  }  
}




void MQTTcallback(char* topic, byte* payload, unsigned int length) {
//Serial.println("callback");
  // Convert the payload to a string
    String message = "";
    for (int i = 0; i < length; i++) {
      message += (char)payload[i];
    }

    // example topic: homeassistant/sensor/Vitotronic/Vito_mittl_Ausgangstemp_Luft/command and the address is Vito_mittl_Ausgangstemp_Luft
    String topic_str = String(topic);
    unsigned int start_index = topic_str.indexOf("Vitotronic/", 1) + strlen("Vitotronic/");
    unsigned int end_index = topic_str.indexOf("/",start_index); 
    String DPname = topic_str.substring(start_index, end_index);
    unsigned int index = getIndexFromName(DPname);
      while ( ! readyToSendToVito() ){
      delay(50); 
    }
    mqtt_write(index, message);  
   // CheckAndPublish("log", "callback done"); // debug message to the log topic.
} 
