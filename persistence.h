#ifndef PERSISTENCE_h
#define PERSISTENCE_h

char stationName[17]; //16 + /

#define SETUP_STORE_VERSION 0 //17 bytes

#define SETUP_STORE_STATION 17 //1 byte
#define SETUP_STORE_IP 18 //4 bytes - 0 if DHCP
#define SETUP_STORE_SENDPORT 22 //2 bytes
#define SETUP_STORE_RECPORT 24 //2 bytes
#define SETUP_STORE_STATION_NAME 26 //17 bytes


byte station = 255;  //default to 255
IPAddress masterIP(0,0,0,0);
int sendPort = 2000;
int recievePort = 1000;

void readAll();

void setStation(byte station);
void setIP(IPAddress ip);
void setSendPort(int sendPort);
void setRecPort(int recPort);
void setStationName(const char * sName);

void updateIP(IPAddress &ip, byte storeAddr);
void readIP(IPAddress &ip, byte storeAddr);


#endif
