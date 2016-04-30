#include "config.h"
#include "ardSwitcher.h"

//if manual true then we force the AP to appear

bool madeManager = false;

WiFiManagerParameter stationIDLabel("Station ID (1>255)");
WiFiManagerParameter stationIDParm("sid", "station ID (0-255)", "000", 4);

WiFiManagerParameter sendPortLabel("Transmit Port (>1000)");
WiFiManagerParameter sendPortParm("sendPort", "tansmit port", "0000", 5);

WiFiManagerParameter stationNameLabel("Station Name");
WiFiManagerParameter stationNameParm("stationName", "station Name", "", 17);

void updateValuesCallback(WiFiManager *manager)
{
    stationIDParm.setDefaultValue(String(station).c_str());
    stationNameParm.setDefaultValue(stationName);
    sendPortParm.setDefaultValue(String(sendPort).c_str());

}

void saveSettingsCallback()
{
  setStation(String(stationIDParm.getValue()).toInt());
  setStationName(stationNameParm.getValue());
  setSendPort(String(sendPortParm.getValue()).toInt());

  poll();
}


void makeWifiManager()
{
  if (!madeManager)
  {
    #ifndef DEBUG
      manager.setDebug(false);
    #endif

    wifiManager.setDisplayExistingCreds(true);
    wifiManager.addParameter(&stationIDLabel);
    wifiManager.addParameter(&stationIDParm);
    wifiManager.addParameter(&stationNameLabel);
    wifiManager.addParameter(&stationNameParm);
    wifiManager.addParameter(&sendPortLabel);
    wifiManager.addParameter(&sendPortParm);

    wifiManager.setDisplaySettingsPageCallback(updateValuesCallback);
    wifiManager.setSaveConfigCallback(saveSettingsCallback);

    wifiManager.setDisplayUploadOption(true); //so we can update software through it

    madeManager = true;

  }

}

void launchWifiManager(bool manual)
{
  makeWifiManager();
  String apSSID = "ardQ"+String(ESP.getChipId());
  String apPass = "12345678";

  wifiManager.setLoopCallback(wifiManagerLoopCallback);

  wifiManager.setConfigPortalTimeout(0); //no timeout

  if (manual)
    wifiManager.startConfigPortal(apSSID.c_str(), apPass.c_str());
  else
    wifiManager.autoConnect(apSSID.c_str(), apPass.c_str());

  wifiManager.setLoopCallback(NULL);
}


long lastMLoop = 0;
bool redOn = false;

void wifiManagerLoopCallback(WiFiManager *manager)
{
   if (millis()-lastMLoop >200)
   { //only do stuff every 200ms
      lastMLoop = millis();
      redOn = !redOn;
//      standbyLampChange(redOn);
      delay(10);
//      goLampChange(!redOn);
   }

   //check for ack being down, if down for 1 sec quit the manager
   long startDown = millis();
   /*
   while (digitalRead(PIN_ACKNOWLEDGE)==SWITCH_IS_DOWN)
   {
       yield();
       if (millis()-startDown >1000)
       {
          manager->setConfigPortalTimeout(1); //which should make it quit..
          return;
       }
   }*/
   
}


  
