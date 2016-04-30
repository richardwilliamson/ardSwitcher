#include "sharedOSC.h"

#include "osc.h"

void checkOSC()
{
  int size;
  if ((size = udp.parsePacket()) > 0)
  {
    OSCMessage msg;

    while (size--)
      msg.fill(udp.read());
    yield();

    if (!msg.hasError())
    {
      String outMsg = oscStart + "out";
      if (!msg.match(outMsg.c_str(), outMsg.length()))
      { //don't try to route any output from another device
        outMsg = oscStart + "registerMaster";
        if (msg.fullMatch(outMsg.c_str(), 0))
        {
          setIP(udp.remoteIP());
          DEBUG_PRINT("master IP now set");
          DEBUG_PRINT(masterIP);

        } else
          routeOSC(msg);
      } else
      {
        DEBUG_PRINT(F("we ignored an output"));
      }

    }

  }
}

void makeOutOSC(String &postScript)
{

  String firstPart = oscStart + "out/station/" + String(station) + "/";

  postScript = firstPart + postScript;

}

void routeOSC(OSCMessage &msg)
{
  //note that "/ardQlight/registerMaster" is matched in the main loop as we need to extract the IP address of the sender
  String firstPart = oscStart + "station/" + String(station);

  if (msg.route(firstPart.c_str(), osc_routeStation)) //send everything with the above to the place
    return;

  firstPart = oscStart + "unit/" + String(ESP.getChipId());

  if (msg.route(firstPart.c_str(), osc_routeStation)) //send everything with the above to the same place
    return;

  firstPart = oscStart + "all";
  if (msg.route(firstPart.c_str(), osc_routeStation)) //send everything with the above to the same place
    return;

  DEBUG_PRINT("NOT ROUTED");
}

//items not covered her..
//
// if (msg.route("/standby", osc_standby, addr))
//        return;
//
//     if (msg.route("/go", osc_go, addr))
//        return;
//
//     if (msg.route("/clearGo", osc_clearGo, addr))
//        return;
//
//    if (msg.route("/clear", osc_clear, addr))
//        return;
//
//     if (msg.route("/flash", osc_flash, addr))
//        return;


void osc_routeStation(OSCMessage &msg, int addr)
{
  DEBUG_PRINT(F("routing for this station"));

  if (msg.route("/set", osc_routeStationSet, addr))
    return;

  if (msg.route("/get", osc_routeStationGet, addr))
    return;

  if (msg.route("/ping", osc_ping, addr))
    return;

  if (msg.route("/poll", osc_poll, addr))
    return;

  if (msg.route("/enableWebConfig", osc_enableWebConfig, addr))
    return;

  if (osc_routeAppStation(msg, addr))
    return;

  DEBUG_PRINT(F("NOT ROUTED THOUGH"));
}

void sendOSC(OSCMessage &msg)
{
  if (!masterIP)
    return; //can't send a response until we have a master IP

  if (msg.hasError())
    return; //can't send a message with an error

  udp.beginPacket(masterIP, sendPort);
  msg.send(udp);
  udp.endPacket();

  msg.empty();

}

void osc_poll(OSCMessage &message, int addr)
{ //re-send all the statuses for everything
  //  poll();
}

//**********************************************
//********BASIC OSC ACTIONS*********************
//**********************************************
void osc_ping(OSCMessage &message, int addr)
{
//  pingOn();
  sendPing();
}

//***************************************************
//*******OSC SETTINGS SETTERS************************
//***************************************************

void osc_routeStationSet(OSCMessage &msg, int addr)
{
  DEBUG_PRINT(F("routing setters"));

  if (msg.route("/station", osc_changeStationNumber, addr))
    return;
  if (msg.route("/sendPort", osc_changeSendPort, addr))
    return;
  if (msg.route("/stationName", osc_setStationName, addr))
    return;

  if (osc_routeAppStationSet(msg, addr))
    return;

  DEBUG_PRINT(F("NOT ROUTED"));

}

void osc_changeStationNumber(OSCMessage &message, int addr)
{
  // Serial.println("changing station");
  //get the int value of the message's first argument

  int newID = message.getInt(0);

  if (newID < 1 || newID > 255)
    return; //can't set to that one!
  setStation(newID);
//  resetState();

  delay(10);

  //  poll();

  DEBUG_PRINT("Station changed to");
  DEBUG_PRINT(newID);
}
void osc_changeSendPort(OSCMessage &message, int addr)
{
  int newPort = message.getInt(0);

  if (newPort < 1)
    return; //can't set to that one!
  setSendPort(newPort);
 // resetState();

  delay(10);

  //  poll();

}

void osc_setStationName(OSCMessage &message, int addr)
{
  int length = message.getDataLength(0);
  char newName[length];

  if (length > 17) length = 17; //max length is 17 (16+/r)

  message.getString(0, newName, length);

  setStationName(newName);

  delay(10);

  sendStationName();

}

//***************************************************
//*******OSC SETTINGS GET-ERS************************
//***************************************************


void osc_routeStationGet(OSCMessage &msg, int addr)
{

  if (msg.route("/get/unitID", osc_getUnitID, addr))
    return;

  if (msg.route("/get/IP", osc_getIP, addr))
    return;

  if (msg.route("/get/sendPort", osc_getSendPort, addr))
    return;

  if (msg.route("/get/version", osc_getVersion, addr))
    return;

  if (msg.route("/get/stationName", osc_getStationName, addr))
    return;

  if (osc_routeAppStationGet(msg, addr))
    return;
}

void osc_getUnitID(OSCMessage &message, int addr)
{
  sendChipID();
}

void sendChipID()
{
  DEBUG_PRINT("sending station ID");
  DEBUG_PRINT(ESP.getChipId());
  //respond with the station ID (chipID()) as a String
  String newM("unitID");
  makeOutOSC(newM);
  OSCMessage ret(newM.c_str());
  ret.add(String(ESP.getChipId()).c_str());

  sendOSC(ret);
}
void sendIP()
{
  DEBUG_PRINT("sending IP");
  DEBUG_PRINT(WiFi.localIP());
  String newM("IP");
  makeOutOSC(newM);
  OSCMessage ret(newM.c_str());
  ret.add(WiFi.localIP().toString().c_str());

  sendOSC(ret);

}
void osc_getIP(OSCMessage &message, int addr)
{
  sendIP();
}
void sendSendPort()
{
  DEBUG_PRINT("SENIDING SEND PORT");
  DEBUG_PRINT(sendPort);

  String newM("sendPort");
  makeOutOSC(newM);
  OSCMessage ret(newM.c_str());
  ret.add(sendPort);

  sendOSC(ret);
}
void osc_getSendPort(OSCMessage &message, int addr)
{
  sendSendPort();

}

void sendVersion()
{
  DEBUG_PRINT(F("SENDING VERSION"));
  //   DEBUG_PRINT(VERSION_NUMBER);
  String newM("version");
  makeOutOSC(newM);
  OSCMessage ret(newM.c_str());
  ret.add(String(FPSTR(VERSION_NUMBER)).c_str());

  sendOSC(ret);
}
void osc_getVersion(OSCMessage &message, int addr)
{
  sendVersion();
}

void sendStationName()
{
  DEBUG_PRINT(F("SENDING STATION NAME"));
  DEBUG_PRINT(stationName);

  String newM("stationName");
  makeOutOSC(newM);
  OSCMessage ret(newM.c_str());
  ret.add(stationName);
  sendOSC(ret);
}

void osc_getStationName(OSCMessage &message, int addr)
{
  sendStationName();
}

void osc_enableWebConfig(OSCMessage &message, int addr)
{
  //get int/float arg
  int val = message.getInt(0);

  if (val == -1)
    val = message.getFloat(0); //no int, try float

  bool webConfigEnabled = val == 1;

  wifiManager.setAlwaysOnIsOn(webConfigEnabled);

}

void sendPing()
{
  DEBUG_PRINT("SENDING PING");
  String newM("ping");
  makeOutOSC(newM);
  OSCMessage ret(newM.c_str());
  sendOSC(ret);
}


void poll()
{
  sendChipID(); //sends back the chip ID for the chip
  sendIP();
  sendSendPort();
  sendVersion();
  sendStationName();

  appPing();
}

