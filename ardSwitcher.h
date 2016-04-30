#ifndef ARDSWITCHER_h
#define ARDSWITCHER_h
#include <OSCMessage.h>
#include <ESP8266WiFi.h>
#include <WiFiUDP.h>
#include <WiFiManager.h>

const char VERSION_NUMBER[] PROGMEM  = "0.01";


WiFiUDP udp;

#define SWITCHER_TRIGGER_ON 1
#define SWITCHER_TRIGGER_OFF 0
#define SWITCHER_TRIGGER_ANY 255


#ifdef DEBUG
 #define DEBUG_PRINT(x)  Serial.println (x)
 #define DEBUG_DELAY delay(1000)
#else
 #define DEBUG_PRINT(x)
 #define DEBUG_DELAY
#endif


//#define IS_THING

#define LED_OFF HIGH
#define LED_ON LOW

#define INPUT_1 4//pin to listen for input on
#define LED_PING 2 //blue LED 
#define LED_ERROR 0 //red LED
#define PIN_RESET 0


//OPERATIONAL STUFF

boolean switcher_enabled = false; //not persistent

boolean switcher_isSingleShot = true; //should be persistent but can be overriden

String switcher_address = "";

String switcher_ready_address = ""; //this is the address once any argument has been inserted..

byte switcher_trigger = SWITCHER_TRIGGER_ANY; //should be persistent but can be overriden

void setArgument(String arg);
 

#endif

