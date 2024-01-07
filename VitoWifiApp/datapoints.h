/*
 * List of data points for the WO1B unit. 
 * Address is the address of the datapoint in the heater
 * Factor is the scaling that viessmann uses. Sometimes they send you a 10th of the value
 * Read Write...
 * SensorType: mainly used for Homeassistant auto discovery. When it reads ESPIO, this accesses the I/O of the ESP. I have a two relays connected here that can pause or power off the heater. 
 * UoM: units of measurement. Used for autodiscovery
 * binary sensor: Type of sensor. Used for autodiscovery  https://www.home-assistant.io/integrations/sensor/ https://www.home-assistant.io/integrations/binary_sensor/
 * poll prio: low, high, disabled. See datapoints.ino for description
 */

 
#ifndef datapointsh
#define datapointsh


    const vitoDPstruct DPoints [] = 
    {
    // Name             ,           Adress, Type ,  Factor, Read/Write , SensorType, UoM, binary sensor , poll prio 
    {"VitoOnOff",                   0x000c, na,        1, RO , EspIO,       ""       , "switch" , low  },  // ESP IO port pins
    {"EVU",                         0x0004, na,        1, RO , EspIO,       ""       , "switch" , low  },  // ESP IO port pins
    {"Betriebsmodus",               0xb000, Mode,      1, RW , None,        ""       , "sensor" , low  }, 
    {"Geraetekennung",              0x00f8, CountS,    1, RO , None,        ""       , "sensor" , disabled  },  
    {"Betriebsmodus",               0xb000, Mode,      1, RW , None,        ""       , "sensor" , low  }, 
    {"ManuellerModus",              0xb020, Mode,      1, RW , None,        ""       , "sensor" , low  }, 
    {"Freigabe_WasserElektro",      0x6015, Mode,      1, RW , power,       ""       , "binary_sensor" , low   }, 
    {"Freigabe_HeizenElektro",      0x7902, Mode,      1, RW , power,       ""       , "binary_sensor" , low   },  
    {"Freigabe_Durchlauferh",       0x7900, Mode,      1, RW , power,       ""       , "binary_sensor" , low   }, 
    {"Freigabe_Kuehlung",           0x71fe, Mode,      1, RW , power,       ""       , "binary_sensor" , low   },  
    {"Status_Verdichter",           0x0400, Mode,      1, RO , power,       ""       , "binary_sensor" , high   },  
    {"Status_Ventilatorstufe1",     0x0402, Mode,      1, RO , power,       ""       , "binary_sensor" , high   },  
    {"Status_Ventilatorstufe2",     0x0404, Mode,      1, RO , power,       ""       , "binary_sensor" , high   },  
    {"Status_Durchlauferh_Stufe1",  0x0408, Mode,      1, RO , power,       ""       , "binary_sensor" , high   },  
    {"Status_Durchlauferh_Stufe2",  0x0409, Mode,      1, RO , power,       ""       , "binary_sensor" , high   },  
    {"Status_HK_UmkehrCool",        0x041c, Mode,      1, RO , power,       ""       , "binary_sensor" , high   },  
    {"Status_Pumpe_Heizkreis_A1",   0x040d, Mode,      1, RO , power,       ""       , "binary_sensor" , high   },  
        //10             , temperature, "°C"       
    {"Status_Zusatzausgang_ZPumpe", 0x0410, Mode,      1, RO , power,       ""       , "binary_sensor" , disabled   },  
    {"Status_Umschaltventil_H_WW",  0x0414, Mode,      1, RO , power,       ""       , "binary_sensor" , high   },  
    {"Status_Speicherladepumpe",    0x0416, Mode,      1, RO , power,       ""       , "binary_sensor" , disabled   }, 
    {"Status_Kaeltekreisumkehr",    0x049c, Mode,      1, RO , power,       ""       , "binary_sensor" , disabled   },  
    {"Status_Sammelmeldung",        0x0411, Mode,      1, RO , power,       ""       , "binary_sensor" , high   }, 
    {"Aussentemperatur",            0x0101, TempL,     1, RO , temperature, "°C"     , "sensor" , high  }, 
    {"Raumtemperatur_ist",          0x0116, TempL,     1, RO , temperature, "°C"     , "sensor" , high  }, 
    {"AussentemperaturRegelung",    0x0952, TempL,     1, RO , temperature, "°C"     , "sensor" , disabled  }, 
    {"Brauchwassertemperatur_oben", 0x010D, TempL,     1, RO , temperature, "°C"     , "sensor" , high  }, 
    {"HysterreseWW_WP",             0x6007, TempL,     1, RO , None,     ""          , "sensor" , low  },  
    {"HysterreseWW_Elektro",        0x6008, TempL,     1, RO , None,     ""          , "sensor" , low  },  
      //20            , temperature, "°C"     
    {"Brauchwassertemperatur_soll", 0x6000, TempL,     1, RW , temperature, "°C"     , "sensor" , low  }, 
    {"Brauchwassertemperatur2_soll",0x600C, TempL,     1, RW , temperature, "°C"     , "sensor" , low  }, 
    {"RaumtemperaturSoll",          0x2000, TempL,     1, RW , temperature, "°C"     , "sensor" , low  }, 
    {"Red_RaumtemperaturSoll",      0x2001, TempL,     1, RW , temperature, "°C"     , "sensor" , low  }, 
    {"PartyRaumtemperaturSoll",     0x2022, TempL,     1, RW , temperature, "°C"     , "sensor" , low  }, 
    {"HeizkennlinieNiveau",         0x2006, TempL,     1, RW , None,        ""       , "sensor" , low  }, 
    {"HeizkennlinieSteigung",       0x2007, TempL,     1, RW , None,        ""       , "sensor" , low  }, 
    {"Raumtemperaturaufschaltung",  0x200a, Mode,      1, RW , None,        ""       , "sensor" , low  }, 
    {"KuehlkennlinieNiveau",        0x7110, TempL,     1, RW , None,        ""       , "sensor" , low  }, 
      //30           , temperature, "°C"
    {"KuehlkennlinieSteigung",      0x7111, TempL,     1, RW , None,     ""          , "sensor" , low  }, 
    {"MinVLTemp_Kuehlung",          0x7103, TempL,     1, RW , temperature, "°C"     , "sensor" , low  },
    {"TemperaturDifferenz_Heizung", 0x7003, TempL,     1, RW , temperature, "°C"     , "sensor" , low  },
    {"TemperaturDifferenz_Kuehlung",0x7004, TempL,     1, RW , temperature, "°C"     , "sensor" , low  },
    {"Temperatur_Verdampfer",       0x0102, TempL,     1, RO , temperature, "°C"     , "sensor" , disabled  }, 
    {"PrimaerkreisVLtemp_Luft",     0x0103, TempL,     1, RO , temperature, "°C"     , "sensor" , disabled  }, 
    {"PrimaerkreisRLtemp_Luft",     0x0104, TempL,     1, RO , temperature, "°C"     , "sensor" , disabled  }, 
    {"PrimaerkreisVLtemp_Wasser",   0x0105, TempL,     1, RO , temperature, "°C"     , "sensor" , high  }, 
    {"PrimaerkreisRLtemp_Wasser",   0x0106, TempL,     1, RO , temperature, "°C"     , "sensor" , high  }, 
    {"Druck_im_Verdampfer",         0x0680, TempL,     1, RO , pressure,    "Pa"     , "sensor" , disabled  }, 
        //40          , temperature, "°C"
    {"Druck_im_Kondensator",        0x0682, TempL,     1, RO , pressure,    "Pa"     , "sensor" , disabled  }, 
    {"Belastung_0_22K",             0x1620, TempL,     1, RO , Time,         "h"     , "sensor" , disabled  }, 
    {"Belastung_22_32K",            0x1622, TempL,     1, RO , Time,         "h"     , "sensor" , disabled  }, 
    {"Belastung_32_41K",            0x1624, TempL,     1, RO , Time,         "h"     , "sensor" , disabled  }, 
    {"Belastung_42_50K",            0x1626, TempL,     1, RO , Time,         "h"     , "sensor" , disabled  }, 
    {"Belastung_51_99K",            0x1628, TempL,     1, RO , Time,         "h"     , "sensor" , disabled  }, 
    {"Betriebsstd_Verdichter",      0x0580, Hours,     1, RO , Time,         "h"     , "sensor" , low  }, 
    {"Betriebsstd_Ventilatorstufe1",0x0582, Hours,     1, RO , Time,         "h"     , "sensor" , disabled  }, 
    {"Betriebsstd_Ventilatorstufe2",0x0584, Hours,     1, RO , Time,         "h"     , "sensor" , disabled  }, 
    {"Betriebsstd_Durchlauferh_1",  0x0588, Hours,     1, RO , Time,         "h"     , "sensor" , disabled  }, 
        //50     , temperature, "°C"
    {"Betriebsstd_Durchlauferh_2",  0x0589, Hours,     1, RO , Time,         "h"     , "sensor" , disabled  }, 
    {"Betriebsstd_Heizkreis_A1",    0x058D, Hours,     1, RO , Time,         "h"     , "sensor" , disabled  }, 
    {"Betriebsstd_Z_Pumpe",         0x058e, Hours,     1, RO , Time,         "h"     , "sensor" , disabled  }, 
    {"Betriebsstd_Sammelmeldung",   0x058f, Hours,     1, RO , Time,         "h"     , "sensor" , disabled  }, 
    {"Betriebsstd_Ventil_WW",       0x0594, Hours,     1, RO , Time,         "h"     , "sensor" , disabled  }, 
    {"Betriebsstd_Speicherpumpe",   0x0596, Hours,     1, RO , Time,         "h"     , "sensor" , disabled  }, 
    {"Betriebsstd_ActiveCooling",   0x059C, Hours,     1, RO , Time,         "h"     , "sensor" , disabled  }, 
    {"Einschalt_Verdichter",        0x0500, CountL,    1, RO , None,     ""          , "sensor" , low  }, 
    {"Einschalt_Primaerquelle",     0x0502, CountL,    1, RO , None,     ""          , "sensor" , disabled  }, 
    {"Einschalt_Sekundaerquelle",   0x0504, CountL,    1, RO , None,     ""          , "sensor" , disabled  }, 
    {"Verdichter_Umkehrungen",      0x051c, CountL,    1, RO , None,     ""          , "sensor" , disabled  }, 
    {"Hysterrese_Verdichter",       0x7304, CountL, 0.1 , RO , temperature, "°C"     , "sensor" , low       },    
        //60     , temperature, "°C"
    {"Einschalt_Sekundaerquelle",   0x0504, CountL,    1, RO , None,     ""          , "sensor" , disabled  }, 
    {"Einschalt_Durchlauferh_1",    0x0508, CountL,    1, RO , None,     ""          , "sensor" , disabled  }, 
    {"Einschalt_Durchlauferh_2",    0x0509, CountL,    1, RO , None,     ""          , "sensor" , disabled  }, 
    {"LeistungDurchlauferhitzer",   0x7907, Mode,      1, RW , None,     ""          , "sensor" , low  }, 
    {"Einschalt_Heizkreis_A1",      0x050d, CountL,    1, RO , None,     ""          , "sensor" , disabled  }, 
    {"Einschalt_Z_Pumpe",           0x0510, CountL,    1, RO , None,     ""          , "sensor" , disabled  }, 
    {"Einschalt_Sammelmeldung",     0x0511, CountL,    1, RO , None,     ""          , "sensor" , disabled  }, 
    {"Einschalt_Ventil_WW",         0x0514, CountL,    1, RO , None,     ""          , "sensor" , disabled  }, 
    {"Einschalt_Speicherpumpe",     0x0516, CountL,    1, RO , None,     ""          , "sensor" , disabled  }, 
    {"Stunden_Sammelmeldung",       0x0591, Hours,     1, RO , None,     ""          , "sensor" , low  }, 
    {"JAZgesamt",                   0x1680, CoP,       1, RO , None,     ""          , "sensor" , low  }, 
        //70        , temperature, "°C"
    {"JAZHeizen",                   0x1681, CoP,       1, RO , None,     ""          , "sensor" , low  }, 
    {"JAZWW",                       0x1682, CoP,       1, RO , None,     ""          , "sensor" , low  }, 
    {"COPWW",                       0x1691, CoP,       1, RO , None,     ""          , "sensor" , low  }, 
    {"COPHeizen",                   0x1690, CoP,       1, RO , None,     ""          , "sensor" , low  }, 
    {"ElektroEnergie_WW_12M",       0x1670, CountL, 0.1 , RO , energy,      "kWh"    , "sensor" , low  }, 
    {"ElektroEnergie_Heizen_12M",   0x1660, CountL, 0.1 , RO , energy,      "kWh"    , "sensor" , low  }, 
    {"WaermeEnergie_WW_12M",        0x1650, CountL, 0.1 , RO , energy,      "kWh"    , "sensor" , low  }, 
    {"WaermeEnergie_Heizen_12M",    0x1640, CountL, 0.1 , RO , energy,      "kWh"    , "sensor" , low  }, 
    {"ElektroEnergie_WW_1W",        0x16D0, CountS, 0.1 , RO , energy,      "kWh"    , "sensor" , low  }, 
    {"ElektroEnergie_Heizen_1W",    0x16D1, CountS, 0.1 , RO , energy,      "kWh"    , "sensor" , low  }, 
    {"WaermeEnergie_WW_1W",         0x16D2, CountS, 0.1 , RO , energy,      "kWh"    , "sensor" , low  }, 
          //80       , temperature, "°C"
    {"WaermeEnergie_Heizen_1W",     0x16D3, CountS, 0.1 , RO , energy,      "kWh"    , "sensor" , low  }, 
    {"Heizleistung",                0x16a0, CountL,0.01 , RO , power,       "kW"     , "sensor" , low  }, 
    {"Leistungsaufnahme",           0x16a4, CountL, 0.01, RO , power,       "kW"     , "sensor" , low  }, 
    {"mittl_Eingangstemp_Luft",     0x16b0, TempL,  1   , RO , temperature, "°C"     , "sensor" , disabled  }, 
    {"mittl_Ausgangstemp_Luft",     0x16b1, TempL,  1   , RO , temperature, "°C"     , "sensor" , disabled  }
    };
#endif
