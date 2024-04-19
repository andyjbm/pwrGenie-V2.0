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
         float leq60sec;
         float leq5;
         float leq15;
         float leq5IsValid;
         float leq15IsValid;
      };

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
      void add2leq(uint16_t db);

      // Somewhere to translate the received 7 segment pattern into a numeric Digit.
      struct segDecode{
         uint8_t seg;
         uint8_t val;
      };

      // This is a seven segment display bitmap representation of each digit.
      // Each one is eight bits: bgcXafed  where X is decimal point. (Its always present for units but not the other digits.)
      
      // Val & 0x0f is the value, 0x10 flag indicates dp present..
      const segDecode volatile sshashes[21] = {
         {0b10101111, 0}, {0b10100000, 1}, {0b11001011, 2}, {0b11101001, 3}, 
         {0b11100100, 4}, {0b01101101, 5}, {0b01101111, 6}, {0b10101000, 7},
         {0b11101111, 8}, {0b11101101, 9},
         {0b10111111, 0x10}, {0b10110000, 0x11}, {0b11011011, 0x12}, {0b11111001, 0x13}, 
         {0b11110100, 0x14}, {0b01111101, 0x15}, {0b01111111, 0x16}, {0b10111000, 0x17},
         {0b11111111, 0x18}, {0b11111101, 0x19}, 
         {0b00000000, 0x20}   // This represents hundreds position for numbers => 99.9 when the 100s is off (not lit).
         };
      
      volatile uint8_t sevenseg[4];
      volatile bool read_started = false;
      volatile bool SPL_Complete = false;
      volatile unsigned long thistick = 0;
      IRAM_ATTR void cb_readClock();

      bool digitNotFound = false;
      bool dpWrongPlace = false;
      bool noDigitWrongPlace = false;

      //leq instance IDs
      uint8_t leq60sec;
      uint8_t leq5;
      uint8_t leq15;

      bool reset30sec = false;
      String LEQInfo = "";

      void begin();
      void isr_reset();
      void ssreaderLoop();
      void DoSPLSend();
   };  // End namespace ssreader.

   // Definitions
   void ssreader::begin() { // "static" class so using namespace and begin instead of ctor.

      // Initialise some LEQ instances (in seconds)...
      leq60sec = leq::newLEQ(60);
      leq5     = leq::newLEQ(60 * 5);
      leq15    = leq::newLEQ(60 * 15);

      // make safe while we config the hardware.
      SPL_Complete = true;
      read_started = true;  
      
      pinMode(SPL_datapin,INPUT);
      pinMode(SPL_clockpin,INPUT);
      attachInterrupt(digitalPinToInterrupt(SPL_clockpin), ssreader::cb_readClock, FALLING);

      //ready to go.
      isr_reset();     
   }

   // This sets up the isr for a packet read.
   // The isr will then wait till the foreground code has decoded and called this reset again.
   void ssreader::isr_reset(){
      sevenseg[0] = 0;
      sevenseg[1] = 0;
      sevenseg[2] = 0;
      sevenseg[3] = 0;
      
      thistick = millis();
      SPL_Complete = false;
      read_started = false;  
   }

   // Callback on clock rising edges. This is where the magic happens.
   IRAM_ATTR void ssreader::cb_readClock(){

      volatile static uint8_t bitpower;
      volatile static uint8_t digit;
      volatile unsigned long lasttick;

      if (!read_started){

         //We're looking for a reeeeeeally long high (>80ms ish) (High to low transition)
         //to indicate the gap between end of the last packet and the start of this one.
         //Note if the foreground code has to wait & takes a while decoding we could miss a packet or two.
         lasttick = thistick;
         thistick = millis();
         if (thistick-lasttick > SPL_deadspace) {
            // This is the 1st clock. Start of data packet. Reset everything before we begin.
            bitpower = 0;
            digit = 0;
            
            // First edge detected - off we go.
            read_started = true;
         }
      } else {
         //read_started == true, we are reading data for real....
         if (!SPL_Complete) {
            if ((bitpower > 99) && (bitpower < 132)){ // 32 bits total
               // Digit time... read in the seven segment encoded bit pattern.
               // Each one is eight bits: bgcXafed  where X is decimal point. (It's always present for units but not the other digits.)
               // We start at bit 100 in the datastream... Bits 103, 111, and 119 will be the decimal points.
               // Clumsy method, We could just use an uint32_t, (rather than an array[digit]) & read all the bits straight in but it makes decoding to digits easier later.
               sevenseg[digit] = (sevenseg[digit] << 1) + digitalRead(SPL_datapin);
            }
            bitpower ++;
            if ((bitpower == 108) | (bitpower == 116) | (bitpower == 124)){ digit ++; } // Next digit
            if (bitpower >= 132) {SPL_Complete = true;}  // We've finished. The ISR can stop responding and the foreground loop can do the decode.
         }
      }
   } // End of ssreader::cb_readClock()

   //This is executed in the foreground and should be called by main::loop()
   //We check for a complete reading here and do all the finishing off here rather than in the ISR...
   //In fast mode this is not guaranteed to complete before the next data packet
   //so they'll be lost until this one is decoded and the foreground resets the isr.
   void ssreader::ssreaderLoop(){

      if (!(read_started && SPL_Complete)) return; //Check for read completing, quit if we're not ready.
      
/*
      std::string str ="";
      for (byte digit=0; digit<4; digit++){ 
         str += std::bitset<8>(sevenseg[digit]).to_string();
         str += ".";
      }
      CONSOLELN(str.c_str());
*/

      //Got one, now we can decode.
      dpWrongPlace = false;
      uint16_t splval = 0;
      uint8_t digit;
      for (digit=4; digit-- > 0;){            // For each digit...*NOTE* digit counts from 3 to zero! NOT 4 to 1.
         splval *= 10;                                // Next digit.
         digitNotFound = true;
         for (uint8_t i=0; i<21; i++){                // Scan all possible 7 seg patterns for a match.
            if (sevenseg[digit] == sshashes[i].seg){  // Convert the 7 seg into its digit value.
               splval += (sshashes[i].val & 0xf);     // We want the value excl dp indicator.
               digitNotFound = false;

               // Check dp is only present for digit 2. eg: 101.3
               bool dp = ((sshashes[i].val & 0x10) == 0x10);  // Get the dp present flag.
               dpWrongPlace = (dp ^ (digit == 1)); // dp xor. Only units digit has decimal point.
               bool nd = ((sshashes[i].val & 0x20) == 0x20);  // Get the digit off flag.
               noDigitWrongPlace = (nd ^ (digit == 3)); // Only the hundreds digit is allowed to be off.
               break; // Save time.
            }
         }
         if (digitNotFound) break; // We didn't identify this one... Bit pattern error.
      }
      
      // Reset 30 sec + 10 sec for connecting to wifi after startup to allow for meter handling and placement
      if (!reset30sec) {
         if (millis() > 40000) {
            reset30sec = true;   // Don't reset again this lifetime.
            leq::resetAll();
         } 
      }

      static String isrReadError = "";
      static uint16_t isrREcount = 0;
      if (digitNotFound || dpWrongPlace || noDigitWrongPlace || !(splval > 0 && splval < 1501)){
         isrREcount++;   
         isrReadError = "[{m}]: count:{c}, dnf:{dnf}: digit:{d}, dpwp:{dpwp}, ndwp:{ndwp}, spl:[{spl}], 7seg:<{ss3}, {ss2}, {ss1}, {ss0}>";
   
         isrReadError.replace(FPSTR("{m}"),     String(millis()));
         isrReadError.replace(FPSTR("{c}"),     String(isrREcount));
         isrReadError.replace(FPSTR("{dnf}"),   String(digitNotFound));
         isrReadError.replace(FPSTR("{d}"),     String(digit));
         isrReadError.replace(FPSTR("{dpwp}"),  String(dpWrongPlace));
         isrReadError.replace(FPSTR("{ndwp}"),  String(noDigitWrongPlace));
         isrReadError.replace(FPSTR("{spl}"),   String(splval));

         isrReadError.replace(FPSTR("{ss3}"), String(sevenseg[3]));
         isrReadError.replace(FPSTR("{ss2}"), String(sevenseg[2]));
         isrReadError.replace(FPSTR("{ss1}"), String(sevenseg[1]));
         isrReadError.replace(FPSTR("{ss0}"), String(sevenseg[0]));
         CONSOLELN(isrReadError);
      }

      
      isr_reset();  // Done decoding. Setup the skittles ready for the next data packet.

      // Quit if outside range or something went wrong with decoding.
      if (digitNotFound || dpWrongPlace || noDigitWrongPlace) return;
      if (!(splval > 0 && splval < 1501)) return;

      // splval is now = dB * 10   
      if (isrReadError.isEmpty()) {
         LEQInfo = "";
      } else {
         LEQInfo = isrReadError + FPSTR("<br>");
      }
      LEQInfo += leq::addVal(splval);  // Add the new SPL to all leq instances. Return is error info.

      #if 0
         //CONSOLE(F("SPL= ")); CONSOLE(db);
         leq::leqArray[leq60sec]->printInfo();
         leq::leqArray[leq5]->printInfo();
         leq::leqArray[leq15]->printInfo();
      #endif
   }

   // Called by foreground loop every x seconds to send data to emoncms.
   void ssreader::DoSPLSend() {
      splData splD;
      splD.leq60sec = leq::leqArray[leq60sec]->read();
      splD.leq5 = leq::leqArray[leq5]->read();
      splD.leq15 = leq::leqArray[leq15]->read();

      splD.leq5IsValid = (leq::leqArray[leq5]->isValid ? 1 : 0);
      splD.leq15IsValid = (leq::leqArray[leq15]->isValid ? 1 : 0);

      emoncms::send2emoncms(splDataNames, (float *) &splD, splDataElementCount);
   }
#endif