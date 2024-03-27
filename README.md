# Welcome to pwrGenie V2.0
This will soon be full of useful info on how this all works.

Honest...

It is currently a WORK IN PROGRESS. There are no guarantees (and probably never will be).
You are welcome to use at your own risk etc.

For now there's some pretty pics in the pics folder and some example firmwares in the releases folder.

Important Notes so far:
    Espressif Framework V6.5.0 now working aok!
    
    WIP: Getting ESP8266 onto latest frameworks.
    I compiled ESP8266 with espressif8266 v3.30102.0. I have not tested the latest V4.2.1 at time of writing.
    This is NOT yet hard referenced in platformio.ini yet but if you are not seeing anything try V3.3.0

Fixed:
    ESP32-s2-mini, ESP32doit-devkit-v1 now work on V6.5.0! Issue with calling wifi.mode in ASync_WifiManager constructor. Not a good place to do that.

Fixed:
    Issue where esp32-s2-mini would not run unless it was connected to serial monitor over usb. It now just works. on a battery pack, or PC usb without serial monitor open. The cause was: while (!Serial){} in setup() preventing the sketch from moving forward. 

    Issue where it required a hard reset to reconnect to wifi after a soft reboot (after flashing or otherwise). This was a hardware bug with the RS485 level shifter lifting the ESP32 Rx pin higher than 3v3 (to 3v9). A zener clamp to 3v0 fixed this. Now a soft reboot (after OTA flash, or reboot from webpage) or reboot from uploading from platformio will reboot and connect to wifi without intervention.