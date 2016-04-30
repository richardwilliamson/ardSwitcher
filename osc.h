#ifndef OSC_h
#define OSC_h

#include "sharedOSC.h"
#include <OSCMessage.h>


String oscStart = "/ardSwitcher/";

//osc expected

// /enable - enables for transmission
  //enable can have certain arguments (note that these only affect this specific enable, any further enables etc will restore to current persistant settings
  // "singleShot" - only send the message once, then disable
  // "argument:XX" - an argument to insert into the OSC address as specified
  // "onOn" - only send when the state changes from ON to OFF
  // "onOff" - only send when the state changed from OFF to ON
  // "onStateChange" - send when the state changes in either direction
  
// /disable - disables transmission

// /set/..
  // /singleShot true/false
  // /OSCAddress "A/STRING/WITH/AN/{ARGUMENT}" - the wildcard will be replaced 
  // /argument "STRING" - the default argument
  // /trigger "onOn", "onOff", "onStateChange"


//route the specific station OSC for the app
boolean osc_routeAppStation(OSCMessage &msg, int addr);
boolean osc_routeAppStationSet(OSCMessage &msg, int addr);
boolean osc_routeAppStationGet(OSCMessage &msg, int addr);


void osc_enable(OSCMessage &msg, int addr);
void osc_disable(OSCMessage &msg, int addr);

void osc_setSingleShot(OSCMessage &msg, int addr);
void osc_setAddress(OSCMessage &msg, int addr);
void osc_setTrigger(OSCMessage &msg, int addr);

void osc_sendTrigger(byte state);

void appPing();


#endif
