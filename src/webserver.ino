/*
 * Small web page that facilitates probing if unknown data points. 
 * There are also sub-pages /read and /write that allow you to acces your heaters DPs from http.
 */

#include "webpage.h"
#include <ESP8266WebServer.h>    // Include the WebServer library
ESP8266WebServer myWebServer(80);    // Create a webserver object that listens for HTTP request on port 80


void WebserverSetup(void){
  myWebServer.on("/", handleRoot);               // Call the 'handleRoot' function when a client requests URI "/"
  myWebServer.onNotFound(handleNotFound);        // When a client requests an unknown URI (i.e. something other than "/"), call function "handleNotFound"
  myWebServer.on("/read", webPage_read);
  myWebServer.on("/write", webPage_write);
  myWebServer.begin();                           // Actually start the myWebserver
}

void WebserverLoop (void) {
   myWebServer.handleClient();
}


void handleRoot() {
  myWebServer.send(200, "text/plain", probingwebpage);   
}

void handleNotFound(){
  myWebServer.send(404, "text/plain", "404: Not found"); // Send HTTP status 404 (Not Found) when there's no handler for the URI in the request
}


void webPage_read() { // webpage "read" Handler 
    unsigned int address, readLen;
    double factor=0;
    while (!readyToSendToVito); 
    if (myWebServer.args() < 2)              { myWebServer.send(200, "text/plain", F("You must specify at least 2 arguments, e.g. http://192.168.1.18/read?DP=0x2000&Type=TempL")); return;}
    if (myWebServer.arg("DP") =="")          { myWebServer.send(200, "text/plain", F("DP argument not found")); return;}
    if (myWebServer.arg("Type") =="")        { myWebServer.send(200, "text/plain", F("Type argument not found Use &Type=TempL|TempS|CountL|CountS|Mode|Hours|CoP|Raw|all")); return;}
    if (((myWebServer.arg("Type") =="Raw") ||(myWebServer.arg("Type") =="all"))
     && (myWebServer.arg("ReadLen") =="" ))  { myWebServer.send(200, "text/plain", F("No ReadLen specified for Type Raw or all")); return;}
    if (((myWebServer.arg("Type") !="Raw")
     &&(myWebServer.arg("Type") !="all")) 
     && (myWebServer.arg("ReadLen") !="" )) { myWebServer.send(200, "text/plain",  F("Do not specify a ReadLen unless you use Raw or all")); return;}
    
    if (myWebServer.arg("factor") !="") { factor = atof(myWebServer.arg("factor").c_str());  }
    
    address = strtol(myWebServer.arg("DP").c_str(),0,16); // interpret DP adress as hex
    type=myWebServer.arg("Type");  
    
    if ((type == "Raw") || (type == "all") )
        readLen = myWebServer.arg("ReadLen").toInt();  
     else
        readLen = TypeStringToLen(myWebServer.arg("Type")); 
        
    if (readLen>10)  { myWebServer.send(200, "text/plain", F("Do not read more than 10 bytes")); return;}
   
    if (readWriteDP(address,readLen,out_str,false)){ // false = read
        message = RawToStr(type,out_str,readLen,factor);
        
        if  (myWebServer.arg("MQTTTopic") !="")  {
  //        MQTTclient.publish(myWebServer.arg("MQTTTopic").c_str(), message.c_str());
          }
    } else  message = "timeout";
    
    logString+=F(" Value ");
    logString += message; 
    //addlog(LOG_LEVEL_INFO,logString);
    message += "\n";  
    
    //Serial1.println(message);
    myWebServer.send(200, "text/plain", message);//Response to the HTTP request
} 

void webPage_write() { // webpage "read" Handler 
    unsigned int address, writeLen;
    if (myWebServer.args() < 2)              { myWebServer.send(200, "text/plain", F("You must specify at least 2 arguments, e.g. http://192.168.1.111/read?DP=0x2000&Type=TempL")); return;}
    if (myWebServer.arg("DP") =="")          { myWebServer.send(200, "text/plain", F("DP argument not found")); return;}
    if (myWebServer.arg("Type") =="")        { myWebServer.send(200, "text/plain", F("Type argument not found Use &Type=TempL|TempS|CountL|CountS|Mode|Hours|CoP|Raw|all")); return;}
    if (myWebServer.arg("Value") =="")       { myWebServer.send(200, "text/plain", F("Write value not specified")); return;}
    if (myWebServer.arg("Type") =="Raw")     { myWebServer.send(200, "text/plain", F("Writing Raw is not supported. Use Type=TempL|TempS|CountL|CountS|Mode|Hours|CoP")); return;}
      
    address = strtol(myWebServer.arg("DP").c_str(),0,16); // interpret DP adress as hex
    type=myWebServer.arg("Type");  
    writeLen = TypeStringToLen(myWebServer.arg("Type")); 
    StrToRaw (myWebServer.arg("Value"), myWebServer.arg("Type"), out_str);
   
    message = RawToStr("all",out_str,writeLen); 
    
    if (readWriteDP(address,writeLen,out_str,true)) // true means write
        message += "write success\n";
    else
         message += "write fail\n";

    logString+=F(" Value ");
    logString += RawToStr(type,out_str,writeLen); ; 
    //addlog(LOG_LEVEL_INFO,logString);
 
    message += "\n";  
    myWebServer.send(200, "text/plain", message);//Response to the HTTP request
} 
 
 
