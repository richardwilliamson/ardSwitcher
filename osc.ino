#include "osc.h"
#include "ardSwitcher.h"

//osc expected

  
// /disable - disables transmission

// /set/..
  // /singleShot true/false
  // /address "A/STRING/WITH/AN/{ARGUMENT}" - the wildcard will be replaced 
  // /argument "STRING" - the default argument
  // /trigger "onOn", "onOff", "onStateChange"

//route the specific station OSC for the app
boolean osc_routeAppStation(OSCMessage &msg, int addr)
{
   if (msg.route("/enable", osc_enable, addr))
      return true;

   if (msg.route("/disable", osc_disable, addr))
      return true;

   return false;
}


// /enable - enables for transmission
  //enable can have certain arguments (note that these only affect this specific enable, any further enables etc will restore to current persistant settings
  // "singleShot" - only send the message once, then disable
  // "arg:XX" - an argument to insert into the OSC address as specified
  // "onOn" - only send when the state changes from ON to OFF
  // "onOff" - only send when the state changed from OFF to ON
  // "onChange" - send when the state changes in either direction
void osc_enable(OSCMessage &msg, int addr)
{
  DEBUG_PRINT(F("ENABLED"));
  switcher_enabled = true;

  for (byte i=0; i<msg.size(); i++)
  {
    if (msg.getType(i)=='s')
    { //got a string
       char msgSt[msg.getDataLength(i)];
       msg.getString(i, msgSt, msg.getDataLength(i));

       DEBUG_PRINT("looking at");
       DEBUG_PRINT(msgSt);

       if (msgSt=="onOn")
       { //set trigger to ON

          continue;
       }

       if (msgSt=="onOff")
       { //set trigger to OFF

         continue;
       }

       if (msgSt=="onChange")
       { //set trigger to any state change
          
          continue;
       }

       if (msgSt=="singleShot")
       { //only fire once then disable

          continue;
       }

       if (strlen(msgSt)>4) // "arg:"
       { 
           String fullArg = String(msgSt);
           if (fullArg.startsWith("arg:"))
           { //good, it does..
              fullArg = fullArg.substring(4);
              setArgument(fullArg);
           }
           continue;
       }
       
    }
  }

  
}
void osc_disable(OSCMessage &msg, int addr)
{
  DEBUG_PRINT(F("DISABLED"));
   switcher_enabled = false;
}

//-----------------------------------------------------------
//------------SETTINGS---------------------------------------
//-----------------------------------------------------------

boolean osc_routeAppStationSet(OSCMessage &msg, int addr)
{
  DEBUG_PRINT(F("routing app setters"));

 if (msg.match("/address", addr))
    DEBUG_PRINT("matched address here");
  
  if (msg.route("/singleShot", osc_setSingleShot, addr))
    return true;
  if (msg.route("/address", osc_setAddress, addr))
     return true;

  if (msg.route("/trigger", osc_setTrigger, addr))
    return true;

   return false; //nothing processed.
}

void osc_setSingleShot(OSCMessage &msg, int addr)
{
  
}
void osc_setAddress(OSCMessage &msg, int addr)
{
  DEBUG_PRINT("SETTING ADDRESS");
  char tAddr[msg.getDataLength(0)];
  msg.getString(0, tAddr, msg.getDataLength(0));

  switcher_address = String(tAddr);

  switcher_ready_address = switcher_address;

  DEBUG_PRINT(switcher_address);
}

void osc_setTrigger(OSCMessage &msg, int addr)
{
  
}
//-----------------------------------------------------------
//------------GETTERS----------------------------------------
//-----------------------------------------------------------
boolean osc_routeAppStationGet(OSCMessage &msg, int addr)
{

    return false; //nothing processed
}

//-----------------------------------------------------------
//------------OUTPUT----------------------------------------
//-----------------------------------------------------------
void osc_sendTrigger(byte state)
{
   if (!switcher_enabled)
      return;
      
   if (switcher_isSingleShot)
      switcher_enabled = false;
      
   if (switcher_ready_address.length()==0)
      return; //nothing to send..

   OSCMessage msg(switcher_ready_address.c_str());

   sendOSC(msg);

   msg.empty();

}




