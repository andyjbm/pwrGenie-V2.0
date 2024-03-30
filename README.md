# Welcome to pwrGenie V2.0
This will soon be full of useful info on how this all works.

Honest...

It is currently a WORK IN PROGRESS. There are no guarantees (and probably never will be).
You are welcome to use at your own risk etc.

For now there's some pretty pics in the pics folder and some example firmwares in the releases folder.

Important Notes so far:
    Espressif Framework V6.5.0 now working aok.

    board d1-mini-esp32 added to environment.
    
    ESP8266 is now on the latest espressif8266 framework v4.2.1. It compiles and appears to run ok although I have not tested for any length of time yet.
    It does NOT work with JKBMS type. There is just not enough RAM available without optimizing. I am not sure how achieveable this will be as yet.

    A note about ESP-s2-mini:
    This board has given me nothing but trouble. I've finally got it working but to a point where it reboots every couple of hours. I've added a d1-mini-ESP32 and so far this has been far more reliable.
    It could be my s2-mini is a clone or a dud or it could be the nature of the s2.
    
    For now I'm going to continue to develop on the d1-mini-ESP32 but also try and optimize space to get JKBMS to fit on an ESP8266.
    
Fixed:
    ESP32-s2-mini, ESP32doit-devkit-v1 now work on V6.5.0! Issue with calling wifi.mode in ASync_WifiManager constructor. Not a good place to do that.

Fixed:
    Issue where esp32-s2-mini would not run unless it was connected to serial monitor over usb. It now just works. on a battery pack, or PC usb without serial monitor open. The cause was: while (!Serial){} in setup() preventing the sketch from moving forward. 

    Issue where it required a hard reset to reconnect to wifi after a soft reboot (after flashing or otherwise). This was a hardware bug with the RS485 level shifter lifting the ESP32 Rx pin higher than 3v3 (to 3v9). A zener clamp to 3v0 fixed this. Now a soft reboot (after OTA flash, or reboot from webpage) or reboot from uploading from platformio will reboot and connect to wifi without intervention.