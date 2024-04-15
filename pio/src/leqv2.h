/* LEQ Library.
   This was refactored so that now we only need one buffer for any number of leq instances.
   The buffer only has to be as big as the largest LEQ requires as there is no point in storing
   all the buffered data that is common to all LEQs more than once!

   Each leq instance keeps track of its own average and time sums, its own Buffer Out Index 
   its own number of samples and its own valid flag.
*/

#pragma once
#ifndef LEQV2_H
   #define LEQV2_h

   #include <Arduino.h>
   #include "defs.h"
   #include "float16.cpp"

   //#define LEQ_DEBUG

   #define MAX_LEQS 5               // Default max number of LEQ instances allowed.
   #define MAXLEQSIZE 15 * 60       // Default max LEQ Size in seconds. Default is 15 minutes.

   namespace leq {
      // "Static" vars global to each leq instance.
      
      // Buffer indexes & sizes
      uint16_t BufInIndex;
      const uint16_t maxBufferSize = MAXLEQSIZE * 2.15; // Max array size. Not a define so that the calc only happens once and here.
      const float float16scale = 1526624;  // For getting dB linear to fit into a float16. Effectively sets max float16 to 150dB. scale = POWER(10,E1) where E1 = 14-3-LOG10(65504)  

      //These buffers are to keep track of the time between samples. By summing these samples 
      //we can determine how many samples we need for a particular leq period.
      //This approach compensates for drift over time as the number of samples required may change as sample duration changes.
      
      uint16_t timeBuffer[maxBufferSize];
      float16 LEQbuffer[maxBufferSize];    // Fits in 2 bytes per index. (uint16_t base)

      class avgLEQ{

         private:
            uint32_t timeSum;
            float avgSum;
            uint32_t LEQSizeInMillis;   // *ToDo Can this be just int?
            uint16_t BufOutIndex;
            uint16_t LEQSampleCount;     // Number of active samples for average calculation.

            #ifdef LEQ_DEBUG
               bool leqDebug = false;
            #endif

         public:
            static bool bufferOverflow;         // Catch for not pressing the SLOW button on the SPL meter!
            bool isValid;
            
            avgLEQ(uint16_t _LEQSizeInSec){
               #ifdef LEQ_DEBUG
                  if (_LEQSizeInSec == 10) leqDebug = true;
                  CONSOLE(F(", Hello from avgLEQ..._LEQSizeInSec: ")); CONSOLELN(_LEQSizeInSec);
               #endif
               LEQSizeInMillis = _LEQSizeInSec * 1000; 
               resetLEQ();
            }

            void addval(float16 newLinSPL, uint16_t sampleTime){
      
               // The sum needs to be float as this could potentially exceed the limit of float16.
               // We use float16 on the new value to deliberately loose precision to that of the buffer value so that 
               // the sum will be correctly reduced by the same amount when this value leaves the buffer.
               avgSum += newLinSPL.toDouble();         // avgSum is sum of scaled linear spl values.
               timeSum += sampleTime;                  // time between last and this reading.
               LEQSampleCount ++;                      // Unique to this instance. But the global buffInIndex is bumped up outside of the class.
            
               if (isValid){
                  // The focus is that the sum of the duration of the samples = (approx) the size of the LEQ time.
                  // The absolute number of samples required to achieve this is not relevant but is needed to calculate the average LEQ spl.
                  // Here we drop values from the end of the buffer until the sum of sample durations is close to the LEQ duration.
                  // This allows drift in samples & sample times from the meter wrt real time from millis()
                  // Note that this means LEQSampleCount will *change* over time if it needs to to fit the LEQ time window.
                  while (timeSum > LEQSizeInMillis){
                     #ifdef LEQ_DEBUG
                        if (leqDebug) {
                           CONSOLE(F("timeSum: ")); CONSOLE(timeSum);
                           CONSOLE(F(", timeBuffer: ")); CONSOLE(timeBuffer[BufOutIndex]);
                           CONSOLE(F(", BufOutIndex: ")); CONSOLE(BufOutIndex);
                           CONSOLE(F(", BufInIndex: ")); CONSOLE(BufInIndex);
                           CONSOLE(F(", maxBufSize: ")); CONSOLE(maxBufferSize);
                           CONSOLE(F(", LEQSampleCount: ")); CONSOLE(LEQSampleCount);
                        }
                     #endif
                     // NOT inside leqdebug. This will print for all leq.
                     if (timeSum < timeBuffer[BufOutIndex]){  //Make sure we won't break anything.
                        // Subtracting this buffer value would make timeSum go -ve.
                        // If the buffer is working properly this should not happen. How did we get here?
                        CONSOLE(F("\nCRASH: ")); 
                        CONSOLE(F(", BufOutIndex: ")); CONSOLE(BufOutIndex);
                        CONSOLE(F(", BufInIndex: ")); CONSOLE(BufInIndex);
                        CONSOLE(F(", maxBufSize: ")); CONSOLELN(maxBufferSize);
                        break;
                     } else {
                        // Roll the oldest out of the end of the buffer & remove from the average totals.
                        avgSum  -= LEQbuffer[BufOutIndex].toDouble();
                        timeSum -= timeBuffer[BufOutIndex];
                        BufOutIndex = (BufOutIndex+1) % maxBufferSize;
                        LEQSampleCount --;
                     }
                  }
                  #ifdef LEQ_DEBUG
                     if (leqDebug){CONSOLELN();}
                  #endif
               
               } else {

                  // Look for achieving a complete LEQ buffer
                  if (timeSum > LEQSizeInMillis){ // We are there.
                     isValid = true;
                  }
                  if (LEQSampleCount >= maxBufferSize) { //Oh dear. Buffer not big enough.
                     bufferOverflow = true;
                     // Loose the oldest out of the buffer and make do with shorter LEQ time window.
                     avgSum  -= LEQbuffer[BufOutIndex].toDouble();
                     timeSum -= timeBuffer[BufOutIndex];
                     BufOutIndex = (BufOutIndex+1) % maxBufferSize;
                     LEQSampleCount --;
                  }
               }
            }

            void printInfo(){
               //CONSOLE(F("spl: ")); CONSOLE(spldb);
               CONSOLE(F("leq: ")); CONSOLE(read());
               //CONSOLE(F(", nvp10: ")); CONSOLE(newvalpten);
               //CONSOLE(F(", BufInIx: ")); CONSOLE(BufInIndex);
               CONSOLE(F(", BufOutIx: ")); CONSOLE(BufOutIndex); 
               //CONSOLE(F(", maxBufSize: ")); CONSOLE(maxBufferSize);
               
               CONSOLE(F(", timeSum: ")); CONSOLE(timeSum);
               CONSOLE(F(", LEQSmplCount: ")); CONSOLE(LEQSampleCount);
               CONSOLE(F(", LEQSizeSec: ")); CONSOLE(LEQSizeInMillis); 
               CONSOLE(F(", LEQIsValid: ")); CONSOLELN(isValid); 
            }

            String getInfo(){
               String info = String(F("leq: "))  + String(read(), 1);
               info += String(F(", BufOutIx: ")) + String(BufOutIndex);
               info += String(F(", splSum: "))   + String(avgSum, 6);
               info += String(F(", timeSum: "))  + String(timeSum) + String(F("/"))  + String(LEQSizeInMillis);
               info += String(F(", LEQsc: "))    + String(LEQSampleCount)  + String(F("/"))  + String(maxBufferSize);
               info += String(F(", S/Sec: "))    + String(LEQSampleCount * 1000.0 / timeSum, 2);
               info += String(F(", IsValid: "))  + String(isValid);
               return info;
            }

            void resetLEQ(){
               LEQSampleCount = 0;
               avgSum = 0;
               timeSum = 0;
               BufOutIndex = 0;
               isValid = false; 
            }

            // Return the current LEQ Value.
            float read(){
               if (LEQSampleCount == 0) return 0; // Division by zero will crash.
               float avgP10 = avgSum / LEQSampleCount;
               float avgdB  = 10.0 * log10(avgP10 * 10000.0 * float16scale); // Scale (10,000 inside log), convert bel to dB * 10
               // We now have an leq value.
               return roundf(avgdB * 10) / 10.0;  // This rounds our leq to 1 dp.
            }
      }; // End Class avgLEQ

      //********************************************************************************
      // Note: Global to all LEQarrays after here.

      avgLEQ * leqArray[3];                  // Array of pointers to LEQ instances.
      bool avgLEQ::bufferOverflow = false;   // Initialise class static.
      byte leqIDcount = 0;                   // LEQ Class instance index. 

   //   uint16_t consecDupes = 0;
   //   uint16_t oRange = 0;
   //   uint16_t uRange = 0;
   //   String lastuRange = "";
   //   String lastoRange = "";
   //   uint16_t pwr10Error = 0;

      // Make a new LEQ, add it to the Array of LEQs and return its unique ID.
      uint8_t newLEQ(uint16_t LEQSizeInSec){
         if (leqIDcount >= MAX_LEQS) return -1; // No more LEQs available.
         #ifdef LEQ_DEBUG
            CONSOLE(F("Hello from newLEQ...leqIDcount: ")); CONSOLE(leqIDcount);
         #endif
         leqArray[leqIDcount] = (new leq::avgLEQ(LEQSizeInSec));
         return leqIDcount++;    // Return the ID of the new LEQ.
      }

      //String last10spl(){
      //    String last10spl = "BufLast30: ";
      //
      //    for (uint8_t i=1; i<31;i++){
      //       if (i!=1) last10spl += ", ";
      //       last10spl += String(LEQbuffer[(BufInIndex-i + maxBufferSize) % maxBufferSize].toDouble(), 6);
      //    }
      //    return last10spl;
      // }

      String getLEQInfo(uint16_t sampleTime){

         String LEQInfo = ""; //last10spl() + "<br>" 
      //   LEQInfo += FPSTR("uRange: "); LEQInfo += uRange;
      //   LEQInfo += FPSTR(":> "); LEQInfo += lastuRange;
      //   LEQInfo += FPSTR(", oRange: "); LEQInfo += oRange;
      //   LEQInfo += FPSTR(":> "); LEQInfo += lastoRange;
      //   LEQInfo += FPSTR(", p10Err: "); LEQInfo += pwr10Error;
      //   LEQInfo += FPSTR(", DupeCount: "); LEQInfo += consecDupes;
      //   LEQInfo += "<br>";
         
         for (byte i=0; i < leqIDcount;i++){
            LEQInfo += String(F("ID: ")) + i + ", " + leqArray[i]->getInfo() + "<br>";
         }
         LEQInfo += String(F("BufInIx: "))        + String(BufInIndex);
         LEQInfo += String(F(", sampleTime: "))   + String(sampleTime);
         if (avgLEQ::bufferOverflow) { // static across all instances.
            LEQInfo += String(F("<br><b>BufferOverflow: *TRUE!* Broken! Put Meter in SLOW!</b>"));
         }
         return LEQInfo;
      }

       String addVal(uint16_t spldB){  // This is spl * 10. eg 1054  = 105.4dB
         #ifdef LEQ_DEBUG
            CONSOLE(F("Buffer: ")); for (short i=0; i<10; i++){CONSOLE(timeBuffer[i]); CONSOLE(" ");}
         #endif

         static uint32_t thisMillis = 0;                 // STATIC! Initialised only once!
         static uint16_t lastspldB = 0;
   
   // Range checks are done in the readerloop now.
   //      if (spldB < 1){
   //         lastuRange += ":" + String(spldB);
   //         uRange ++;
   //         return getLEQInfo(millis() - thisMillis); // Actually lastMillis but we don't want to commit yet.
   //      }
   //      if (spldB > 1500){
   //         lastoRange += ":" + String(spldB);
   //         oRange ++;
   //         return getLEQInfo(millis() - thisMillis); // Actually lastMillis but we don't want to commit yet.
   //      }

         // Explanation: The SPL meter in fast mode sends new packets every 120ms but only updates data value every 500ms (approx).
         // Rather than fill the buffer & use RAM unnecessarily we just let the sampletime increase for this dupe value until we receive a different value.
         
         if (spldB == lastspldB && (millis() - thisMillis < 800)) { // But we don't want consecutive dupes indefinately otherwise the graph will look odd.
   //         consecDupes ++;
            return getLEQInfo(millis() - thisMillis); // Actually lastMillis but we don't want to commit yet.
         }
         lastspldB = spldB;

         // Here we have to convert to spl (/10) then to bels (/10)
         // Then raise 10^x then scale by 10,000 and the floatscale to fit into float16
         // The float16scale is a "normalising" ratio which makes the max float16value (65504) equivalent to 150.0 dB
         // (See 1/2 precision float spreadsheet.)
         float bels = spldB / 100.0;
         float splPwrTen = pow(10, bels-4) / float16scale;   // then raise to 10^x and scale /10,000 (-4 exponent)
         
         if (splPwrTen < 0 || splPwrTen > 65504) {
   //         pwr10Error ++;
            return getLEQInfo(millis() - thisMillis); // Actually lastMillis but we don't want to commit yet.
         }

         uint32_t lastMillis = thisMillis;
         thisMillis = millis();
         uint16_t sampleTime = thisMillis - lastMillis;  // Something's wrong if we're waiting more than a minute between samples!
         
         // Insert the values into the buffers.
         float16 splfloat16 = (float16)splPwrTen;  // Convert once & use same to add to buffer and to LEQ splsums.
         LEQbuffer[BufInIndex] = splfloat16;
         timeBuffer[BufInIndex] = sampleTime;
         BufInIndex = (BufInIndex + 1) % maxBufferSize;  // Round Robin (FIFO...)

         // If we need more bufferspace then increase them here.
         /* Great idea, but q**ks everything up. Best start with an array that's the right size in the 1st place.
               if (LEQSampleCount +1 >= maxBufferSize){
                  maxBufferSize += 10; // Add ten more values
                  LEQbuffer.resize(10);
                  timeBuffer.resize(10);
               }
         */

         //Allow each LEQ add the values to their own sums and increment their own BufOut pointers & counters.
         avgLEQ::bufferOverflow = false;
         for (uint8_t i=0; i < leqIDcount;i++){
            leqArray[i]->addval(splfloat16, sampleTime);
         }

      return getLEQInfo(sampleTime);
      }

       // Reset 30 sec after startup to allow for meter handling and placement
      void resetAll(){
         for (byte i=0;i < leqIDcount;i++){
            leqArray[i]->resetLEQ();
         }
         BufInIndex=0;  // Reset the global buffer.
      }
   }; // End Namespace leq.
#endif