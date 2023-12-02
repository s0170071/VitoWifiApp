 
// note for Arduino IDE: this file is automatically included into the VitoMqtt.ino as if they were one file.
/*
 * There are two priorities for the data point polling. Those values that indicate a status, e.g. compressor, are polled more often.
 * Those values that change only slowly are polled less often. In general, polling should not be faster than every 500 ms because otherwise the heater may become inresponsive. 
 * The mechanics is this: a low priority DP is polled every 5s. If that time is elapesd, the next DP with that DP will be fetched from the list. If it is at the end, it will start again (round robin) 
 * If no low priority DP is 'ready' because the 5s have not elapsed yet, a DP from the high priority list is polled every 500ms. 
 * 
 */


#include "DPtypedef.h"
#include "datapoints.h"
#define TIMEOUT_PROBE_VITO 500 // poll from the high prio list not faster than every  500ms 
#define TIMEOUT_DP_LOW TIMEOUT_PROBE_VITO *10 // poss from the low prio list not faster than 10*500ms (5s)
#define NUMBER_OF_DP sizeof(DPoints)/sizeof(vitoDPstruct)
/*
   typedef struct {  unsigned char name[48];
                      uint16_t  address; 
                      EnumDataType dataFormatType; // CountS, CountL...
                      float factor; 
                      EnumReadWrite ReadWrite;
                      EnumSensorType sensorType; // tempeature, setpoint, state,
                      unsigned char UoM[10]; 
                      unsigned char sensorTypeTopic[15];
                      EnumPriorityType priority;
                      
                      
    } vitoDPstruct; 
*/


/*
 * class definition for the DPs. Like a struct...
 */
class VitoDataPoints {
  private:
    //extern const vitoDPstruct DPoints; 
  public: 
    String            getName           (unsigned int index)      {return String((char*)DPoints[index].name);   }; 
    String            getAddressString  (unsigned int index)      {return String(DPoints[index].address);       }; 
    unsigned int      getAddress        (unsigned int index)      {return  DPoints[index].address;              };
    float             getFactor         (unsigned int index)      {return  DPoints[index].factor;               };
    unsigned char     getDataTypeBytes  (unsigned int index)      {return getDataTypeBytes(DPoints[index].dataFormatType);}; 
    unsigned char     getDataTypeBytes  (String type); 
    unsigned char     getDataTypeBytes  (EnumDataType type); 
    String            getDataTypeString (unsigned int index) ; 
    String            getSensorTypeString  (unsigned int index) ;
    bool              isWritable        (unsigned int index)      {return (DPoints[index].ReadWrite != RO);     };

    String            getUoM            (unsigned int index)      {return String((char*)DPoints[index].UoM);    }; 
    String            getTypeTopic      (unsigned int index)      {return String((char*)DPoints[index].sensorTypeTopic);   }; 
    EnumPriorityType  getPriority       (unsigned int index)      {return DPoints[index].priority;               }; 
    unsigned int      getIndexFromName  (String name); 
};

/*
 * Some functions to access the fields of the DP structs.
 */

byte VitoDataPoints::getDataTypeBytes (EnumDataType type){  
  byte readLen=4;   // for  CountL, Hours and Raw
      if      ( (type == TempL) || (type == CountS))       readLen = 2;
      else if ( (type == STAT ) || (type == TempS)  || (type == Mode) || (type == CoP) ) readLen = 1;
  return readLen;
}
byte VitoDataPoints::getDataTypeBytes (String type) {
  byte readLen=4;   // for  CountL, Hours and Raw
      if      ( (type == "TempL") || (type == "CountS"))       readLen = 2;
      else if ( (type == "STAT" ) || (type == "TempS")  || (type == "Mode") || (type == "CoP") ) readLen = 1;
  return readLen;
}
 unsigned int VitoDataPoints::getIndexFromName(String name){
   for (int i = 0; i< NUMBER_OF_DP; i++ ) { 
     if (  "Vito_"+String((char*)DPoints[i].name) ==  name)  { return i; }
   }
  return (0);
}
  
/*
 * Just convert from Enum to String but using the index of the DP
 */
String VitoDataPoints::getSensorTypeString (unsigned int index)  {
    EnumSensorType sensorType = DPoints[index].sensorType;
    switch( sensorType) {
      case temperature:  return("temperature");     break;
      case state:        return("state");    break;
      case setpoint:     return("setpoint"); break;
      case power:        return("power");    break;
      case None:         return("None");  break;
      case Time:         return("time");    break;
      case energy:       return("energy");   break;
      case pressure:     return("pressure"); break;
      }
      return "";
}

/*
 * convert the Enum data type of the sensor type to a string by index
 */
String VitoDataPoints::getDataTypeString( unsigned int index){
 EnumDataType type = DPoints[index].dataFormatType;
     switch(type) {
       case CountS:  return("CountS");   break;
       case CountL:  return("CountL");   break;
       case TempS:   return("TempS");    break;
       case TempL:   return("TempL");    break;
       case Mode:    return("Mode");     break;
       case Hours:   return("Hours");    break;
       case CoP:     return("CoP");      break;
      }
      return "";
}
 

/*
 * handle timings of the low and high prio DPs. 
 * This function checks if the minimum time between two access attempts is elapsed. 500ms.
 */
bool readyToSendToVito() {
  static unsigned long   timeoutProbeVitoTimer = 0;
  if (millis() > (timeoutProbeVitoTimer+ TIMEOUT_PROBE_VITO ) ) {
      timeoutProbeVitoTimer= millis();
      return true; } 
  return false; 
}

/*
 * This function checks if the minimum temporal distance between two low priority DP accesses is elapsed.
 */
bool readyToSendDPlow() {
  static unsigned long  timeoutProbeDPlowTimer = 0;
  if (millis() > (timeoutProbeDPlowTimer+ TIMEOUT_DP_LOW )) {
      timeoutProbeDPlowTimer= millis();
      return true; } 
  return false; 
}

/*
 * Get next DP to probe. Checks low and high priority and picks the right one.
 */
int getNextDP () {// {disabled , low, high}
  static unsigned int DPptrLow=0, DPptrHigh=0; 
  if (readyToSendToVito()== false) {

   // Serial.print(".");
    return -1; 
  }
  if (readyToSendDPlow() == true){
    while (1){
      if ( ++DPptrLow >= NUMBER_OF_DP ) DPptrLow = 0; 
      if ( DPoints[DPptrLow].priority == low)   return (DPptrLow);
      }
  }  else {
    while (1){
      if ( ++DPptrHigh >= NUMBER_OF_DP ) DPptrHigh = 0; 
      if ( DPoints[DPptrHigh].priority == high) return (DPptrHigh);
    }    
  }
}

/*
 * return  DP index from its given name.
 */
unsigned int getIndexFromName(String name){
   
   //Serial.println("Looking for " + name);
   for (int i = 0; i< NUMBER_OF_DP; i++ ) { 
    //Serial.println ("Vito_"+ String((char*)DPoints[i].name) );
     if (  "Vito_"+String((char*)DPoints[i].name) ==  name)  {
       //Serial.println("MATCH");
       return i; 
     }
   }
  return (0);
}


/*
 * convert the type of a sensor into a string
 */
String SensorTypetoString ( EnumSensorType sensorType ){
    switch( sensorType) {
      case temperature:  return("temperature");     break;
      case state:        return("state");    break;
      case setpoint:     return("setpoint"); break;
      case power:        return("power");    break;
      case None:         return("None");  break;
      case Time:         return("time");    break;
      case energy:       return("energy");   break;
      case pressure:     return("pressure"); break;
      }
      return "";
}

/*
 * convert the data type enum into a string.
 */
String DataTypeTpString( EnumDataType type){

     switch(type) {
       case CountS:  return("CountS");   break;
       case CountL:  return("CountL");   break;
       case TempS:   return("TempS");    break;
       case TempL:   return("TempL");    break;
       case Mode:    return("Mode");     break;
       case Hours:   return("Hours");    break;
       case CoP:     return("CoP");      break;
      }
      return "";
}

/*
 * this function will dump a pre-formatted things file for OpenHab to the console. 
 * Handy to avoid typos.
 */
void generateThingsFile(){
Serial.begin (115200); 
delay(10);
Serial.println();Serial.println();Serial.println();
for (int i = 0; i< NUMBER_OF_DP; i++ ) { 
  String s = "     Type number : Channel_Vito_"; s= s +(char*)DPoints[i].name; 
  while (s.length()<65){ s=s+" ";}
  Serial.print(s);  

  s=("\""); s=s+String((char*)DPoints[i].name); s=s+"\"";
  while (s.length()<40) s=s+" ";
  Serial.print(s);
  s= "[ stateTopic=\"";  s=s+ getStateTopic(i); s=s+"\" ,";
  while (s.length()<100) s=s+" ";
  Serial.print(s);
  
  s= "commandTopic=\""; s=s+getCommandTopic(i); s=s+"\" ,";
  while (s.length()<100) s=s+" ";
  Serial.print(s);
  
  
  s="expire=\"1h,UNDEF\"  ] ";
  Serial.print(s);   
  Serial.println(); 
 }

}

/*
 * used for debugging
 */ 
 void dumpDPs(){  for (int i = 0; i< NUMBER_OF_DP; i++ ) { dumpSingleDPtoSerial(i);  } } 
 void dumpSingleDPtoSerial(int i){
   return;
   Serial.println();
     if ((i<0)|| (i > NUMBER_OF_DP)) return;
     switch(DPoints[i].priority) {
       case high:    Serial.print("high: ");      break;
       case low:    Serial.print("low: ");      break;
       case disabled:    Serial.print("disabled: ");      break;
       }

     Serial.print("Name: " + String((char*)DPoints[i].name));
     Serial.print(" Address: 0x"); Serial.print(DPoints[i].address, HEX);
     Serial.print(" Type: "+ DataTypeTpString(DPoints[i].dataFormatType));
     
     Serial.print(" Factor: "); Serial.print(DPoints[i].factor);
     Serial.print(" RW: "); 
     switch(DPoints[i].ReadWrite) {
       case RW:    Serial.print("RW");      break;
       case RO:    Serial.print("RO");      break;
       }
     Serial.print(" sensorType: " + SensorTypetoString(DPoints[i].sensorType)); 
     Serial.print("UoM: " + String((char*)DPoints[i].UoM));
     Serial.print("TypeTopic: " + String((char*)DPoints[i].sensorTypeTopic));
 
 }


 /*
  * functions to generate strings for home assistant auto discovery
  */

String getUniqueID(unsigned int index){         return  "Vito_"          +  String((char*)DPoints[index].name);}
String getStateTopic(unsigned int index){       return  "homeassistant/" +  String((char*)DPoints[index].sensorTypeTopic)+  "/Vitotronic/"+ getUniqueID(index)  +  "/state";}
String getCommandTopic(unsigned int index){     return  "homeassistant/" +  String((char*)DPoints[index].sensorTypeTopic)+  "/Vitotronic/"+ getUniqueID(index)  +  "/command";}
String getSubscribeTopic(void){                 return  "homeassistant/+/Vitotronic/+/command";}
String getConfigTopic(unsigned int index){      return  "homeassistant/" +  String((char*)DPoints[index].sensorTypeTopic)+  "/Vitotronic/"+ getUniqueID(index)  +  "/config";}
String getAvailabilityTopic(unsigned int index){return  "homeassistant/" +  String((char*)DPoints[index].sensorTypeTopic)+  "/Vitotronic/available";}
String getDeviceClass(unsigned int index){      return                      SensorTypetoString(DPoints[index].sensorType);}// temperatur ?
String getUoM(unsigned int index){              return                      String((char*)DPoints[index].UoM);}

/*
 * publish autodiscovery message for a single DP
 */
 
void publish_discovery_message(unsigned int index) {
  // Publish a discovery message for a variable https://www.home-assistant.io/integrations/climate.mqtt/
   String payload = "{";
    payload += "\"name\":\""                      + getUniqueID(index) +          "\",";
    payload += "\"unique_id\":\""                 + getUniqueID(index) +          "\",";
    payload += "\"device\": {\"identifiers\": \"heater\",\"name\": \"Viessmann WO1B\",\"sw_version\": \"1.0\",\"model\": \"WO1B\",\"manufacturer\": \"Viessmann\"},";
    payload += "\"device_class\":\""              + getDeviceClass(index) +       "\",";      // temperature ?
    payload += "\"unit_of_measurement\":\""       + getUoM(index) +               "\",";
    payload += "\"state_topic\":\""               + getStateTopic(index)+         "\",";
    payload += "\"availability_topic\":\""        + getAvailabilityTopic(index) + "\",";
    payload += "\"payload_available\":\"online\",";
    payload += "\"payload_not_available\":\"offline\"";
    payload += "}";
    CheckAndPublish(getConfigTopic(index),payload ) ; 
}

/*
 * publish all DPs for autodiscovery
 */
   
void advertiseDPs()
{
     for (int i = 0; i< NUMBER_OF_DP; i++ ) {
       if ( DPoints[i].priority != disabled){
          publish_discovery_message (i);
       }
     }
}
