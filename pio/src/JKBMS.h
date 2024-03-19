// https://github.com/ArminJo/JK-BMSToPylontechCAN

#ifndef JKBMS_H
    #define JKBMS_H

    #define MILLISECONDS_BETWEEN_JK_DATA_FRAME_REQUESTS     10000

    // 60 gets timeouts now & again, 80 is ok.
    #define TIMEOUT_MILLIS_FOR_FRAME_REPLY                  100 // I measured 26 ms between request end and end of received 273 byte result
 
    #ifdef ESP8266
        #define jkbms_RX_PIN       D2
        #define jkbms_TX_PIN       D3
        //#define jkbms_TX_EN_PIN    D0
    #else
        #define jkbms_RX_PIN       4
        #define jkbms_TX_PIN       0
        //#define jkbms_TX_EN_PIN    16
    #endif


    #include "Arduino.h"

    #include "SoftwareSerial.h"

    #include "jk-bms.hpp"

    #include "defs.h"
    #include "Globals.h"

    #include "Ecms_Struct.h"
    #include "emoncms.h"

   
    SoftwareSerial TxToJKBMS(jkbms_RX_PIN,jkbms_TX_PIN);
    bool sDebugModeActivated = false;               // Is activated on long press
    bool sFrameIsRequested = false;                 // If true, request was recently sent so now check for serial input
    bool frameIsReceivedOK = false;
    uint32_t sMillisOfLastRequestedJKDataFrame = 0;
    uint32_t sMillisOfLastReceivedByte = 0;         // For timeout


    bool sBMSFrameProcessingComplete = false; // True if one status frame was received and processed or timeout happened. Then we can do a sleep at the end of the loop.
    uint16_t sTimeoutFrameCounter = 0;      // Counts BMS frame timeouts, (every 2 seconds)
    bool sStaticInfoWasSent = false; // Flag to send static Info only once after reset.

    void processReceivedData();
    void processJK_BMSStatusFrame();
    void printReceivedData();

    // Like a class but global. And already instansiated.
    namespace pg_jkbms {
        void setup_bms_serial();
        void send_data_request_frame();
        void bms_pollLoop();
        void check_for_reply();
        bool readJK_BMSStatusFrame();
        void processJK_BMSStatusFrame();
        void handleFrameReceiveTimeout();
        void processReceivedData();
        void printReceivedData();
    }

    
    // Put this in your setup() {}
    void pg_jkbms::setup_bms_serial()
    {
        TxToJKBMS.begin(115200,EspSoftwareSerial::Config::SWSERIAL_8N1, jkbms_RX_PIN,jkbms_TX_PIN,false, 320);
        //TxToJKBMS.begin(9600);

        // Skip the 1st poll.
        sMillisOfLastRequestedJKDataFrame = millis() + MILLISECONDS_BETWEEN_JK_DATA_FRAME_REQUESTS;
    }

    // Call this in your loop() {}
    void pg_jkbms::bms_pollLoop()
    {
        // Request status frame every 2 seconds
        if ((sMillisOfLastRequestedJKDataFrame - millis()) < MILLISECONDS_BETWEEN_JK_DATA_FRAME_REQUESTS) {
            sMillisOfLastRequestedJKDataFrame += MILLISECONDS_BETWEEN_JK_DATA_FRAME_REQUESTS; // set for next check

            LOGDEBUG(F("Sending JKBMS Request for Data..."));
            sDebugModeActivated = true;
            send_data_request_frame();
        }

        //checkButtonPress();


        if (frameIsReceivedOK) {
            processJK_BMSStatusFrame(); // Process the complete receiving of the status frame and set the appropriate flags
            sBMSFrameProcessingComplete = true;
            frameIsReceivedOK = false;
        }

        // This and the one above frameIsReceivedOK are reverse order so as to allow
        // a go round the loop between receive complete and processing the frame.
        if (sFrameIsRequested) {
            check_for_reply();
        }

        // Do this once after each complete status frame or timeout
        if (sBMSFrameProcessingComplete) {
            sDebugModeActivated = false;            // reset flag here. It may be set again at start of next loop.
            sBMSFrameProcessingComplete = false;    // prepare for next loop
        } // if (sBMSFrameProcessingComplete)
    }

    // Sends a frame requesting data.
    void pg_jkbms::send_data_request_frame()
    {
       // Flush input buffer and send request to JK-BMS
        while (TxToJKBMS.available()) {
            TxToJKBMS.read();
        }
        
        #if defined(TIMING_TEST)
            digitalWriteFast(TIMING_TEST_PIN, HIGH);
        #endif
        requestJK_BMSStatusFrame(&TxToJKBMS, JKRequestStatusFrame, sizeof(JKRequestStatusFrame), sDebugModeActivated); // 1.85 ms
//        requestJK_BMSStatusFrame(&TxToJKBMS, JKRequestVBattFrame, sizeof(JKRequestVBattFrame), sDebugModeActivated); // 1.85 ms
        #if defined(TIMING_TEST)
            digitalWriteFast(TIMING_TEST_PIN, LOW);
        #endif
        
        sFrameIsRequested = true; // enable check for serial input
        initJKReplyFrameBuffer();
        sMillisOfLastReceivedByte = millis(); // initialize reply timeout
    }

    void pg_jkbms::check_for_reply()
    {
        // Get reply from BMS and check timeout
        while (millis() - sMillisOfLastReceivedByte < TIMEOUT_MILLIS_FOR_FRAME_REPLY) {
            if (TxToJKBMS.available()) {
                #if defined(TIMING_TEST)
                    digitalWriteFast(TIMING_TEST_PIN, HIGH);
                #endif
                if (readJK_BMSStatusFrame()) {
                    // Frame completely received.
                    frameIsReceivedOK = true;
                    sFrameIsRequested = false; // Everything OK, do not try to receive more
                    sJKBMSFrameHasTimeout = false;
                    if (sTimeoutFrameCounter > 0) {// First frame after timeout
                        sTimeoutFrameCounter = 0;
                        // #if defined(USE_SERIAL_2004_LCD) && !defined(DISPLAY_ALWAYS_ON)
                        //    if (checkAndTurnLCDOn()) {
                        //      JK_INFO_PRINTLN(F("successfully receiving first BMS status frame after BMS communication timeout")); // Switch on LCD display, triggered by successfully receiving first BMS status frame
                        //    }
                        // #endif
                    }
                    
                    if (sDebugModeActivated) {
                        //Do it once at every debug start
                        if (sReplyFrameBufferIndex == 0) {
                            Serial.println(F("sReplyFrameBufferIndex is 0"));
                        } else {
                            Serial.print(sReplyFrameBufferIndex + 1);
                            Serial.println(F(" bytes received"));
                            printJKReplyFrameBuffer();
                        }
                        Serial.println();
                    }
                    return ;
                }
                #if defined(TIMING_TEST)
                    digitalWriteFast(TIMING_TEST_PIN, LOW);
                #endif
            } else {
                return ; // Not finished but let's not block while we wait for more data.
            }

        }  // While timeout.
        // Here we have requested frame, but serial was not available for a longer time => timeout at receiving
        // If no bytes received before (because of BMS disconnected), print it only once
        handleFrameReceiveTimeout();
        
    }

    bool pg_jkbms::readJK_BMSStatusFrame() {
        sMillisOfLastReceivedByte = millis();
        uint8_t tReceiveResultCode = readJK_BMSStatusFrameByte(&TxToJKBMS);
        if (tReceiveResultCode == JK_BMS_RECEIVE_FINISHED) {
            /*
            * All JK-BMS status frame data received
            */
            return true;

        } else if (tReceiveResultCode != JK_BMS_RECEIVE_OK) {
            /*
            * Error here
            */
            Serial.print(F("Receive error="));
            Serial.print(tReceiveResultCode);
            Serial.print(F(" at index"));
            Serial.println(sReplyFrameBufferIndex);
            sFrameIsRequested = false; // do not try to receive more
            sBMSFrameProcessingComplete = true;
            printJKReplyFrameBuffer();
        }
        return false;
    }

    void pg_jkbms::processJK_BMSStatusFrame() {
     LOGDEBUG(F("Call to processJK_BMSStatusFrame."));  
        
        processReceivedData();
    //    printReceivedData();

        // Copy complete reply and computed values for change determination
    //    lastJKReply.SOCPercent = sJKFAllReplyPointer->SOCPercent;
    //    lastJKReply.AlarmUnion.AlarmsAsWord = sJKFAllReplyPointer->AlarmUnion.AlarmsAsWord;
    //    lastJKReply.BMSStatus.StatusAsWord = sJKFAllReplyPointer->BMSStatus.StatusAsWord;
    //    lastJKReply.SystemWorkingMinutes = sJKFAllReplyPointer->SystemWorkingMinutes;
    }

    void pg_jkbms::handleFrameReceiveTimeout() {
        //sDoErrorBeep = true;
        sFrameIsRequested = false; // Do not try to receive more
        sBMSFrameProcessingComplete = true;
        sJKBMSFrameHasTimeout = true;
        if (sReplyFrameBufferIndex != 0 || sTimeoutFrameCounter == 0) {

            // No byte received here -BMS may be off or disconnected
            // Do it only once if we receive only 0 bytes
            LOGDEBUG1(F("Receive timeout at ReplyFrameBufferIndex: "), sReplyFrameBufferIndex);
            if (sReplyFrameBufferIndex > 3) {
                LOGDEBUG1(F("Packet length bytes sReplyFrameLength: "), sReplyFrameLength);
                printJKReplyFrameBuffer();
            }
            // #if defined(USE_SERIAL_2004_LCD)
            //         if (sSerialLCDAvailable && sLCDDisplayPageNumber == JK_BMS_PAGE_CAN_INFO) {
            //             // Update the changed values on LCD
            //             myLCD.clear();
            //             myLCD.setCursor(0, 0);
            //             printCANInfoOnLCD();
            //         }
            // #  if !defined(DISPLAY_ALWAYS_ON)
            //         if (checkAndTurnLCDOn()) {
            //             Serial.println(F("BMS communication timeout")); // Switch on LCD display, triggered by BMS communication timeout
            //         }
            // #  endif
            // #endif
        }

        sTimeoutFrameCounter++;
        if (sTimeoutFrameCounter == 0) {
            sTimeoutFrameCounter--; // To avoid overflow, we have an unsigned integer here
        }
        LOGDEBUG1(F("sTimeoutFrameCounter: "),sTimeoutFrameCounter);

        // #if defined(USE_SERIAL_2004_LCD)
        //     printTimeoutMessageOnLCD();
        // #endif
    }

    void pg_jkbms::processReceivedData() {
        /*
        * Set the static pointer to the start of the reply data which depends on the number of cell voltage entries
        * The JKFrameAllDataStruct starts behind the header + cell data header 0x79 + CellInfoSize + the variable length cell data (CellInfoSize is contained in JKReplyFrameBuffer[12])
        */
        sJKFAllReplyPointer = reinterpret_cast<JKReplyStruct*>(&JKReplyFrameBuffer[JK_BMS_FRAME_HEADER_LENGTH + 2
                + JKReplyFrameBuffer[JK_BMS_FRAME_INDEX_OF_CELL_INFO_LENGTH]]);

        fillJKConvertedCellInfo();
//        fillJKComputedData();

        // #if !defined(NO_ANALYTICS)
        //     computeCapacity();
        //     writeSOCData();
        // #endif

    //    handleAndPrintAlarmInfo();
    //    computeUpTimeString();

    //    fillAllCANData(sJKFAllReplyPointer);
    //    sCANDataIsInitialized = true; // One time flag
    }

    void pg_jkbms::printReceivedData() {
        if (!sStaticInfoWasSent) {
            sStaticInfoWasSent = true;
            printJKStaticInfo();
        }

        //#if defined(USE_SERIAL_2004_LCD)
        //    if (sLCDDisplayPageNumber != JK_BMS_PAGE_CAPACITY_INFO) {
        //        printJKDynamicInfo(); // do not interfere with plotter output
        //    }
        //    printBMSDataOnLCD();
        //#else
        printJKDynamicInfo();
        //#endif
    }
#endif