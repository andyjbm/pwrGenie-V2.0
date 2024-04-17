# Welcome to pwrGenie V2.0
This will soon be full of useful info on how this all works.

Honest...

It is currently a WORK IN PROGRESS. There are no guarantees (and probably never will be).
You are welcome to use at your own risk etc.

For now there's some pretty pics in the pics folder and some example firmwares in the releases folder.

Important Notes so far
======================
You will need to open the Secrets_h.example file, put your emoncms APIKEYS in there adn rename it to Secrets.h for the source to compile. See the comments in the file.

Still in testing:
    Nothing atm.

Working:
    Refactor to spl and leq to allow the splmeter to run in fast mode without breaking the data.
    Implemented float16 library (see credits) in leq to be able to use a bigger leq buffer for the same RAM.

    modbus working properly with softwareserialisr now. Lots of refactoring to make it easier to add new modbus devices and protocols.
    JKBMS, SPL Meter, on the following boards:

    ESP32 boards:
        d1-mini-esp32
        ESP32doit-devkit-v1
        ESP32-S2-mini (mine is a bit flakey (reboots every 2-3hrs) but I think that's because it's a chinese knock off. Every other board I've tested has been stable.)
    
    ESP8266 boards: 
        lolin ESP8266 d1-mini
    
    Espressif32 Framework V6.5.0 now working aok.
    Espressif8266 Framework v4.2.1 aok.


Fixed:
    So JKBMS was still not working 100% on ESP8266. I found a NEW SoftwareSerial library working on the edge-triggered ISR principle and saving a whopping 6k of heap RAM over the ESP8266 & plerup/EspSoftwareSerial libs. Can be found here: https://github.com/CrazyRobMiles/Esp8266EdgeSoftwareSerial

    JKBMS is definately now working on ESP8266 for over 9hrs solid without a crash & restart.

    JKBMS now working on ESP8266. The issue was SoftwareSerial sucking 10kB RAM for the buffer. Fixed.

    ESP32-s2-mini, ESP32doit-devkit-v1 now work on V6.5.0! Issue with calling wifi.mode in ASync_WifiManager constructor. Not a good place to do that.

    Issue where esp32-s2-mini would not run unless it was connected to serial monitor over usb. It now just works. on a battery pack, or PC usb without serial monitor open. The cause was: while (!Serial){} in setup() preventing the sketch from moving forward. 

    Issue where it required a hard reset to reconnect to wifi after a soft reboot (after flashing or otherwise). This was a hardware bug with the RS485 level shifter lifting the ESP32 Rx pin higher than 3v3 (to 3v9). A zener clamp to 3v0 fixed this. Now a soft reboot (after OTA flash, or reboot from webpage) or reboot from uploading from platformio will reboot and connect to wifi without intervention.


Credits:
    Thanks to the following libs used in various forms of modification:

    ESPASync_WiFimanager:    https://github.com/khoih-prog/ESPAsync_WiFiManager
    jk-bms:                  https://github.com/ArminJo/JK-BMSToPylontechCAN
    SoftwareSerial:          https://github.com/CrazyRobMiles/Esp8266EdgeSoftwareSerial
    modbus-esp8266:          https://registry.platformio.org/libraries/emelianov/modbus-esp8266                  
    ArduinoJson:             https://registry.platformio.org/libraries/bblanchon/ArduinoJson
    float16:                 https://github.com/RobTillaart/float16
    getFrameworkVersions.py  Anon from somewhere - StackOverflow possibly... I'm still searching my google history!
Apologies to those I've missed, they will be added as and when I realise!
