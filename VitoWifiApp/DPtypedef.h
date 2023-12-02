#ifndef DPtpyedefh
#define DPtpyedefh
#include <c_types.h>

    enum EnumDataType { na, CountS, CountL, Mode, TempS,TempL,Hours,CoP,STAT};
    enum EnumReadWrite {RO,  RW,  WO };
    enum EnumSensorType {temperature , setpoint, state, power, None, Time, energy, pressure, binary_sensor,EspIO};
    enum EnumPriorityType {disabled , low, high};


    
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

#endif
