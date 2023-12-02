#include <VitoWiFi.h>
#include "DPtypedef.h"
#include "datapoints.h"
#define WRITE true
#define READ false
OptolinkP300* myOptolinkPlugin_182 = NULL;



// Wait for vito started
// if the WO1B heater control is being talked to after power up, it goes into a bootloader mode. 
// Nothing bad happens, but it won't start. We know when it has started as soon as it sends 0x05.
// a good value for timeout is 20000 (ms)
void WaitForVitoStarted(unsigned long timeout)
{
  unsigned long t=millis(); 
  Serial.begin(4800); 
  Serial.swap(); 
  delay(1000); 
  while (Serial.available()) {Serial.read();}  // empty reas queue first.
  while (millis()< (t +timeout)){
    delay(100);
     if (Serial.available() > 0) {
      if (  Serial.read() == 0x05)   break;
    } 
  }
  Serial.swap(); 
}

/*
 * Just initialize the VitoWifi library. If you use Serial.swap depends on where you connect your hardware. Mine is on Serial1,
 */
void VitoWifiSetup(void){
  myOptolinkPlugin_182 = new OptolinkP300();
  myOptolinkPlugin_182->begin(&Serial);
  Serial.swap(); 
}


/*
 * Called cyclicly. Fetches next DP from the list and reads it from the heater.
 */
void VitoWifiLoop(void){
  if (myOptolinkPlugin_182 != NULL ) myOptolinkPlugin_182->loop();
  if (myOptolinkPlugin_182->connected()) {
      int a =  getNextDP() ; 
      if (a >= 0) {   mqtt_read(a); }
  }
}



/*
 * We use raw reading from VitoWifi. Decode ourselves into a String that holds just the value.
 */

String retval=""; 
String retval_all="";
DPValue globalDPvalue(23.6f);

String RawToStr (String type, uint8* raw, uint8 readLen=0, double factor=0){
      //if (factor == 0) factor =1; 
      if (readLen==0 ) readLen = TypeStringToLen(type);
      if (readLen>10) {//addLog(LOG_LEVEL_ERROR,"Readlen>10");  
      readLen=10;}
      if (type == "TempL"){ DPTemp temp_dp("TempL", "DPs", 0x00);               globalDPvalue = temp_dp.decode(raw);   if (factor!=0) retval = factor*globalDPvalue.getFloat(); else retval = globalDPvalue.getFloat();   } 
      else if (type == "TempS") {   DPTempS temps_dp("TempS", "DPs", 0x00);     globalDPvalue = temps_dp.decode(raw);  if (factor!=0) retval = factor*globalDPvalue.getU8();    else retval = globalDPvalue.getU8();  }
      else if (type == "STAT")  {   DPStat stat_dp("Stat", "DPs", 0x00);        globalDPvalue = stat_dp.decode(raw);     retval =  globalDPvalue.getBool() ? "true" : "false";      }
      else if (type == "CountL"){   DPCount count_dp("CountL", "DPs", 0x00);    globalDPvalue = count_dp.decode(raw);  if (factor!=0) retval = factor*globalDPvalue.getU32();   else retval = globalDPvalue.getU32();  }
      else if (type == "CountS"){   DPCountS counts_dp("CountS", "DPs", 0x00);  globalDPvalue = counts_dp.decode(raw); if (factor!=0) retval = factor*globalDPvalue.getU16();   else retval = globalDPvalue.getU16();   }
      else if (type == "Mode")  {   DPMode mode_dp("Mode", "DPs", 0x00);        globalDPvalue = mode_dp.decode(raw);   if (factor!=0) retval = factor*globalDPvalue.getU8();    else retval = globalDPvalue.getU8(); }
      else if (type == "Hours") {   DPHours hours_dp("Hours", "DPs", 0x00);     globalDPvalue = hours_dp.decode(raw);  if (factor!=0) retval = factor*globalDPvalue.getFloat(); else retval = globalDPvalue.getFloat(); }
      else if (type == "CoP")   {   DPCoP cop_dp("CoP", "DPs", 0x00);           globalDPvalue = cop_dp.decode(raw);    if (factor!=0) retval = factor*globalDPvalue.getFloat(); else retval = globalDPvalue.getFloat();}
      else if (type == "Raw")   {
        retval="";
        for (int i = 0; i<readLen; i++) {
          if (raw[i] < 0x10) retval +="0"; 
          retval += String(raw[i],HEX);
        }
      }
      else if (type == "all") {
        retval_all = "TempL:  " + RawToStr("TempL",raw,0,factor) +"\n";
        retval_all += "TempS:  " + RawToStr("TempS",raw,0,factor) +"\n";
        retval_all += "STAT:   " + RawToStr("STAT",raw,0,factor) +"\n";
        retval_all += "CountL: " + RawToStr("CountL",raw,0,factor) +"\n";
        retval_all += "CountS: " + RawToStr("CountS",raw,0,factor) +"\n";
        retval_all += "Mode:   " + RawToStr("Mode",raw,0,factor) +"\n";
        retval_all += "Hours:  " + RawToStr("Hours",raw,0,factor) +"\n";
        retval_all += "CoP:    " + RawToStr("CoP",raw,0,factor) +"\n";
        retval_all += "Raw:    " + RawToStr("Raw",raw, readLen ) +"\n";
        return retval_all;
      }
      
      else  retval = "unknown encoding requested";
      return retval;
}


/*
 * Encode a String so it can be processed by VitoWifi.
 */
void StrToRaw (String value, String type, uint8* raw ){
  
      for (int i = 0; i<10; i++) raw[i]=0;
      if (type == "TempL")      {   DPTemp temp_dp("TempL", "DPs", 0x00);       DPValue tempDP((float)atof(value.c_str())); temp_dp.encode(raw,tempDP);    } 
      else if (type == "TempS") {   DPTempS temps_dp("TempS", "DPs", 0x00);     DPValue tempDP((uint8_t)atoi(value.c_str())); temps_dp.encode(raw,tempDP);   }
      else if (type == "STAT")  {   DPStat stat_dp("Stat", "DPs", 0x00);        DPValue tempDP((bool )atoi(value.c_str())); stat_dp.encode(raw,tempDP);    }
      else if (type == "CountL"){   DPCount count_dp("CountL", "DPs", 0x00);    DPValue tempDP((uint32_t)atoi(value.c_str())); count_dp.encode(raw,tempDP);   }
      else if (type == "CountS"){   DPCountS counts_dp("CountS", "DPs", 0x00);  DPValue tempDP((uint16_t)atoi(value.c_str())); counts_dp.encode(raw,tempDP);  }
      else if (type == "Mode")  {   DPMode mode_dp("Mode", "DPs", 0x00);        DPValue tempDP((uint8_t)atoi(value.c_str())); mode_dp.encode(raw,tempDP);    }
      else if (type == "Hours") {   DPHours hours_dp("Hours", "DPs", 0x00);     DPValue tempDP((uint16_t)atoi(value.c_str())); hours_dp.encode(raw,tempDP);   }
      else if (type == "CoP")   {   DPCoP cop_dp("CoP", "DPs", 0x00);           DPValue tempDP((uint8_t)atoi(value.c_str())); cop_dp.encode(raw,tempDP);     }
      
}


/*
 * Write a DP to the heater. 
 */

 
String logString;

bool readWriteDP (word address, byte Len, uint8_t* rawdata, bool Write = false ){
  logString=""; 
  
  unsigned long now = millis(); 
  if ( Len==0 )  return false;
    if (  myOptolinkPlugin_182->connected()== false) logString+"VitoWifi not connected"; 
    while (myOptolinkPlugin_182->isBusy() && ((millis()-now) < 2000)) 
        {
        delay(10);  // wait for optolink to become available
        myOptolinkPlugin_182->loop();
        }
     // if (myOptolinkPlugin_182->isBusy() ) {  return false;  }
     logString+=(F("Vito: Address: 0x")); 
     if (address<0x1000) logString+=String("0");
     if (address<0x100) logString+=String("0");
     if (address<0x10) logString+=String("0");
        
     logString+=String( (uint16_t)address, HEX);
     logString+=String(F(" Len: "));
     logString+=String( Len);
       
     
        
    if (!Write) {
        logString+=("... read");
        myOptolinkPlugin_182->readFromDP( address, Len);  // issue read command
        }
    else 
        {
          logString+=("... write");
         myOptolinkPlugin_182->writeToDP( address, Len, rawdata);
        }
         
    while (((millis()-now) < 3000)){
        myOptolinkPlugin_182->loop();
        if (myOptolinkPlugin_182->available() > 0) {
          myOptolinkPlugin_182->read(rawdata);
           
          logString+=(F(" data: "));
          ////addlog(LOG_LEVEL_DEBUG, logString);
          for (uint8_t i = 0; i < Len; ++i) {
            if ( rawdata[i] < 0x10) logString+=("0");
            logString+=String(rawdata[i], HEX);
           }
           return true; 
        }
        if (myOptolinkPlugin_182->available() < 0) {
          logString+="Optolink error: ";logString+=(myOptolinkPlugin_182->readError());
           ////addlog(LOG_LEVEL_ERROR,logString);
          return false; 
        }
  }
 
   logString+=F("Vito: Optolink unknown error. ");
   CheckAndPublish("log", logString);
   //addlog(LOG_LEVEL_ERROR,logString);
  return false;
}
 
 
uint8_t out_str[10] = {0};
String type, message = "";

/*
 * Get a DP from mqtt and forward it to the heater. 
 * "index" is the number of the DP in the list. See datapoints.h
 * Note: There is a special sensorType that is called EspIO that reads and forwards an IO pin state to mqtt instead of a Vito value .
 */

bool mqtt_read(unsigned int index){
  unsigned int address = DPoints[index].address; 
    if (DPoints[index].sensorType== EspIO){
          CheckAndPublish( getStateTopic(index), (digitalRead(address)==1) ? "1" : "0");
          CheckAndPublish( "log I/O", (digitalRead(address)==1) ? "1" : "0");
          return true;
    } else {
        //  CheckAndPublish("log", "read"+ String(index));
        unsigned int readLen = TypeStringToLen(DPoints[index].dataFormatType);  // number of bytes to read
        type = DataTypeTpString(DPoints[index].dataFormatType);                 // data type as string (TEMPL)
        float factor = DPoints[index].factor;                                   // factor
        if (readWriteDP(address,readLen,out_str,READ)){ // false = read
            message = RawToStr(type,out_str,readLen,factor);
            CheckAndPublish( getStateTopic(index), message);
            return true;  
             
        } else {
            String s= "error reading DP "; s=s+index;  
            CheckAndPublish( "log", s);
            return false; 
        }  
    }
}

/*
 * read a DP from the heater and forward it to mqtt
 * Note: There is a special sensorType that is called EspIO that reads and forwards an IO pin state to mqtt instead of a Vito value .
 */

bool mqtt_write(unsigned int index, String value){
    unsigned int  writeLen = TypeStringToLen(DPoints[index].dataFormatType); 
    unsigned int address = DPoints[index].address;
    if (DPoints[index].sensorType== EspIO){
          pinMode(address, OUTPUT); 
          digitalWrite (address,value.toInt());
    } else {
          StrToRaw (value, DataTypeTpString(DPoints[index].dataFormatType), out_str);
          if  (!readWriteDP(address,writeLen,out_str,WRITE)){   return false;  }   
    }
    return mqtt_read(index);     
}


/*
 * return the number of bytes to send for a specific data type
 */
byte TypeStringToLen (EnumDataType type){  return TypeStringToLen(DataTypeTpString(type));}
byte TypeStringToLen (String type){
  byte readLen=4;   // for  CountL, Hours and Raw
      if      ( (type == "TempL") || (type == "CountS"))       readLen = 2;
      else if ( (type == "STAT" ) || (type == "TempS")  || (type == "Mode") || (type == "CoP") ) readLen = 1;
  return readLen;
}
