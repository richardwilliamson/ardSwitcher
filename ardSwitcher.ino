#define DEBUG

#include "ardSwitcher.h"
#include "osc.h"
#include "persistence.h"
#include "config.h"

byte lastState;

void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);
  pinMode(LED_PING, OUTPUT);
  pinMode(LED_ERROR, OUTPUT);
  pinMode(INPUT_1, INPUT_PULLUP);
  
  digitalWrite(LED_PING, LED_OFF);
  
  //if (LED_RED != PIN_ACKNOWLEDGE)
      
  readAll(); //read in settings from memory
  launchWifiManager(false); //false because we don't want to force the AP to appear..


  udp.begin(1000);

  //by default listens on multicast
  DEBUG_PRINT(F("started"));
  DEBUG_PRINT(WiFi.localIP());
  DEBUG_PRINT(WiFi.SSID());

  DEBUG_PRINT(F("DONE SETUP"));

  //FLash LEDs 4 times

  long last = millis()-400; //so we come on the first time
  bool ison=false;
  byte count=0;

  bool isHigh = false;

  while (count<4)
  {
     if (millis()-last > 300)
     {
        isHigh != isHigh;

        digitalWrite(LED_PING, (isHigh?LOW:HIGH));
        digitalWrite(LED_ERROR, (isHigh?LOW:HIGH));
        count++;
        last=millis();
     }
  }

  lastState = digitalRead(INPUT_1);
  
  
}

long changeTime = 0; //the time we sensed a change
int debounceTime = 200; //ms to wait to check it's changed


void loop() {
  // put your main code here, to run repeatedly:
  // wifiManager.runLoop(); //listen for connections..

  checkOSC(); //check and process any incoming OSC

  //now look and see what's what..

 
  if (switcher_enabled)
  {
    byte state = digitalRead(INPUT_1);

    if (state!=lastState)
    {
      if (changeTime = 0)
      { //haven't had a change yet
        changeTime = millis();
        DEBUG_PRINT("CHANGE STATE WAIT");
      } else if(millis() - changeTime >debounceTime)
      {
         //has changed for 200ms 
         if (switcher_trigger== SWITCHER_TRIGGER_ANY || (switcher_trigger==SWITCHER_TRIGGER_ON && state == HIGH) || (switcher_trigger == SWITCHER_TRIGGER_OFF && state == LOW))
         {
            osc_sendTrigger(state);
            DEBUG_PRINT("CHANGE STATE TRANSMIT");
         }

         DEBUG_PRINT("STATE NOW CHANGED");

         lastState = state;
         changeTime = 0;
      }
    } else
    {
      changeTime = 0;
    }
  }
  
  
}


void setArgument(String arg)
{
  switcher_ready_address = switcher_address;

  switcher_ready_address.replace("{ARG}", arg);

  DEBUG_PRINT(switcher_ready_address);
  
}




