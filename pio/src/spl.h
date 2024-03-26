#pragma once
#ifndef SPL_H
   #define SPL_H

   #include <Arduino.h>
   #include <ESP.h>
   #include <string.h>
   #include <bitset>
   #include "leqv2.h"
   #include "emoncms.h"

   const uint8_t splDataElementCount = 5;
   //The names of the data values for emoncms fields.
   const char namedb[] PROGMEM = "spl";      // This is actually a 10 second LEQ
   const char nameLEQ5[] PROGMEM = "LEQ5";
   const char nameLEQ15[] PROGMEM ="LEQ15";

   const char nameLEQ5Valid[] PROGMEM = "LEQ5Valid";    
   const char nameLEQ15Valid[] PROGMEM = "LEQ15Valid";

   // An Array of the above.
   const char * const splDataNames[splDataElementCount] = {
      namedb, nameLEQ5, nameLEQ15,
      nameLEQ5Valid, nameLEQ15Valid
   };

   // SPL LEQ results:
   struct splData {
         float leq10sec;
         float leq5;
         float leq15;
         float leq5IsValid;
         float leq15IsValid;
      };

   splData splD;

   #if defined (ARDUINO_ARCH_ESP8266)
      #define  SPL_datapin         D2   //D2 GPIO4
      #define  SPL_clockpin        D1   //D1 GPIO5
   #elif defined(ESP32)
      //Not sure this is right. Never built a SPL meter with ESP32 yet
      #define  SPL_datapin         2   //D2 GPIO4
      #define  SPL_clockpin        1   //D1 GPIO5
   #else
      #error Architecture unrecognized by this code.
   #endif

   #define  SPL_deadspace       80   // mS Time between datapackets

   // Declarations
   namespace ssreader{

      void DoSPLSend();
      void begin();
      IRAM_ATTR void cb_readClock();
      void ssreaderLoop();
      void add2leq(float db);

      // Somewhere to translate the received 7 segment pattern into a numeric Digit.
      struct segDecode{
         byte seg;
         byte val;
      };

      // This is a seven segment display bitmap representation of each digit.
      const segDecode sshashes[11] = {
         {0b01011111, 0}, {0b01010000, 1}, {0b01101011, 2}, {0b01111001, 3}, 
         {0b01110100, 4}, {0b00111101, 5}, {0b00111111, 6}, {0b01011000, 7},
         {0b01111111, 8}, {0b01111101, 9}, {0b00000000, 0}
         };
      
      volatile bool read_started = false;
      volatile bool SPL_Complete = false;
      volatile unsigned long thistick;
      volatile unsigned long lasttick;
      volatile byte sevenseg[4];
      IRAM_ATTR void cb_readClock();

      //leq instance IDs
      byte leq10sec;
      byte leq5;
      byte leq15;

      // For resetting 30 sec after powerup.
      unsigned long creationtime;
      bool reset30sec = false;

      void begin();
      void ssreaderLoop();
      void add2leq(float db);
      void DoSPLSend();
   };  // End namespace ssreader.

   // Definitions

   void ssreader::begin() { // "static" class so using namespace and begin instead of ctor.

      // Initialise some LEQ instances (in seconds)...
      leq10sec = leq::newLEQ(10);
      leq5 = leq::newLEQ(60 * 5);
      leq15 = leq::newLEQ(60 * 15);
      creationtime = millis();

      read_started = false;
      SPL_Complete = false;
      thistick = millis();
      lasttick = thistick;
      
      pinMode(SPL_datapin,INPUT);
      pinMode(SPL_clockpin,INPUT);
      attachInterrupt(digitalPinToInterrupt(SPL_clockpin), ssreader::cb_readClock, FALLING);
   }

   // Callback on clock rising edges. This is where the magic happens.
   IRAM_ATTR void ssreader::cb_readClock(){

      static byte bitpower;
      static byte digit;
         
      if (!read_started){

         lasttick = thistick;
         thistick = millis();

         //We're looking for a reeeeeeally long high (80ms ish) (High to low transition)
         //to indicate the end of the last packet.
         if (thistick-lasttick > SPL_deadspace) {
            // This is the 1st clock. Start of data packet. Reset everything before we begin.
            bitpower = 0;
            digit = 0;
            sevenseg[0] = 0;
            sevenseg[1] = 0;
            sevenseg[2] = 0;
            sevenseg[3] = 0;
            
            // First edge detected - off we go.
            read_started = true;
            SPL_Complete = false;
         }
      } else {
         //read_started == true, we are reading data for real....
         if (!SPL_Complete) {
            if ((bitpower > 99) && (bitpower < 132) &&        // 32 bits total
               (bitpower != 103) && (bitpower != 111) &&   // Decimal point positions. 
               (bitpower != 119) && (bitpower != 127)){
               
               // Digit time... read in the seven segment encoded bit pattern.
               // Each one is eight bits: bgcXafed  where X I'm guessing would be a decimal point. (We'll never know, it doesn't move.)
               // We start at bit 100 in the datastream... Bits 103, 111, and 119 will be the decimal points.
               sevenseg[digit] = (sevenseg[digit] << 1) + digitalRead(SPL_datapin);
            }
            bitpower += 1;
            if ((bitpower == 108) | (bitpower == 116) | (bitpower == 124)){ digit += 1; } // Next digit
            if (bitpower >= 132) {SPL_Complete = true;}  // We've finished. The ISR can stop responding and the foreground loop can do the decode.
         }
      }
   } // End of ssreader::cb_readClock()

   //This is executed in the foreground and should be called by main::loop()
   //We check for a complete reading here and do all the finishing off here rather than in the ISR...
   void ssreader::ssreaderLoop(){
      //Check for timeout. 
      if (read_started && SPL_Complete){
            //Timeout triggered - we are done. Now we can decode.
            read_started = false;  
            SPL_Complete = false;
   /*
            std::string str ="";
            for (byte digit=0; digit<4; digit++){ 
               str += std::bitset<8>(sevenseg[digit]).to_string();
               str += ".";
            }
            CONSOLELN(str.c_str());
   */
            unsigned int splval = 0;
            for (byte digit=4; digit-- > 0;){      // For each digit...
               splval *= 10;                       // Next digit.
               for (byte i=0; i<11; i++){          // Scan all possible 7 seg patterns for a match.
                  if (sevenseg[digit] == sshashes[i].seg){ // Convert the 7 seg into its digit value.
                     splval += sshashes[i].val;
                     break; 
                  }
               }  
            }
            add2leq(splval / 10.0);  // Implicit conversion to float   
      }
   }

   void ssreader::add2leq(float db){
      //static float lastdb;
      // In fast mode the spl meter free runs the display quite fast even if the meter's
      // adc hasn't finished a new conversion. We need a way to send timely samples.
      // if (lastdb == db){ return; } 
      // lastdb = db;

      // Reset 30 sec after startup to allow for meter handling and placement
      if (!reset30sec) {
         if (millis() - creationtime > 30000) {
            reset30sec = true;   // Don't reset again this lifetime.
            leq::resetAll();
         } 
      }

      leq::addVal(db);  // Add the new SPL to all leq instances.

      //CONSOLE(F("SPL= ")); CONSOLE(db);
      
      leq::leqArray[leq10sec]->printInfo();
      leq::leqArray[leq5]->printInfo();
      leq::leqArray[leq15]->printInfo();
   }

   // Called by foreground loop every x seconds to send data to emoncms.
   void ssreader::DoSPLSend() {
      splD.leq10sec = leq::leqArray[leq10sec]->read();
      splD.leq5 = leq::leqArray[leq5]->read();
      splD.leq15 = leq::leqArray[leq15]->read();

      splD.leq5IsValid = (leq::leqArray[leq5]->isValid() ? 1 : 0);
      splD.leq15IsValid = (leq::leqArray[leq15]->isValid() ? 1 : 0);

      emoncms::send2emoncms(splDataNames, (float *) &splD, splDataElementCount);
   }
#endif