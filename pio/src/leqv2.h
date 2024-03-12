/* LEQ Library.
   This was refactored so that now we only need one buffer for any number of leq instances.
   The buffer only has to be as big as the largest LEQ requires as there is no point in storing
   all the buffered data that is common to all LEQs more than once!

   Each leq instance keeps track of its own average and time sums, its own Buffer Out Index 
   its own number of samples and its own valid flag.
*/

#ifndef LEQV2_H
   #define LEQV2_h

   #include <Arduino.h>
   #include "defs.h"

   //#define LEQ_DEBUG

   #define MAX_LEQS 5               // Default max number of LEQ instances allowed.
   #define MAXBUFFERSIZE 15 * 60    // Default max LEQ Size. Buffer size is 1.2 times this value.

   String LEQInfo;

   namespace leq {
      // "Static" vars global to each leq instance.

      byte leqIDcount = 0;             // LEQ Instance Index.
      // Buffer indexes & sizes
      unsigned short BufInIndex;
      const unsigned short maxBufferSize = MAXBUFFERSIZE * 1.2; // Not a define so that the calc only happens once and here.

      //These are to keep track of the time between samples. By summing these samples we can determine
      //how many samples we need for a particular leq period.
      //This approach compensates for possible drift over time as the number of samples required may change.
      unsigned short timeBuffer[maxBufferSize];
      float LEQbuffer[maxBufferSize];

      unsigned long thisMillis;

      class avgLEQ{

         private:
            unsigned int timeSum;
            float avgSum;
            bool LEQvalid;
            unsigned int LEQSizeInSec;
            unsigned long LEQSizeInMillis;   // *ToDo Can this be just int?
            unsigned short BufOutIndex;
            unsigned int LEQSampleCount;      // Number of active samples for average calculation.

            #ifdef LEQ_DEBUG
               bool leqDebug = false;
            #endif

         public: 

            avgLEQ(unsigned int _LEQSizeInSec){
               #ifdef LEQ_DEBUG
                  if (_LEQSizeInSec == 10) leqDebug = true;
                  CONSOLE(F(", Hello from avgLEQ..._LEQSizeInSec: ")); CONSOLELN(_LEQSizeInSec);
               #endif
               LEQSizeInSec = _LEQSizeInSec;
               LEQSizeInMillis = _LEQSizeInSec * 1000; 
               resetLEQ();
            }

            void addval(float newvalpt, unsigned short sampleTime){
      
               avgSum += newvalpt;                    // New de-logerithmic value into the average
               timeSum += sampleTime;                 // time between last and this reading.
            
               if (LEQvalid){
                  //Drop old buffer values until we have enough for the corect LEQ duration.
                  //This is to allow drift in samples from the meter wrt real time from millis()
                  //Important: This means LEQSampleCount will *change* over time if it needs
                  //to to fit the LEQ time window.
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
                     if (timeSum < timeBuffer[BufOutIndex]){
                        CONSOLE(F("\nCRASH: ")); 
                        CONSOLE(F(", BufOutIndex: ")); CONSOLE(BufOutIndex);
                        CONSOLE(F(", BufInIndex: ")); CONSOLE(BufInIndex);
                        CONSOLE(F(", maxBufSize: ")); CONSOLELN(maxBufferSize);
                     } else {
                        // Roll the old out of the end of the buffer.
                        avgSum  -= LEQbuffer[BufOutIndex];
                        timeSum -= timeBuffer[BufOutIndex];
                        BufOutIndex = (BufOutIndex+1) % maxBufferSize;
                        LEQSampleCount --;
                     }
                  }
               }
               #ifdef LEQ_DEBUG
                  if (leqDebug){CONSOLELN();}
               #endif
               
               LEQSampleCount ++;
               
               // Look for achieving a complete LEQ buffer
               if (!LEQvalid) { // Quick check for speed.
                  if (timeSum > LEQSizeInMillis){ // We are there.
                     LEQvalid = true;
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
               CONSOLE(F(", LEQSizeSec: ")); CONSOLE(LEQSizeInSec); 
               CONSOLE(F(", LEQIsValid: ")); CONSOLELN(LEQvalid); 
            }
            String getInfo(){
               String info = String(F("leq: "))  + String(read());
               info += String(F(", BufOutIx: ")) + String(BufOutIndex);
               info += String(F(", timeSum: "))  + String(timeSum);
               info += String(F(", LEQsc: "))    + String(LEQSampleCount);
               info += String(F(", LEQSize: "))  + String(LEQSizeInSec);
               info += String(F(", IsValid: "))  + String(LEQvalid);
               return info;
            }

            void resetLEQ(){
               LEQSampleCount = 0;
               avgSum = 0;
               timeSum = 0;
               BufOutIndex = 0;
               LEQvalid = false; 
            }

            bool isValid(){return LEQvalid;}

            // Return the current LEQ Value.
            float read(){
               if (LEQSampleCount == 0) return 0; // Division by zero will crash.
               float avgP10 = avgSum / LEQSampleCount;
               float avgdB  = 10.0 * log10(avgP10);
               // We now have an leq value.
               return roundf(avgdB * 10) / 10.0;  // This rounds our leq to 1 dp.
            }
   }; // End Class avgLEQ

      leq::avgLEQ * leqArray[3];       // Array of pointers to LEQ instances.

      // Make a new LEQ, add it to the Array of LEQs and return its unique ID.
      byte newLEQ(short LEQSizeInSec){
         if (leqIDcount >= MAX_LEQS) return -1; // No more LEQs available.
         #ifdef LEQ_DEBUG
            CONSOLE(F("Hello from newLEQ...leqIDcount: ")); CONSOLE(leqIDcount);
         #endif
         leqArray[leqIDcount] = (new leq::avgLEQ(LEQSizeInSec));
         return leqIDcount++;    // Return the ID of the new LEQ.
      }

      void addVal(float spldB){
         #ifdef LEQ_DEBUG
            CONSOLE(F("Buffer: ")); for (short i=0; i<10; i++){CONSOLE(timeBuffer[i]); CONSOLE(" ");}
         #endif
   
         float newvalpten = pow(10, spldB / 10); // Convert to bels (/10) then raise to 10^x)

         unsigned long lastMillis = thisMillis;
         thisMillis = millis();
         unsigned short sampleTime = thisMillis - lastMillis;
         
         //Allow each LEQ add the values to their own sums and to up their own BufOut pointers.
         for (byte i=0; i < leqIDcount;i++){
            leqArray[i]->addval(newvalpten, sampleTime);
         }

         // Insert the values into the buffers.
         LEQbuffer[BufInIndex] = newvalpten;
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
         
         LEQInfo ="";
         for (byte i=0; i < leqIDcount;i++){
            LEQInfo += String(F("ID: ")) + i + ", " + leqArray[i]->getInfo() + "<br>";
         }
         LEQInfo += String(F("BufInIx: "))        + String(BufInIndex);
         LEQInfo += String(F(", sampleTime: "))   + String(sampleTime);
      }

      // Reset 30 sec after startup to allow for meter handling and placement
      void resetAll(){
         for (byte i=0;i < leqIDcount;i++){
            leqArray[i]->resetLEQ();
         }
         leq::BufInIndex=0;  // Reset the global buffer.
      }
   }; // End Namespace leq.
#endif