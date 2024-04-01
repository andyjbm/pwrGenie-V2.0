
#pragma once
#ifndef JKBMS_H
    #define JKBMS_H

    #ifdef ESP8266
        #define jkbms_RX_PIN       D2
        #define jkbms_TX_PIN       D3
    #elif USING_ESP32_S2 
        #define jkbms_RX_PIN    4 // GPIO_NUM_4  //4 = D2
        #define jkbms_TX_PIN    5 // GPIO_NUM_5  //0 = D3
    #elif USING_ESP32_D1_MINI
        #define jkbms_RX_PIN    4 // GPIO_NUM_4  
        #define jkbms_TX_PIN    16 // GPIO_NUM_16
    #elif USING_ESP32_DOIT_DEVKIT_V1
        #error "You haven't decided on pins for BMS serial RS485 on this board yet. Please pick some and place the defs here in JKBMS.h"
        #define jkbms_RX_PIN    ?
        #define jkbms_TX_PIN    ?
    #elif
        #error "Board not recongnised. Add the board def and pick some pins for RS485 serial in JKBMS.h"
    #endif

    #include "Arduino.h"
    #include "emoncms.h"
   
    // Use Software serial for ESP8266 & Hardware for ESP32
    #ifdef JKBMS_SSerial
        #include "SoftwareSerial.h"
        SoftwareSerial TxToJKBMS(jkbms_RX_PIN,jkbms_TX_PIN);
    #else
        HardwareSerial TxToJKBMS(1);
    #endif

    #include "JKBMS_Ecms_Struct.h"
    #include "jk-bms.hpp"

    bool sDebugModeActivated = false;               // Is activated on long press
    bool sFrameIsRequested = false;                 // If true, request was recently sent so now check for serial input
    bool frameIsReceivedOK = false;
    bool JK_LastFrameReceivedOK = false;            // Signal for main.cpp debug webpage.
    String JK_LastResultData = "";

    bool sSendRequestFrame = false;
    bool readJKBMS = false;
    uint8_t readJKRetries = 0;

    uint32_t sMillisOfLastRequestedJKDataFrame = 0;
    uint32_t sMillisOfLastReceivedByte = 0;         // For timeout

    bool sBMSFrameProcessingComplete = false; // True if one status frame was received and processed or timeout happened. Then we can do a sleep at the end of the loop.
    uint32_t sTimeoutFrameCounter = 0;      // Counts BMS frame timeouts, (every 2 seconds)
    uint32_t sTimeoutFrameCounterLifetime = 0;
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
        jkbms_readJKResultCode readJK_BMSStatusFrame();
        void processJK_BMSStatusFrame();
        void handleFrameReceiveTimeout();
        void processReceivedData();
        void printReceivedData();
        void postDataSomewhereUseful();
    }
    
    // Put this in your setup() {}
    void pg_jkbms::setup_bms_serial()
    {
        #ifdef JKBMS_SSerial
            //310 is the size of the byte buffer. A JKBMS reply frame is 301 bytes. SoftwareSerial default is 64 bytes.
            //1550 is the size of the isr bit buffer. Ths works out at 5*buffer = 310*5=1550.
            //This is because the baud rate is so high and it takes so long to read bytes out of the buffer.
            // Without these speecified, SoftwareSerial would choose a collosal isr buffer and use up approx 10kB ESP8266 RAM!
           TxToJKBMS.begin(115200, EspSoftwareSerial::Config::SWSERIAL_8N1, jkbms_RX_PIN, jkbms_TX_PIN, false, 310,1550);
        #else            
            TxToJKBMS.begin(115200, SERIAL_8N1, jkbms_RX_PIN, jkbms_TX_PIN);
        #endif

        // Skip the 1st poll.
        sMillisOfLastRequestedJKDataFrame = millis() + MILLISECONDS_BETWEEN_JK_DATA_FRAME_REQUESTS;
    }

    // Call this in your loop() {}
    void pg_jkbms::bms_pollLoop()
    {
        // Request status frame every x seconds
        //if ((sMillisOfLastRequestedJKDataFrame - millis()) < MILLISECONDS_BETWEEN_JK_DATA_FRAME_REQUESTS) {
        //    sMillisOfLastRequestedJKDataFrame += MILLISECONDS_BETWEEN_JK_DATA_FRAME_REQUESTS; // set for next check
        if (readJKBMS) { // Request from maim to get the data from BMS.
            readJKBMS = false; 
            sSendRequestFrame = true;
            readJKRetries = 0;
        }

        if (sSendRequestFrame) {
            sDebugModeActivated = true;
            sSendRequestFrame = false;

            send_data_request_frame();

            sMillisOfLastReceivedByte = millis(); // initialize reply timeout
            sFrameIsRequested = true;             // enable check for serial input
        }

        //checkButtonPress();

        if (frameIsReceivedOK) {
            // Once we have a successful frame we can interpret it and perform fun calculations on our data.
            if (sDebugModeActivated) {
                //Do it once at every debug start
                if (sReplyFrameBufferIndex == 0) {
                    CONSOLELN(F("sReplyFrameBufferIndex is 0"));
                } else {
                    CONSOLELN(sReplyFrameBufferIndex + 1, F("bytes received"));
                    //printJKReplyFrameBuffer();
                }
            }

            // Process the complete status frame, do calcs and set the appropriate flags.
            processJK_BMSStatusFrame();

            // We're finished. Reset ready for next time.
            sBMSFrameProcessingComplete = true;
            frameIsReceivedOK = false;

            // Data is ready to post onto a DB or take some other action.
            postDataSomewhereUseful(); 
        }

        // This and the one above frameIsReceivedOK are reverse order so as to allow
        // a go round the loop between receive complete and processing the frame.
        if (sFrameIsRequested)
        {
            #if defined(TIMING_TEST)
                digitalWriteFast(TIMING_TEST_PIN, HIGH);
            #endif

            switch (readJK_BMSStatusFrame())
            {
                case JK_BMS_RECEIVE_FINISHED :  // Frame read complete. Correct number of bytes and CRC ok.
                    if (readJKRetries > 0){
                        CONSOLELN(F("readJK Frame FINISHED with retry Count:"), readJKRetries);
                    }
                    frameIsReceivedOK = true;
                    JK_LastFrameReceivedOK = true; // Signal to Main.cpp debug webpage.
                    sFrameIsRequested = false; // Everything OK, do not try to receive more
                    break;

                case JK_BMS_RECEIVE_OK :  // Reading data in progress - waiting on Serial. Go around.
                    break;

                default : // Error or Timeout
                    if (readJKRetries < READJK_TIMEOUT_RETRY_COUNT) {
                        // Error or Timeout but retries not reached.
                        readJKRetries++;
                        sFrameIsRequested = false;
                        sSendRequestFrame = true;  // Try again, Set to send a new requestFrame.
                    } else {
                        // Retries reached, time to abort.
                        LOGDEBUG1(F("readJK Frame Error or Timeout, retry Count: "), readJKRetries);
                        sFrameIsRequested = false; 
                        JK_LastFrameReceivedOK = false; // Signal to Main.cpp debug webpage.
                        sBMSFrameProcessingComplete = true; // Give up. We're done trying.
                    }
            }
            #if defined(TIMING_TEST)
                digitalWriteFast(TIMING_TEST_PIN, LOW);
            #endif
        }

        // Do this once after each complete status frame or timeout.
        // It's a place for tidying up and resetting, not posting results anywhere.
        // Use postDataSomewhereUseful() in JKBMS.h to post your data somewhere useful. 
        if (sBMSFrameProcessingComplete) {
            sDebugModeActivated = false;            // reset flag here. It may be set again at start of next loop.
            sBMSFrameProcessingComplete = false;    // prepare for next loop.
        } // if (sBMSFrameProcessingComplete)
    }

    // Sends a frame requesting data.
    void pg_jkbms::send_data_request_frame()
    {
       // Flush input buffer and send request to JK-BMS
        while (TxToJKBMS.available()) {
            TxToJKBMS.read();
        }
        
        initJKReplyFrameBuffer();
        #if defined(TIMING_TEST)
            digitalWriteFast(TIMING_TEST_PIN, HIGH);
        #endif
        requestJK_BMSStatusFrame(&TxToJKBMS, JKRequestStatusFrame, sizeof(JKRequestStatusFrame), sDebugModeActivated); // 1.85 ms
        #if defined(TIMING_TEST)
            digitalWriteFast(TIMING_TEST_PIN, LOW);
        #endif
    }

    jkbms_readJKResultCode pg_jkbms::readJK_BMSStatusFrame()
    {
        // Get reply from BMS and check timeout
 
        while (millis() - sMillisOfLastReceivedByte < TIMEOUT_MILLIS_FOR_FRAME_REPLY) {
            if (TxToJKBMS.available())
            {
                sMillisOfLastReceivedByte = millis();

                jkbms_readJKResultCode tReceiveResultCode = readJK_BMSStatusFrameByte(&TxToJKBMS);
                if (tReceiveResultCode == JK_BMS_RECEIVE_FINISHED)
                {
                    if (sTimeoutFrameCounter > 0) {// First complete frame after some timeouts.
                        sTimeoutFrameCounter = 0;  // Reset this counter but not the lifetime counter.
                    }
                } else if (tReceiveResultCode == JK_BMS_RECEIVE_ERROR)
                { 
                    // Error here
                    CONSOLELN(F("Receive error"), tReceiveResultCode, F("at index"), sReplyFrameBufferIndex);
                }
                return tReceiveResultCode; // FINISHED, OK or ERROR.
            }
        }  // While timeout loop

        handleFrameReceiveTimeout();
        return JK_BMS_RECEIVE_TIMEOUT;        
    }

    void pg_jkbms::processJK_BMSStatusFrame() {
     // LOGDEBUG(F("Call to processJK_BMSStatusFrame."));  
        
        processReceivedData();
        printReceivedData();

        // Copy complete reply and computed values for change determination
    //    lastJKReply.SOCPercent = sJKFAllReplyPointer->SOCPercent;
    //    lastJKReply.AlarmUnion.AlarmsAsWord = sJKFAllReplyPointer->AlarmUnion.AlarmsAsWord;
    //    lastJKReply.BMSStatus.StatusAsWord = sJKFAllReplyPointer->BMSStatus.StatusAsWord;
    //    lastJKReply.SystemWorkingMinutes = sJKFAllReplyPointer->SystemWorkingMinutes;
    }

    void pg_jkbms::handleFrameReceiveTimeout() {
        //sDoErrorBeep = true;
        if (sReplyFrameBufferIndex != 0 || sTimeoutFrameCounter == 0) {

            // No byte received here -BMS may be off or disconnected
            // Do it only once if we receive only 0 bytes
            CONSOLELN(F("Receive timeout at ReplyFrameBufferIndex:"), sReplyFrameBufferIndex);
            if (sReplyFrameBufferIndex > 3) {
                CONSOLELN(F("Packet length bytes sReplyFrameLength:"), sReplyFrameLength);
                //printJKReplyFrameBuffer();
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
            sTimeoutFrameCounter--; // This is to avoid overflow. We have an unsigned integer here
        }
        sTimeoutFrameCounterLifetime++;
        if (sTimeoutFrameCounterLifetime == 0) {
            sTimeoutFrameCounterLifetime--; // This is to avoid overflow. We have an unsigned integer here
        }
        CONSOLELN(F("sTimeoutFrameCounter:"),sTimeoutFrameCounter);
        CONSOLELN(F("Lifetime Timeouts:   "),sTimeoutFrameCounterLifetime);

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


        // Here we sanity check to make sure the JKReplyStruct is overlaying over the frameBuffer correctly.
        // We check by looking at the last token we are expecting and making sure it's what it's supposed to be.
        if (sJKFAllReplyPointer->TokenProtocolVersionNumber != 0xC0) {
            CONSOLELN(F("WARNING! Data Frame **NOT** interpreted correctly! TokenProtocolVersionNumber should be 0xC0, was in fact:"))
            printBytePaddedHex(sJKFAllReplyPointer->TokenProtocolVersionNumber);
            CONSOLELN()
        } else {
            CONSOLELN(F("OK! Data Frame was interpreted correctly."));
        }

        fillJKConvertedCellInfo();
        fillJKComputedData();

        // #if !defined(NO_ANALYTICS)
        //     computeCapacity();
        //     writeSOCData();
        // #endif

        handleAndPrintAlarmInfo();
        computeUpTimeString();

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

    String getPROGMEMstr(const char * const pMemPtr){
        char bufr[20];
        strcpy_P(bufr,pMemPtr);
        return (String) bufr;
    }

    void pg_jkbms::postDataSomewhereUseful()
    {
        float * jkDFloat = (float *) &jkD;    // Specifically cast as address to a float.
        JK_LastResultData ="";

        // cellVoltages
        bool doBR = false;
        for (uint8_t i = 0; i < JKConvertedCellInfo.ActualNumberOfCellInfoEntries; ++i) {
            // For Emoncms:
            jkDFloat[i] = JKConvertedCellInfo.CellInfoStructArray[i].CellMillivolt;
            
            // for Debug webpage:
            if (doBR) {JK_LastResultData += "<br>";}
            if (i == JKConvertedCellInfo.IndexOfMinimumCellMillivolt) {JK_LastResultData += "<div style='color:#f00'><b>";}
            if (i == JKConvertedCellInfo.IndexOfMaximumCellMillivolt) {JK_LastResultData += "<div style='color:#00f'><b>";}

            JK_LastResultData += getPROGMEMstr(jkDataNames[i]) + ": " + JKConvertedCellInfo.CellInfoStructArray[i].CellMillivolt;

            if ((i == JKConvertedCellInfo.IndexOfMinimumCellMillivolt) ||
                (i == JKConvertedCellInfo.IndexOfMaximumCellMillivolt))
            {
                JK_LastResultData += "</b></div>";
                doBR = false;
            } else {
                doBR = true;
            }
        }
       
        jkD.AvCellV = JKConvertedCellInfo.AverageCellMillivolt;
        jkD.CellDif = JKConvertedCellInfo.MaximumCellMillivolt - JKConvertedCellInfo.MinimumCellMillivolt;
        jkD.BalI = 999; // Not sure if this is available.

        // Battery Volts, current, soc and balancer active. More to follow soon.
        jkD.V = float(JKComputedData.BatteryVoltage10Millivolt) / 100;
        jkD.I = JKComputedData.BatteryLoadCurrentFloat;
        jkD.SOC = sJKFAllReplyPointer->SOCPercent;
        jkD.BalAct = sJKFAllReplyPointer->BMSStatus.StatusBits.BalancerActive;
        jkD.ChAct = sJKFAllReplyPointer->BMSStatus.StatusBits.ChargeMosFetActive;
        jkD.DisAct = sJKFAllReplyPointer->BMSStatus.StatusBits.DischargeMosFetActive;

        for (uint8_t i = 20; i < jkbms_DataElementCount; ++i){
            JK_LastResultData += "<br>" + getPROGMEMstr(jkDataNames[i]) + ": " + ((float*)&jkD)[i];
        }

        emoncms::send2emoncms(jkDataNames, (float *) &jkD, jkbms_DataElementCount);
    }

#endif