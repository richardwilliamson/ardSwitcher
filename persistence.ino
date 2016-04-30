#include "persistence.h"

#include <EEPROM.h>


void readAll()
{  //todo should we check if we have already read?
  
   EEPROM.begin(512); 
   EEPROM.get(SETUP_STORE_STATION, station);
   DEBUG_PRINT("STATION IS");
   DEBUG_PRINT(station);
   
   EEPROM.get(SETUP_STORE_SENDPORT, sendPort);
   if (sendPort<1) sendPort = 2000;
   DEBUG_PRINT("SEND PORT IS");
   DEBUG_PRINT(sendPort);
   
   EEPROM.get(SETUP_STORE_RECPORT, recievePort);
   if (recievePort<1 || recievePort>10000) recievePort = 1000;

   DEBUG_PRINT("REC_PORT IS");
   DEBUG_PRINT(recievePort);
   
   readIP(masterIP, SETUP_STORE_IP);
   DEBUG_PRINT("MASTER IP IS");
   DEBUG_PRINT(masterIP);

   EEPROM.get(SETUP_STORE_STATION_NAME, stationName);
   DEBUG_PRINT("CHECKING STATION NAME");
   DEBUG_PRINT(stationName);
   bool gotLineEnd = false;
   for (byte i=0; i<sizeof(stationName); i++)
   {
      if (stationName[i] == '\0')
      {
          gotLineEnd = true;
          break;
      }
      if (!isAscii(stationName[i])) stationName[i] = ' ';
   }
   if (!gotLineEnd)
      setStationName("NO NAME");
      
   DEBUG_PRINT("STATION NAME IS");
   DEBUG_PRINT(stationName);
   
   EEPROM.end();
}

void setStation(byte sta)
{
  station = sta;
   EEPROM.begin(512);
   EEPROM.put(SETUP_STORE_STATION, station);
   EEPROM.commit();
   EEPROM.end();
}
void setIP(IPAddress ip)
{
   masterIP = ip;
   EEPROM.begin(512);
   updateIP(masterIP, SETUP_STORE_IP);
   EEPROM.commit();
   EEPROM.end();
}
void setSendPort(int sPort)
{
   sendPort = sPort;
   EEPROM.begin(512);
   EEPROM.put(SETUP_STORE_SENDPORT, sendPort);
   EEPROM.commit();
   EEPROM.end();
}
void setRecPort(int recPort)
{
   recievePort = recPort;
   EEPROM.begin(512);
   EEPROM.put(SETUP_STORE_RECPORT, recPort);
   EEPROM.commit();
   EEPROM.end();
}

void setStationName(const char * sName)
{
   strcpy(stationName, sName);
   EEPROM.begin(512);
   EEPROM.put(SETUP_STORE_STATION_NAME, stationName);
   EEPROM.commit();
   EEPROM.end();
}


void updateIP(IPAddress &ip, byte storeAddr)
{
  for (byte i=0; i<4; i++)
    EEPROM.put(storeAddr+i, ip[i]);
}
void readIP(IPAddress &ip, byte storeAddr)
{
  for (byte i=0; i<4; i++)
    EEPROM.get(storeAddr+i, ip[i]);
}
