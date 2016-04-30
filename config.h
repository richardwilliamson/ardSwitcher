#ifndef CONFIG_h
#define CONFIG_h

#include <WiFiManager.h>

//if manual true then we force the AP to appear

WiFiManager wifiManager; 

void makeWifiManager();

void wifiManagerLoopCallback(WiFiManager *manager);
void updateValuesCallback(WiFiManager *manager);
void saveSettingsCallback();

void launchWifiManager(bool mannual);




#endif
