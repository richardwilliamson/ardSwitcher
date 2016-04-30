# ardSwitcher

An OSC system using ESP8266 chips, arduino IDE for sending OSC when a switch changes state

pin definitions in ardQlight.h

Devices listen to broadcast messages via udp - ie xx.xx.xx.255 - on port 1000, they return messages to the IP address specified by the last /registerMaster which allows iPads etc to respond

Devices are assigned a station between 1 and 255 (default is 255), can be assigned a group and a name (group and name are not yet implemented)

**requires my fork of wifiManager.h which adds some functionality**

#operation

You can send a ping command which will flash an onboard blue LED, and reply with /ping which means you can keep a heartbeat running and know that the device is communicating


##special OSC commands (recieved)
**/ardSwitcher/registerMaster** this tells the units to change the destination for all of their output messages (see output section) so should be used by, for example, a touch OSC layout - there can only be one master on the network

##Standard OSC commands (recieved)

Devices can be contacted in a number of ways:

**/ardSwitcher/all/..** contacts all devices on the network - useful for polling, testing, flashing and clearing

**/ardSwitcher/station/XXX/..** contacts the station with the specified number - note that there is nothing to stop there being more than one station with a specified number on the network, however currently the behaviour when acknowledging standbys is undefined

**/ardSwitcher/unit/XXX/..** contacts the station with the specified unique ID, which is the serial number of the onboard chip - this can be viewed in the web config, or queried with ../get/unitID

**/ardSwitcher/group/XXX/...** contacts all stations assigned to the specified group - note that this is not yet implemented

###The following commands can be sent using the above prefixes:###

**../ping** cause the on board (currently blue) LED to flash, and the device to respond to the master with ../ping


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
  // /OSCAddress "A/STRING/WITH/AN/{ARG}" - the wildcard {ARG} will be replaced with an argument sent with the /enable
  // /trigger "onOn", "onOff", "onStateChange"

#output

The unit outputs information to the master (specified by /registerMaster) when certain actions occur, to allow the master to update according to the status of the devices on the network

Monitoring devices can request an update by using ../poll for either all or a selection of units

all are outputted in the form:

**/ardSwitcher/out/station/XXX/...** where XXX is the station number - remember that there can be more than one station with a specific number

**../unitID (String unitID)** when polled or requested returns a String containing the unique ID of the unit 

**../ping** the response to a ping request

##setup osc

By default a station will be number 255

As with the normal methods you can access units via /unit, /station, /group or /all

**../poll** polls the specified devices and requests their status - they respond with the status of their lamps (on or off) and their unitID as a string (see below)

**../set/station (int newStation)** send an int between 1 and 255 and the unit will change it's station number to that number

**../set/stationName (String new Name)** send a String (max 16 chars) with the new name for this station

**../set/sendPort (int port)** sets the port that the unit will send information back out on - useful with /all/... when configuring the system 

to recieve settings use the corresponding getter:

**../get/stationName** replies with a String containing the station name for that unit

**../get/unitID** prompts the unit to respond with a String containing it's unique unit ID - useful with /all when adding units to the network

**../get/IP** responds with the IP address of the unit

**../get/sendPort** responds with the port that the unit sends its output onto

**../get/version** responds with the version of the ardQ firmware that the device is running

##manual setup

Many features of the device can also be configured via manual setup through a web page over the device's own access point (when activated)

If a device powers up and cannot find it's last wireless network it automatically launches the access point and both onboard LEDs will flash alternately. Once launched you simply point your phone or computer to the wireless network (named ArdQXXXXX where XXXXX is the unit ID for the device) and use the password '12345678' - the web page should launch automatically, otherwise navigate to 192.168.4.1 and it should appear.

You can also launch this accesspoint manually by holding down both red and green buttons for 1 second - the lamps will start flashing and off you go.

To exit the access point press and hold both buttons again for one second and the device will return to normal operation (although maybe still with no Wifi!)

new firmware can be uploaded to the unit by using the relevant page - note that this doesn't work with the access portal, you must navigate using a traditional browser to the unit's IP address - either on the captive portal (192.168.4.1) or, if enabled, over main wifi network

to enable/disable access to the web config through the main network send the following OSC (to either ../all/.. or specific unit(s) ):

**../enableWebConfig (int 0 or 1)** where 0 disables, and 1 enabled - you can then access the unit from a web browser on a machine on the same network using the unit's IP address (which you can find out using ../get/IP)

Note that it is not advisable to have web config enabled during performances, as additional load is placed on the units. 


###TODO
