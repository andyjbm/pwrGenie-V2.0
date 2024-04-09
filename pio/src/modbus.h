#pragma once
#ifndef pgMODBUS_H
    #define pgMODBUS_H

    #include "Arduino.h"
    #include "SoftwareSerialisr.hpp"
    #include <ModbusRTU.h>

    #include "defs.h"
    #include "emoncms.h"

    #include <stdlib.h>

    #ifdef ESP8266
        #define mb_RX_PIN       D2
        #define mb_TX_PIN       D3
        #define mb_TX_EN_PIN    D0 // For old (non-auto direction RS485 boards.)
    #else
        #define mb_RX_PIN       4
        #define mb_TX_PIN       0
        #define mb_TX_EN_PIN    16
    #endif

    namespace mbErrorStr{
        const char mbrc00[] PROGMEM = "0x00 EX_SUCCESS - No error.";
        const char mbrc01[] PROGMEM = "0x01 EX_ILLEGAL_FUNCTION - Function Code not Supported.";
        const char mbrc02[] PROGMEM = "0x02 EX_ILLEGAL_ADDRESS - Output Address not exists.";
        const char mbrc03[] PROGMEM = "0x03 EX_ILLEGAL_VALUE - Output Value not in Range.";
        const char mbrc04[] PROGMEM = "0x04 EX_SLAVE_FAILURE - Slave or Master Device Fails to process request.";
        const char mbrc05[] PROGMEM = "0x05 EX_ACKNOWLEDGE - Not used.";
        const char mbrc06[] PROGMEM = "0x06 EX_SLAVE_DEVICE_BUSY - Not used.";
        const char mbrc08[] PROGMEM = "0x08 EX_MEMORY_PARITY_ERROR - Not used.";
        const char mbrc0A[] PROGMEM = "0x0A EX_PATH_UNAVAILABLE - Not used.";
        const char mbrc0B[] PROGMEM = "0x0B EX_DEVICE_FAILED_TO_RESPOND - Not used.";
        const char mbrcE1[] PROGMEM = "0xE1 EX_GENERAL_FAILURE - Custom. Unexpected master error.";
        const char mbrcE2[] PROGMEM = "0xE2 EX_DATA_MISMACH - Custom. Input data size mismach.";
        const char mbrcE3[] PROGMEM = "0xE3 EX_UNEXPECTED_RESPONSE - Custom. Returned result doesn't mach transaction.";
        const char mbrcE4[] PROGMEM = "0xE4 EX_TIMEOUT - Custom. Operation not finished within reasonable time.";
        const char mbrcE5[] PROGMEM = "0xE5 EX_CONNECTION_LOST - Custom. Connection with device lost.";
        const char mbrcE6[] PROGMEM = "0xE6 EX_CANCEL - Custom. Transaction/request canceled.";
        const char mbrcE7[] PROGMEM = "0xE7 EX_PASSTHROUGH - Custom. Raw callback. Indicate to normal processing on callback exit.";
        const char mbrcE8[] PROGMEM = "0xE8 EX_FORCE_PROCESS - Custom. Raw callback. Indicate to force processing on callback exit.";
        const char mbrcXX[] PROGMEM ="No ResultCode Match.";
    };

    namespace mbDataNames{
        // Parameter names. Across all devices.
        const char d0[] PROGMEM = "vL1";
        const char d1[] PROGMEM = "vL2";
        const char d2[] PROGMEM = "vL3";
        const char d3[] PROGMEM = "vL1L2";
        const char d4[] PROGMEM = "vL2L3";
        const char d5[] PROGMEM = "vL1L3";

        const char d6[] PROGMEM = "i1";
        const char d7[] PROGMEM = "i2";
        const char d8[] PROGMEM = "i3";
        const char d9[] PROGMEM = "iN";

        const char d10[] PROGMEM = "p1";
        const char d11[] PROGMEM = "p2";
        const char d12[] PROGMEM = "p3";

        const char d13[] PROGMEM = "VA1";
        const char d14[] PROGMEM = "VA2";
        const char d15[] PROGMEM = "VA3";

        const char d16[] PROGMEM = "VAr1";
        const char d17[] PROGMEM = "VAr2";
        const char d18[] PROGMEM = "VAr3";

        const char d19[] PROGMEM = "vLNAvg";
        const char d20[] PROGMEM = "vLLAvg";
        const char d21[] PROGMEM = "kW";      //Power Sum
        const char d22[] PROGMEM = "kVA";     //VA Sum
        const char d23[] PROGMEM = "kVAr";    //VAr Sum

        const char d24[] PROGMEM = "pf1";
        const char d25[] PROGMEM = "pf2";
        const char d26[] PROGMEM = "pf3";

        const char d27[] PROGMEM = "pfAvg";

        const char d28[] PROGMEM = "PhS";
        const char d29[] PROGMEM = "fHz";

        const char d30[] PROGMEM = "kWhT";
        const char d31[] PROGMEM = "kVArhT";

        const char d32[] PROGMEM = "bfHz";      // bus Hz
        const char d33[] PROGMEM = "bL1";       // Bus Voltages
        const char d34[] PROGMEM = "bL2";
        const char d35[] PROGMEM = "bL3";
        const char d36[] PROGMEM = "bL1L2";
        const char d37[] PROGMEM = "bL2L3";
        const char d38[] PROGMEM = "bL1L3";

        const char * const mbDataNames[] = {
            d0,d1,d2,d3,d4,d5,d6,d7,d8,d9,d10,d11,d12,d13,d14,d15,d16,d17,d18,d19,d20,
            d21,d22,d23,d24,d25,d26,d27,d28,d29,d30,d31,d32,d33,d34,d35,d36,d37,d38
            };
    };

    // This for the results after register data wrangling.
    struct mbData {
            float d1,d2,d3,d4,d5,d6,d7,d8,d9,d10,d11,d12,d13,d14,d15,d16,d17,d18,d19,d20,d21,d22,d23,d24,d25,d26,d27,d28,d29,d30,d31;
        };

    #ifdef MODBUS_DEVICE_EM21
        const uint8_t MODBUS_ID       = 1;
        const uint8_t MODBUS_HBASE    = 0;
        const uint16_t MODBUS_BAUD    = 9600;
        const uint8_t MODBUS_REG_PER  = 12;         // Regs count to fetch in each block request.
        const uint8_t MODBUS_REGMAX   = 60;         // Number of registers to be fetched/ We only need 56 but have to fetch in blocks of 12 from the EM21 => Since learned this may be because of the default 64 byte buffer on SoftwareSerial.
        const uint8_t mbDataElementCount = 31;   // Number of parameters these registers fit into.
        
        // Array of the names for EM21   
        const uint16_t mbNameIndex[] = {
            0,1,2,3,4,5,6,7,8,  10,11,12, 13,14,15, 16,17,18, 19,20, 21,22,23, 24,25,26, 27,28,29, 30,31};
 
        const int mbScale[mbDataElementCount] = {
            10,10,10,  10,10,10,  1000,1000,1000,  10,10,10, 10,10,10, 
            10,10,10,  10,10, 1000,1000,1000, 1000,1000,1000,1000, 1, 1, 10, 10 
        };

        // How many registers per parameter eg: 32bit Parameters need two registers etc.
        const byte mbRegSize[mbDataElementCount] = {2,2,2,  2,2,2,  2,2,2,  2,2,2,  2,2,2,  2,2,2,  2,2,2,2,2,  1,1,1, 1,1,1,  2,2};
        // How many decimal places to format display strings to.
        const byte mbDP[mbDataElementCount] =      {2,2,2,  2,2,2,  2,2,2,  2,2,2,  2,2,2,  2,2,2,  2,2,2,2,2,  3,3,3, 3,2,1,  2,2};
    
    #elif defined(MODBUS_DEVICE_APM303)    
        const uint8_t MODBUS_ID       = 1;
        const uint8_t MODBUS_HBASE    = 0;
        const uint16_t MODBUS_BAUD    = 9600;
        const uint8_t MODBUS_REG_PER  = 12;     // Regs count to fetch in each block request.
        const uint8_t MODBUS_REGMAX   = 12;     // Number of registers to be fetched/ We only need 56 but have to fetch in blocks of 12 from the EM21 => Since learned this may be because of the default 64 byte buffer on SoftwareSerial.
        const uint8_t mbDataElementCount = 12;  // Number of parameters these registers fit into.

        const uint16_t mbNameIndex[] = {0,1,2,3,4,5,6,7,8,21,22,27};
        const int mbScale[mbDataElementCount] =     {  1,1,1,  1,1,1,  1,1,1,  1,1,1 };
        const byte mbRegSize[mbDataElementCount] =  {  1,1,1,  1,1,1,  1,1,1,  1,1,1 };
        const byte mbDP[mbDataElementCount] =       {  1,1,1,  1,1,1,  1,1,1,  1,1,1 };
    
    #elif defined(MODBUS_DEVICE_APM403)
        const uint8_t MODBUS_ID       = 1;
        const uint8_t MODBUS_HBASE    = 1036;
        const uint16_t MODBUS_BAUD    = 9600;
        const uint8_t MODBUS_REG_PER  = 50;     // Regs count to fetch in each block request.
        const uint8_t MODBUS_REGMAX   = 18;     // Number of registers to be fetched/ We only need 56 but have to fetch in blocks of 12 from the EM21 => Since learned this may be because of the default 64 byte buffer on SoftwareSerial.
        const uint8_t mbDataElementCount = 18;  // Number of parameters these registers fit into.

        const uint16_t mbNameIndex[] = {29,0,1,2,3,4,5,6,7,8,9,32,33,34,35,36,37,38};
        const int mbScale[mbDataElementCount] =     { 10,  1,1,1,  1,1,1,  1,1,1,1,  10,  1,1,1,  1,1,1 };
        const byte mbRegSize[mbDataElementCount] =  {  1,  1,1,1,  1,1,1,  1,1,1,1,   1,  1,1,1,  1,1,1 };
        const byte mbDP[mbDataElementCount] =       {  1,  1,1,1,  1,1,1,  1,1,1,1,   1,  1,1,1,  1,1,1 };

    #elif defined(MODBUS_DEVICE_DSE)    
        const uint8_t MODBUS_ID       = 1;
        const uint8_t MODBUS_HBASE    = 0;
        const uint16_t MODBUS_BAUD    = 9600;
        const uint8_t MODBUS_REG_PER  = 12;     // Regs count to fetch in each block request.
        const uint8_t MODBUS_REGMAX   = 12;     // Number of registers to be fetched/ We only need 56 but have to fetch in blocks of 12 from the EM21 => Since learned this may be because of the default 64 byte buffer on SoftwareSerial.
        const uint8_t mbDataElementCount = 12;  // Number of parameters these registers fit into.

        const uint16_t mbNameIndex[] = {0,1,2,3,4,5,6,7,8,21,22,27};
        const int mbScale[mbDataElementCount] =     {  1,1,1,  1,1,1,  1,1,1,  1,1,1 };
        const byte mbRegSize[mbDataElementCount] =  {  1,1,1,  1,1,1,  1,1,1,  1,1,1 };
        const byte mbDP[mbDataElementCount] =       {  1,1,1,  1,1,1,  1,1,1,  1,1,1 };
    
    #endif
    
    // What buffer to use here? 256 is safe because JKBMS is ok with 310.
    // Is this why we could only fetch 12 registers at a time? (Maybe not bcuase rPI can only do 6 Regs at a time.)
    // But that's 24 bytes and SoftwareSerial default is 64? To be tested on an EM21.

    //SoftwareSerial S(mb_RX_PIN, mb_TX_PIN, false); //,256); // *TODO* Work out the correct buffer size and fix the MODBUS_REG_PER values.
    SoftwareSerial S(mb_RX_PIN, mb_TX_PIN,false,256); // *TODO* Work out the correct buffer size and fix the MODBUS_REG_PER values.
    ModbusRTU mb;

    const uint8_t mbRegCount = MODBUS_REGMAX;
    uint16_t mbResult[mbRegCount];      // Register size is 16 bit. Some values use 2 registers.

    // Jiggery Pokery to convert uint32 to float
    // by pointing the two data types at the same address.
    mbData cvD;                         // uint32
    //float * cvDFlt = &cvD.d1;         // Should work without recast? implied?
    float * cvDFlt = (float *) &cvD;    // Specifically cast as address to a float.

    Modbus::ResultCode cbResult;

    bool modbusSuccess = false;         // Used by debug page to determine modbus read outcome
    String modbus_LastResult = "";      // For displaying on the debug page.
    String mbCall ="";

    bool mbCallback(Modbus::ResultCode event, uint16_t transactionId, void* mbdata) { // Callback to monitor errors
        cbResult = event;
        char hexStr[6];
        sprintf(hexStr, "%X ", cbResult);
        mbCall += FPSTR("In Callback: Result 0x"); mbCall += hexStr;        

        CONSOLE(F("  [mbCb] result: 0x")); CONSOLE(hexStr);
        if (event == Modbus::ResultCode::EX_TIMEOUT) CONSOLE(F(": Timeout..."));  // 0xe4
        CONSOLELN();

        if (cbResult != Modbus::ResultCode::EX_SUCCESS) {
            return false;  // Really not sure if the return value signals anything on a master read callback.
        }
        return true;
    }

    void initModbus(){
    	S.begin(MODBUS_BAUD);
        mb.begin(&S, mb_TX_EN_PIN); // TX_EN_PIN needed for non-Auto direction RS485 boards.
        mb.master();
    }

    // Returns TRUE if there was a successful handshake. See mbResult via callback for reply outcome.
    // Returns FALSE if the Modbus was busy and couldn't send.
    bool mbFetch(int baseReg, int numReg, uint16_t mbRegs[]){
        if (!mb.slave()) { // Is the wire busy?
            mb.readHreg(MODBUS_ID, baseReg, mbRegs, numReg, mbCallback); // Send Read Hreg from Modbus Server
            //mb.readHreg(MODBUS_ID, 1054, mbRegs, 2, mbCallback);       // Send Read Hreg from Modbus Server
            while(mb.slave()) { mb.task(); delay(10); }                 // Wait for send and receive reply to take place.
            return true;                                                // Complete or timed out. See the callback for results.
        }
        return false;                                                   // MODBUS Busy - couldn't send request.
    }

    uint32_t reg2uint32(uint16_t reg[]){
        uint32_t result = reg[0] + reg[1] * 65536;
        //memcpy(&result, reg, sizeof(uint32_t));
        return result;
    }

    const __FlashStringHelper * getmbErrorStr(Modbus::ResultCode errCode){
        switch (errCode)
        {
            case Modbus::EX_SUCCESS                     : return FPSTR(mbErrorStr::mbrc00);
            case Modbus::EX_ILLEGAL_FUNCTION            : return FPSTR(mbErrorStr::mbrc01);
            case Modbus::EX_ILLEGAL_ADDRESS             : return FPSTR(mbErrorStr::mbrc02);
            case Modbus::EX_ILLEGAL_VALUE               : return FPSTR(mbErrorStr::mbrc03);
            case Modbus::EX_SLAVE_FAILURE               : return FPSTR(mbErrorStr::mbrc04);
            case Modbus::EX_ACKNOWLEDGE                 : return FPSTR(mbErrorStr::mbrc05);
            case Modbus::EX_SLAVE_DEVICE_BUSY           : return FPSTR(mbErrorStr::mbrc06);
            case Modbus::EX_MEMORY_PARITY_ERROR         : return FPSTR(mbErrorStr::mbrc08);
            case Modbus::EX_PATH_UNAVAILABLE            : return FPSTR(mbErrorStr::mbrc0A);
            case Modbus::EX_DEVICE_FAILED_TO_RESPOND    : return FPSTR(mbErrorStr::mbrc0B);
            case Modbus::EX_GENERAL_FAILURE             : return FPSTR(mbErrorStr::mbrcE1);
            case Modbus::EX_DATA_MISMACH                : return FPSTR(mbErrorStr::mbrcE2);
            case Modbus::EX_UNEXPECTED_RESPONSE         : return FPSTR(mbErrorStr::mbrcE3);
            case Modbus::EX_TIMEOUT                     : return FPSTR(mbErrorStr::mbrcE4);
            case Modbus::EX_CONNECTION_LOST             : return FPSTR(mbErrorStr::mbrcE5);
            case Modbus::EX_CANCEL                      : return FPSTR(mbErrorStr::mbrcE6);
            case Modbus::EX_PASSTHROUGH                 : return FPSTR(mbErrorStr::mbrcE7);
            case Modbus::EX_FORCE_PROCESS               : return FPSTR(mbErrorStr::mbrcE8);
        }
        return FPSTR(mbErrorStr::mbrcXX);
    }

    // Main work done here.
    bool doModbusWork(){
        modbus_LastResult = "";
        mbCall = "";

        // We can only fetch MODBUS_REG_PER registers at a time :-(
        uint16_t i=0;    
        uint16_t regfetch = MODBUS_REG_PER;
        while (i < MODBUS_REGMAX){
            if (!(i + regfetch < MODBUS_REGMAX)) {
                regfetch = MODBUS_REGMAX - i;
            }
            mbCall += FPSTR("Fetch request: i="); mbCall += i;
            mbCall += FPSTR(", Base: "); mbCall += (MODBUS_HBASE + i);
            mbCall += FPSTR(", RegCount="); mbCall += regfetch;
            mbCall += FPSTR(", OutOf="); mbCall += MODBUS_REGMAX; mbCall += " >> ";

            bool result = mbFetch(i + MODBUS_HBASE, regfetch, &mbResult[i]);            
            if (result) { // Handshake ok.
                if (cbResult == Modbus::ResultCode::EX_SUCCESS ){
                    mbCall += FPSTR("mFetch ok: "); mbCall += i; mbCall += "\n";
                } else {
                    mbCall += FPSTR("mbFetch error: "); mbCall += getmbErrorStr(cbResult);
                    modbus_LastResult = mbCall;
                    CONSOLELN(mbCall);
                    return false;
                }
            } else { // Modbus Busy.
                mbCall += FPSTR("mbFetch failed: MODBUS busy.\n");
                modbus_LastResult = mbCall;
                CONSOLELN(mbCall);
                return false;
            }
            i += regfetch;
        }
        mbCall += FPSTR("Done Fetching AOK!\n");

        CONSOLELN(FPSTR("Done Handshake - mbCall:"));
        CONSOLE(mbCall);
        
        // Are we done with this info now? Call was successful so the rest is data wrangling?
        mbCall.replace("\n","<br>");
        modbus_LastResult = mbCall;
        modbus_LastResult += FPSTR("<table class=\"table\"><tbody>");

        // Convert the register buffer values to useful data.
        uint16_t regAdr = 0;
        char dbuff[10];
        char *mbDN[mbDataElementCount];
        for (uint16_t i=0; i<mbDataElementCount; i++){
            if (mbRegSize[i] == 2) {
                cvDFlt[i] = reg2uint32(&mbResult[regAdr]) / ((float) mbScale[i]);
            }
            if (mbRegSize[i] == 1) {        
                cvDFlt[i] = mbResult[regAdr] / ((float) mbScale[i]);
            }
            regAdr += mbRegSize[i];
            mbDN[i] = (char *)mbDataNames::mbDataNames[mbNameIndex[i]];  // We get the Field names from our Global list indexed into by our device definition.
            modbus_LastResult += FPSTR("<tr><td>");
            modbus_LastResult += FPSTR(mbDN[i]);
            modbus_LastResult += FPSTR("</td><td>");
            
            //Bug with this when compiling for esp32. Maybe broken for 8266 now?
            //modbus_LastResult += String(cvDFlt[i],mbDP[i]);
            dtostrf(cvDFlt[i],1,mbDP[i],dbuff);  // dtostrf(float, min width, precision, stringbuffer)
            modbus_LastResult += dbuff;
            
            modbus_LastResult += FPSTR("</td></tr>");
        }
        modbus_LastResult += FPSTR("</tbody></table>");
    
        CONSOLE(F("Emoncms sending mbData..."));
        emoncms::send2emoncms(mbDN,(float *) &cvD, mbDataElementCount);
        return true;
    }

#endif