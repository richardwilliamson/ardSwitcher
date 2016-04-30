#ifndef SHAREDOSC_H
#define SHAREDOSC_H


void checkOSC();

void sendOSC(OSCMessage &message);

void routeOSC(OSCMessage &message);
void osc_routeStation(OSCMessage &message, int addr);

void osc_routeStationSet(OSCMessage &message, int addr);
void osc_routeStationGet(OSCMessage &message, int addr);


void osc_poll(OSCMessage &message, int addr);
void osc_ping(OSCMessage &message, int addr);

void osc_changeStationNumber(OSCMessage &message, int addr);
void osc_changeSendPort(OSCMessage &message, int addr);
void osc_setStationName(OSCMessage &message, int addr);

void osc_enableWebConfig(OSCMessage &message, int addr);

void osc_getUnitID(OSCMessage &message, int addr);
void osc_getIP(OSCMessage &message, int addr);
void osc_getSendPort(OSCMessage &message, int addr);
void osc_getVersion(OSCMessage &message, int addr);
void osc_getStationName(OSCMessage &message, int addr);

void makeOutOSC(String &postScript);

void sendChipID(); //sends back the chip ID for the chip
void sendIP();
void sendSendPort();
void sendVersion();
void sendStationName();
void sendPing();

void poll();




#endif
