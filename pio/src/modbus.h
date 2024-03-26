#pragma once
#ifndef pgMODBUS_H
    #define pgMODBUS_H

    #include "Arduino.h"
    #include <ModbusRTU.h>
    #include "SoftwareSerial.h"

    #include "defs.h"
    #include "emoncms.h"

    #include <stdlib.h>

   
    //settings for EM21 Modbus
    //These need turning into User Parameters
    #define EM21_ID         1
    #define EM21_HBASE      0
    #define EM21_BAUD       9600
    #define EM21_REG_PER    12          // Regs count to fetch in each block request.
    #define EM21_REGMAX     60          // Number of registers to be fetched/ We only need 56 but have to fetch in blocks of 12 from the EM21
    #define EM21_DataElementCount 31    // Number of parameters these registers fit into.

    //Settings for APM403 Control Panel.
    #define APM403_ID       1
    #define APM403_HBASE    1036
    #define APM403_BAUD     9600
    #define APM403_REG_PER  50          // Max regs that can be fetched in one request
    #define APM403_REGMAX   18          // Number of registers to be fetched
    #define APM403_DataElementCount 18  // Number of parameters these fit into.

    //Settings for APM303 Control Panel.
    #define APM303_ID       1
    #define APM303_HBASE    0
    #define APM303_BAUD     9600
    #define APM303_REG_PER  12          // Max regs that can be fetched in one request
    #define APM303_REGMAX   12          // Number of registers to be fetched
    #define APM303_DataElementCount 12  // Number of parameters these fit into.

    // #define READ_INTERVAL   10          // In Seconds.

    #ifdef MODBUS_DEVICE_EM21
        #define MODBUS_ID       EM21_ID
        #define MODBUS_HBASE    EM21_HBASE
        #define MODBUS_BAUD     EM21_BAUD
        #define MODBUS_REG_PER  EM21_REG_PER
        #define MODBUS_REGMAX   EM21_REGMAX   
        #define MODBUS_DATAELEMENTCOUNT EM21_DataElementCount
    #elif MODBUS_DEVICE_APM403
        #define MODBUS_ID       APM403_ID
        #define MODBUS_HBASE    APM403_HBASE
        #define MODBUS_BAUD     APM403_BAUD
        #define MODBUS_REG_PER  APM403_REG_PER
        #define MODBUS_REGMAX   APM403_REGMAX   
        #define MODBUS_DATAELEMENTCOUNT APM403_DataElementCount
    #elif MODBUS_DEVICE_APM303
        #define MODBUS_ID       APM303_ID
        #define MODBUS_HBASE    APM303_HBASE
        #define MODBUS_BAUD     APM303_BAUD
        #define MODBUS_REG_PER  APM303_REG_PER
        #define MODBUS_REGMAX   APM303_REGMAX   
        #define MODBUS_DATAELEMENTCOUNT APM303_DataElementCount
    #else
        // Some default values for the compile to succeed. Use APM303 for now.
        #define MODBUS_DEVICE_APM303

        #define MODBUS_ID       1
        #define MODBUS_HBASE    0
        #define MODBUS_BAUD     9600
        #define MODBUS_REG_PER  12
        #define MODBUS_REGMAX   12   
        #define MODBUS_DATAELEMENTCOUNT 12
    #endif

    #ifdef ESP8266
        #define mb_RX_PIN       D2
        #define mb_TX_PIN       D3
        #define mb_TX_EN_PIN    D0
    #else
        #define mb_RX_PIN       4
        #define mb_TX_PIN       0
        #define mb_TX_EN_PIN    16
    #endif

    const uint8_t mbRegCount = MODBUS_REGMAX;

    SoftwareSerial S(mb_RX_PIN, mb_TX_PIN);
    ModbusRTU mb;

    uint16_t mbResult[mbRegCount];      // Register size is 16 bit. Some values use 2 registers.
    const uint8_t mbDataElementCount = MODBUS_DATAELEMENTCOUNT;  // This is not the number of registers. It's the number of parameters (or the size of mbDataNames[] array below. )

    #ifdef MODBUS_DEVICE_EM21

        // Parameter names.
        const char d1[] PROGMEM = "v1";
        const char d2[] PROGMEM = "v2";
        const char d3[] PROGMEM = "v3";
        const char d4[] PROGMEM = "vL1L2";
        const char d5[] PROGMEM = "vL2L3";
        const char d6[] PROGMEM = "vL1L3";

        const char d7[] PROGMEM = "i1";
        const char d8[] PROGMEM = "i2";
        const char d9[] PROGMEM = "i3";

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
        const char d21[] PROGMEM = "pSum";
        const char d22[] PROGMEM = "VASum";
        const char d23[] PROGMEM = "VArSum";

        const char d24[] PROGMEM = "pf1";
        const char d25[] PROGMEM = "pf2";
        const char d26[] PROGMEM = "pf3";

        const char d27[] PROGMEM = "PFAvg";

        const char d28[] PROGMEM = "PhS";
        const char d29[] PROGMEM = "fHz";

        const char d30[] PROGMEM = "KwhT";
        const char d31[] PROGMEM = "KVArhT";
        
     // Array of the above names   
    const char * const mbDataNames[mbDataElementCount] = {
            d1,d2,d3,  d4,d5,d6,  d7,d8,d9,  d10,d11,d12,  d13,d14,d15,  d16,d17,d18,
            d19, d20,    d21, d22, d23,   d24,d25,d26,  d27,d28,d29,   d30, d31
    };

    struct mbData {
        float d1,d2,d3,d4,d5,d6,d7,d8,d9,d10,d11,d12,d13,d14,d15,d16,d17,d18;
        float d19, d20, d21, d22, d23, d24, d25, d26, d27, d28, d29, d30, d31;
    };

    const int mbScale[mbDataElementCount] = {
        10, 10, 10,             // Voltages L-N
        10, 10, 10,             // Voltages L-L
        1000, 1000, 1000,       // Currents
        10, 10, 10,             // Real Power
        10, 10, 10,             // VA
        10, 10, 10,             // VAr
        10,10,1,1,1, // Scale?  // vLNAvg, vLLAvg,pSum, VASum, VArSum
        1000, 1000, 1000, 1000, // PF1,2,3, PF avg 
        1, 1,                   // Phase Seq, Hz,
        10, 10                  // kwh Total, kVAr Total
    };

    // How many registers per parameter eg: 32bit Parameters need two registers etc.
    const byte mbRegSize[mbDataElementCount] = {2,2,2,  2,2,2,  2,2,2,  2,2,2,  2,2,2,  2,2,2,  2,2,2,2,2,  1,1,1, 1,1,1,  2,2};
    // How many decimal places to format display strings to.
    const byte mbDP[mbDataElementCount] =      {2,2,2,  2,2,2,  2,2,2,  2,2,2,  2,2,2,  2,2,2,  2,2,2,2,2,  3,3,3, 3,2,1,  2,2};
        
    #endif

    #ifdef MODBUS_DEVICE_APM303

        // Parameeter names.
        const char d1[] PROGMEM = "gL1";        // genset L1
        const char d2[] PROGMEM = "gL2";        // genset L2
        const char d3[] PROGMEM = "gL3";        // genset L3
        const char d4[] PROGMEM = "gL1L2"; 
        const char d5[] PROGMEM = "gL2L3";
        const char d6[] PROGMEM = "gL1L3";
        const char d7[] PROGMEM = "gI1";        //Currents
        const char d8[] PROGMEM = "gI2";
        const char d9[] PROGMEM = "gI3";
        //const char d10[] PROGMEM = "gIN";       // Neutral Current


        const char d10[] PROGMEM = "gkW";
        const char d11[] PROGMEM = "gkVA";
        const char d12[] PROGMEM = "gPF";      // bus Hz
        //const char d13[] PROGMEM = "bL1";       // Bus Voltages
        //const char d14[] PROGMEM = "bL2";
        //const char d15[] PROGMEM = "bL3";
        //const char d16[] PROGMEM = "bL1L2";
        //const char d17[] PROGMEM = "bL2L3";
        //const char d18[] PROGMEM = "bL1L3";
        //const char d1[] PROGMEM = "gfHz";       // genset Hz 

    // Array of the above names
    const char * const mbDataNames[mbDataElementCount] = {
            d1,d2,d3, d4,d5,d6, d7,d8,d9, d10,d11,d12  //, d13,d14,d15, d16,d17,d18
        };

    struct mbData {
        float d1, d2, d3, d4, d5, d6, d7, d8, d9, d10, d11, d12; //, d13, d14, d15, d16, d17, d18;
    };

    const int mbScale[mbDataElementCount] =     {  1,1,1,  1,1,1,  1,1,1,  1,1,1 };
    const byte mbRegSize[mbDataElementCount] =  {  1,1,1,  1,1,1,  1,1,1,  1,1,1 };
    const byte mbDP[mbDataElementCount] =       {  1,1,1,  1,1,1,  1,1,1,  1,1,1 };

    #endif

    #ifdef MODBUS_DEVICE_APM403

        // Parameeter names.
        const char d1[] PROGMEM = "gfHz";       // genset Hz 
        const char d2[] PROGMEM = "gL1";        // genset L1
        const char d3[] PROGMEM = "gL2";        // genset L2
        const char d4[] PROGMEM = "gL3";        // genset L3
        const char d5[] PROGMEM = "gL1L2"; 
        const char d6[] PROGMEM = "gL2L3";
        const char d7[] PROGMEM = "gL1L3";
        const char d8[] PROGMEM = "gI1";        //Currents
        const char d9[] PROGMEM = "gI2";
        const char d10[] PROGMEM = "gI3";
        const char d11[] PROGMEM = "gIN";       // Neutral Current

        const char d12[] PROGMEM = "bfHz";      // bus Hz
        const char d13[] PROGMEM = "bL1";       // Bus Voltages
        const char d14[] PROGMEM = "bL2";
        const char d15[] PROGMEM = "bL3";
        const char d16[] PROGMEM = "bL1L2";
        const char d17[] PROGMEM = "bL2L3";
        const char d18[] PROGMEM = "bL1L3";

    // Array of the above names
    const char * const mbDataNames[mbDataElementCount] = {
            d1, d2,d3,d4, d5,d6,d7,  d8,d9,d10,d11, d12, d13,d14,d15, d16,d17,d18
        };

    struct mbData {
        float d1, d2, d3, d4, d5, d6, d7, d8, d9, d10, d11, d12, d13, d14, d15, d16, d17, d18;
    };

    const int mbScale[mbDataElementCount] =     { 10,  1,1,1,  1,1,1,  1,1,1,1,  10,  1,1,1,  1,1,1 };
    const byte mbRegSize[mbDataElementCount] =  {  1,  1,1,1,  1,1,1,  1,1,1,1,   1,  1,1,1,  1,1,1 };
    const byte mbDP[mbDataElementCount] =       {  1,  1,1,1,  1,1,1,  1,1,1,1,   1,  1,1,1,  1,1,1 };

    #endif
    
    // Jiggery Pokery to convert uint32 to float
    // by pointing the two data types at the same address.
    mbData cvD;                         // uint32
    //float * cvDFlt = &cvD.d1;         // Should work without recast? implied?
    float * cvDFlt = (float *) &cvD;    // Specifically cast as address to a float.

    bool mbRequestOK = false;

    bool modbusSuccess = false;         // Used by debug page to determine modbus read outcome
    String modbus_LastResult = "";      // For displaying on the debug page.

    bool cb(Modbus::ResultCode event, uint16_t transactionId, void* mbdata) { // Callback to monitor errors
        if (event != Modbus::EX_SUCCESS) {
            CONSOLE(F("Request result: 0x")); CONSOLE(event, HEX); CONSOLE(" ");
            modbus_LastResult = FPSTR("Request result: 0x");
            modbus_LastResult += String(event,HEX);

            if (event == Modbus::ResultCode::EX_TIMEOUT) CONSOLE(F("Timeout..."));  // 0xe4

            mbRequestOK = false;
            return false;
        }
        mbRequestOK = true;
        return true;
    }

    void initModbus(){
        S.begin(MODBUS_BAUD, SWSERIAL_8N1);
        mb.begin(&S, mb_TX_EN_PIN);
        mb.master();
    }

    bool mbFetch(int baseReg, int numReg, uint16_t mbRegs[]){
        if (!mb.slave()) {                                    // Check if no transaction in progress
            CONSOLE(F("mbRead Request: ID: ")); CONSOLE(MODBUS_ID); CONSOLE(F(", Base: ")); CONSOLE(baseReg); CONSOLE(F(", Count: ")); CONSOLE(numReg); CONSOLE(F(" >> "));
            mb.readHreg(MODBUS_ID, baseReg, mbRegs, numReg, cb); // Send Read Hreg from Modbus Server
            //mb.readHreg(MODBUS_ID, 1054, mbRegs, 2, cb); // Send Read Hreg from Modbus Server
            
            while(mb.slave()) { // Check if transaction is active // Timeout??? Runs forever?
                mb.task();
                delay(10);
            }
            return true; // MODBUS wasn't busy but the fetch may have failed. See the callback result.
        }
        CONSOLELN(F("mbRead Request: MODBUS busy.."));
        return false;   // MODBUS Busy - transaction in progress.
    }

    uint32_t reg2uint32(uint16_t reg[]){
        uint32_t result = reg[0] + reg[1] * 65536;
        //memcpy(&result, reg, sizeof(uint32_t));
        return result;
    }

    // Main work done here.
    bool doModbusWork(){
        CONSOLELN(F("###################################"));

        // We can only fetch MODBUS_REG_PER registers at a time :-(
        char dbuff[10];
        int i=0;    
        int regfetch = MODBUS_REG_PER;
        while (i < MODBUS_REGMAX){
            if (!(i + regfetch < MODBUS_REGMAX)) {
                regfetch = MODBUS_REGMAX - i;
            }
            CONSOLE(F("Fetch request: i=")); CONSOLE(i); CONSOLE(F(", size=")); CONSOLE(regfetch); CONSOLE(F(", total=")); CONSOLE(MODBUS_REGMAX); CONSOLE(F(", "));
            bool result = mbFetch(i + MODBUS_HBASE, regfetch, &mbResult[i]);            
            if (result) {
                if (mbRequestOK){
                    CONSOLE(F("Fetch ok: ")); CONSOLELN(i); 
                } else {
                    CONSOLELN(F("MODBUS Fetch Failed. "));
                    return false;
                }
            } else {
                CONSOLE(F("MODBUS busy, Fetch Failed: ")); CONSOLELN(i); 
                return false;
            }
            i += regfetch;
        }
        CONSOLELN(F("Done Fetching AOK!"));

        modbus_LastResult = FPSTR("<table class=\"table\"><tbody>");
        int regAdr = 0;
        for (int i=0; i<mbDataElementCount; i++){
            if (mbRegSize[i] == 2) {
                cvDFlt[i] = reg2uint32(&mbResult[regAdr]) / ((float) mbScale[i]);
            }
            if (mbRegSize[i] == 1) {        
                cvDFlt[i] = mbResult[regAdr] / ((float) mbScale[i]);
            }
            regAdr += mbRegSize[i];
            modbus_LastResult += FPSTR("<tr><td>");
            modbus_LastResult += FPSTR(mbDataNames[i]);
            modbus_LastResult += FPSTR("</td><td>");
            
            //Bug with this when compiling for esp32. Maybe broken for 8266 now?
            //modbus_LastResult += String(cvDFlt[i],mbDP[i]);
            dtostrf(cvDFlt[i],4,mbDP[i],dbuff);  // dtostrf(float, min width, precision, stringbuffer)
            modbus_LastResult += dbuff;
            
            modbus_LastResult += FPSTR("</td></tr>");
        }
        modbus_LastResult += FPSTR("</tbody></table>");
    
        CONSOLE(F("Emoncms sending mbData..."));
        emoncms::send2emoncms(mbDataNames,(float *) &cvD, mbDataElementCount);
        return true;
    }

#endif