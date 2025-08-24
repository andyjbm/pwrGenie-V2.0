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


    // This is an friendly-named index into the datanames arrays below
    // so as not to have to use d14, r10142, d21, d36, r1043 in the code
    // which isn't very helpful.
    // It *MUST* be kept in sync positionally with the arrays below!
    enum indexDataNames{ vL1,vL2,VL3, vL1L2,vL2L3,vL1L3, i1,i2,i3,iN,   //0 to 9
                        p1,p2,p3, VA1,VA2,VA3, VAr1,VAr2,VAr3,          //10 to 18
                        vLNAvg,vLLAvg, kW,kVA,kVAr,                     //19 to 23
                        pf1,pf2,pf3,pfAvg,                              //24 to 27
                        PhS,fHz, KWhT,kVArT,                            //28 t0 31
                        bfHz, bL1,bL2,bL3, bL1L2,bL2L3,bL1L3,           //32 to 38
                        
                        vBatt1,vBatt2, v12Alt,                          //39 to 41
                        LpH,Loadpc,Runtime,                             //42 to...
                        CoolTemp,tIntManifold,POil,PIntake,
                        SootLoad,AshLoad,DevTank,Fuelpc,                //...52
                        RPM1,RPM2,RPM3,Hz1,Hz2,Hz3,RPM4,                 //52 to 59
                        
                        d60,d61,d62,d63,d64,d65,d66,d67,d68,d69, d70,d71,d72,d73,

                        r1000,r1001,r1002,r1003,r1004,r1005,r1006,r1007,r1008,r1009,
                        r1010,r1011,r1012,r1013,r1014,r1015,r1016,r1017,r1018,r1019,
                        r1020,r1021,r1022,r1023,r1024,r1025,r1026,r1027,r1028,r1029,
                        r1030,r1031,r1032,r1033,r1034,r1035,r1036,r1037,r1038,r1039,
                        r1040,r1041,r1042,r1043,r1044,r1045,r1046,r1047,r1048,r1049,
                        r1050,r1051,r1052,r1053,r1054,r1055,r1056,r1057,r1058,r1059,
                        r1060,r1061,r1062,r1063,r1064,r1065,r1066,r1067,r1068,r1069,
                        r1070,r1071,r1072,r1073        
    };

    namespace mbDataNames{
        // Parameter names. Across all devices.
        // These are home grown once we know what a parameter is called!
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

        const char d39[] PROGMEM = "vBatt1";
        const char d40[] PROGMEM = "vBatt2";
        const char d41[] PROGMEM = "12vAlt";
        
        const char d42[] PROGMEM = "L-h";
        const char d43[] PROGMEM = "Load";
        const char d44[] PROGMEM = "Runtime";
        const char d45[] PROGMEM = "Coolant Temp";        //80
        const char d46[] PROGMEM = "t-IntManifold";
        const char d47[] PROGMEM = "P-Oil";
        const char d48[] PROGMEM = "P-Intake";
        const char d49[] PROGMEM = "SootLoad";
        const char d50[] PROGMEM = "AshLoad";             //90
        const char d51[] PROGMEM = "DevTank";
        const char d52[] PROGMEM = "FuelTank";
        const char d53[] PROGMEM = "RPM1"; 
        const char d54[] PROGMEM = "RPM2"; 
        const char d55[] PROGMEM = "RPM3"; 
        const char d56[] PROGMEM = "Hz1"; 
        const char d57[] PROGMEM = "Hz2"; 
        const char d58[] PROGMEM = "Hz3"; 
        const char d59[] PROGMEM = "RPM4"; 
        const char d60[] PROGMEM = "d60";
        const char d61[] PROGMEM = "d61";
        const char d62[] PROGMEM = "d62";
        const char d63[] PROGMEM = "d63";
        const char d64[] PROGMEM = "d64";
        const char d65[] PROGMEM = "d65";
        const char d66[] PROGMEM = "d66";
        const char d67[] PROGMEM = "d67";
        const char d68[] PROGMEM = "d68";
        const char d69[] PROGMEM = "d69";
        const char d70[] PROGMEM = "d70";
        const char d71[] PROGMEM = "d71";
        const char d72[] PROGMEM = "d72";
        const char d73[] PROGMEM = "d73";

        // These are default MODBUS Register names
        // for things we have not identified yet.
        // We can quickly rename these as we are reverse engineering
        // a device but then ideally rename them back to their reg number
        // and create a new parameter name in the list above.
        const char r1000[] PROGMEM = "r1000";        //74
        const char r1001[] PROGMEM = "r1001";
        const char r1002[] PROGMEM = "r1002";
        const char r1003[] PROGMEM = "r1003";
        const char r1004[] PROGMEM = "r1004";
        const char r1005[] PROGMEM = "r1005";
        const char r1006[] PROGMEM = "r1006";        //80
        const char r1007[] PROGMEM = "r1007";
        const char r1008[] PROGMEM = "r1008";
        const char r1009[] PROGMEM = "r1009";
        const char r1010[] PROGMEM = "r1010";
        const char r1011[] PROGMEM = "r1011";
        const char r1012[] PROGMEM = "r1012";
        const char r1013[] PROGMEM = "r1013";
        const char r1014[] PROGMEM = "r1014";
        const char r1015[] PROGMEM = "r1015";
        const char r1016[] PROGMEM = "r1016";        //90
        const char r1017[] PROGMEM = "r1017";
        const char r1018[] PROGMEM = "r1018";
        const char r1019[] PROGMEM = "r1019";
        const char r1020[] PROGMEM = "r1020";
        const char r1021[] PROGMEM = "r1021";
        const char r1022[] PROGMEM = "r1022";
        const char r1023[] PROGMEM = "r1023";
        const char r1024[] PROGMEM = "r1024";
        const char r1025[] PROGMEM = "r1025";
        const char r1026[] PROGMEM = "r1026";       //100
        const char r1027[] PROGMEM = "r1027";
        const char r1028[] PROGMEM = "r1028";
        const char r1029[] PROGMEM = "r1029";
        const char r1030[] PROGMEM = "r1030";
        const char r1031[] PROGMEM = "r1031";
        const char r1032[] PROGMEM = "r1032";
        const char r1033[] PROGMEM = "r1033";
        const char r1034[] PROGMEM = "r1034";
        const char r1035[] PROGMEM = "r1035";
        const char r1036[] PROGMEM = "r1036";
        const char r1037[] PROGMEM = "r1037";
        const char r1038[] PROGMEM = "r1038";
        const char r1039[] PROGMEM = "r1039";
        const char r1040[] PROGMEM = "r1040";
        const char r1041[] PROGMEM = "r1041";
        const char r1042[] PROGMEM = "r1042";
        const char r1043[] PROGMEM = "r1043";
        const char r1044[] PROGMEM = "r1044";
        const char r1045[] PROGMEM = "r1045";
        const char r1046[] PROGMEM = "r1046";
        const char r1047[] PROGMEM = "r1047";
        const char r1048[] PROGMEM = "r1048";
        const char r1049[] PROGMEM = "r1049";
        const char r1050[] PROGMEM = "r1050";
        const char r1051[] PROGMEM = "r1051";
        const char r1052[] PROGMEM = "r1052";
        const char r1053[] PROGMEM = "r1053";
        const char r1054[] PROGMEM = "r1054";
        const char r1055[] PROGMEM = "r1055";
        const char r1056[] PROGMEM = "r1056";
        const char r1057[] PROGMEM = "r1057";
        const char r1058[] PROGMEM = "r1058";
        const char r1059[] PROGMEM = "r1059";
        const char r1060[] PROGMEM = "r1060";
        const char r1061[] PROGMEM = "r1061";
        const char r1062[] PROGMEM = "r1062";
        const char r1063[] PROGMEM = "r1063";
        const char r1064[] PROGMEM = "r1064";
        const char r1065[] PROGMEM = "r1065";
        const char r1066[] PROGMEM = "r1066";
        const char r1067[] PROGMEM = "r1067";
        const char r1068[] PROGMEM = "r1068";
        const char r1069[] PROGMEM = "r1069";
        const char r1070[] PROGMEM = "r1070";
        const char r1071[] PROGMEM = "r1071";
        const char r1072[] PROGMEM = "r1072";
        const char r1073[] PROGMEM = "r1073";  //147

        // This is an index of the datanames above. There should be *EXACTLY*
        // the same *number of elements* as there are parameter names above.
        const char * const mbDataNames[] = {
            d0,d1,d2,d3,d4,d5,d6,d7,d8,d9,d10,d11,d12,d13,d14,d15,d16,d17,d18,d19,
            d20,d21,d22,d23,d24,d25,d26,d27,d28,d29,d30,d31,d32,d33,d34,d35,d36,d37,d38,d39,
            d40,d41,d42,d43,d44,d45,d46,d47,d48,d49,d50,d51,d52,d53,d54,d55,d56,d57,d58,d59,
            d60,d61,d62,d63,d64,d65,d66,d67,d68,d69,d70,d71,d72,d73,

            r1000,r1001,r1002,r1003,r1004,r1005,r1006,r1007,r1008,r1009,  //74
            r1010,r1011,r1012,r1013,r1014,r1015,r1016,r1017,r1018,r1019,  //84
            r1020,r1021,r1022,r1023,r1024,r1025,r1026,r1027,r1028,r1029,  //94
            r1030,r1031,r1032,r1033,r1034,r1035,r1036,r1037,r1038,r1039,  //104
            r1040,r1041,r1042,r1043,r1044,r1045,r1046,r1047,r1048,r1049,  //114
            r1050,r1051,r1052,r1053,r1054,r1055,r1056,r1057,r1058,r1059,  //124
            r1060,r1061,r1062,r1063,r1064,r1065,r1066,r1067,r1068,r1069,  //134
            r1070,r1071,r1072,r1073                                       //144
        };
    };

    #ifdef MODBUS_DEVICE_EM21
//        const uint8_t MODBUS_ID       = 1;        // Set on config page
        const uint16_t MODBUS_HBASE    = 0;
        const uint16_t MODBUS_BAUD    = 9600;
        const uint8_t MODBUS_REG_PER  = 12;         // Regs count to fetch in each block request.
        const uint8_t MODBUS_REGMAX   = 60;         // Number of registers to be fetched/ We only need 56 but have to fetch in blocks of 12 from the EM21 => Since learned this may be because of the default 64 byte buffer on SoftwareSerial.
        const uint8_t mbDataElementCount = 31;   // Number of parameters these registers fit into.
        const uint8_t mbResult2SendCount = 31;   // The number of values from what we collected to send to EMONCMS.

        // Array of the names for EM21 - in MODBUS Register order.  
        const uint16_t mbNameIndex[] = {
            //0,1,2,3,4,5,6,7,8,  10,11,12, 13,14,15, 16,17,18, 19,20, 21,22,23, 24,25,26, 27,28,29, 30,31
            vL1,vL2,VL3, vL1L2,vL2L3,vL1L3, i1,i2,i3, p1,p2,p3, VA1,VA2,VA3, VAr1,VAr2,VAr3,
            vLNAvg,vLLAvg, kW,kVA,kVAr, pf1,pf2,pf3,pfAvg, PhS,fHz, KWhT,kVArT
            };
 
        //Map used to pick the dataElements we want and in the order we want.
        const byte mbResultMap[mbResult2SendCount] = {
            0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30
        };

        const int mbScale[mbDataElementCount] = {
            10,10,10,  10,10,10,  1000,1000,1000,  10,10,10, 10,10,10, 
            10,10,10,  10,10, 1000,1000,1000, 1000,1000,1000,1000, 1, 1, 10, 10 
        };

        // How many registers per parameter eg: 32bit Parameters need two registers etc.
        const byte mbRegSize[mbDataElementCount] = {2,2,2,  2,2,2,  2,2,2,  2,2,2,  2,2,2,  2,2,2,  2,2,2,2,2,  1,1,1, 1,1,1,  2,2};
        // How many decimal places to format display strings to.
        const byte mbDP[mbDataElementCount] =      {2,2,2,  2,2,2,  2,2,2,  2,2,2,  2,2,2,  2,2,2,  2,2,2,2,2,  3,3,3, 3,2,1,  2,2};
    
    #elif defined(MODBUS_DEVICE_APM303)    
        //const uint8_t MODBUS_ID       = 1;       // Set on config page
        const uint16_t MODBUS_HBASE    = 0;
        const uint16_t MODBUS_BAUD    = 9600;
        const uint8_t MODBUS_REG_PER  = 12;     // Regs count to fetch in each block request.
        const uint8_t MODBUS_REGMAX   = 12;     // Number of registers to be fetched/ We only need 56 but have to fetch in blocks of 12 from the EM21 => Since learned this may be because of the default 64 byte buffer on SoftwareSerial.
        const uint8_t mbDataElementCount = 12;  // Number of parameters these registers fit into.
        const uint8_t mbResult2SendCount = 12;   // The number of values from what we collected to send to EMONCMS.

        const uint16_t mbNameIndex[] = {
            //0,1,2,3,4,5,6,7,8,21,22,27
            vL1,vL2,VL3, vL1L2,vL2L3,vL1L3, i1,i2,i3, kW,kVA, pfAvg
            };

        const byte mbResultMap[mbResult2SendCount] = { 0,1,2,3,4,5,6,7,8,9,10,11};
        const int mbScale[mbDataElementCount] =     {  1,1,1,  1,1,1,  1,1,1,  1,1,1 };
        const byte mbRegSize[mbDataElementCount] =  {  1,1,1,  1,1,1,  1,1,1,  1,1,1 };
        const byte mbDP[mbDataElementCount] =       {  1,1,1,  1,1,1,  1,1,1,  1,1,1 };
    
    #elif defined(MODBUS_DEVICE_APM403)
        //const uint8_t MODBUS_ID       = 1;       // Depreciated. Set on config page now.
        const uint16_t MODBUS_HBASE    = 1000; //1036;
        const uint16_t MODBUS_BAUD    = 9600;
        const uint8_t MODBUS_REG_PER  = 50;     // Regs count to fetch in each block request.
        const uint8_t MODBUS_REGMAX   = 74;     //38 //18   // Number of registers to be fetched.
    
        const uint8_t mbDataElementCount = 74;  //38 //18 // Number of parameters these registers fit into.

        const uint8_t mbResult2SendCount = 74;  // The number of values from what we collected to send to EMONCMS.

        // THese are arrays of parameter info that this device is going to collect and send to EMONCMS.
        const uint16_t mbNameIndex[] = {
            r1000,RPM1,r1002,r1003,RPM2,LpH,CoolTemp,tIntManifold,POil,PIntake, // r1000 to r1009
            Loadpc,r1011,r1012,r1013,Runtime,SootLoad,AshLoad,DevTank,Hz1,RPM3, // r1010 to r1019

            kW, p1,p2,p3, kVAr,VAr1,VAr2,VAr3, kVA,VA1,VA2,VA3,     // r1020 to r1031
            pfAvg, pf1,pf2,pf3,                                     // r1032 to r1035

            fHz, vL1,vL2,VL3, vL1L2,vL2L3,vL1L3, i1,i2,i3,iN,
            bfHz, bL1,bL2,bL3, bL1L2,bL2L3,bL1L3,

            vBatt1, v12Alt, r1056,r1057, Fuelpc, r1059, RPM4,
            r1061, r1062, r1063, r1064, r1065, r1066, r1067, r1068, r1069,
            vBatt2, r1071,r1072,r1073

            //29,0,1,2,3,4,5,6,7,8,9,32,33,34,35,36,37,38,                    //Reg 1036 to 1053
            //39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,    //Reg 1054 to 
            //59,60,61,62,63,64,65,66,67,68,69,70,71,72,73                    //Reg
        
        };

        //Map used to pick the dataElements we want and in the order we want.
        const byte mbResultMap[mbResult2SendCount] = {
            0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,
            20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,
            40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,
            60,61,62,63,64,65,66,67,68,69,70,71,72,73
        };

        const int mbScale[mbDataElementCount] =     { 
            1,1,1,1,1, 10, 1,1, 100,100, 1,  1,1,1,1,1,1,1,1,1, 1,1,1,1, 1,1,1,1, 1,1,1,1, 100,100,100,100,
            10,  1,1,1,  1,1,1,  1,1,1,1,  10,  1,1,1,  1,1,1, 10,10, 1,1, 1, 1,1,1,1,1,1, 1,1,1,1,1,10,1,1,1
            };
        const byte mbRegSize[mbDataElementCount] =  {
            1,1,1,1,1, 1,1,1,1,1,  1,1,1,1,1, 1,1,1,1,1, 1,1,1,1, 1,1,1,1, 1,1,1,1, 1,1,1,1,
            1, 1,1,1, 1,1,1, 1,1,1,1, 1, 1,1,1, 1,1,1, 1,1, 1,1, 1, 1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1
            };
        const byte mbDP[mbDataElementCount] =       {
            1,1,1,1,1,1,1,1,1,1,  1,1,1,1,1,1,1,1,1,1, 1,1,1,1, 1,1,1,1, 1,1,1,1, 2,2,2,2,
            1,  1,1,1,  1,1,1,  1,1,1,1, 1, 1,1,1, 1,1,1, 1,1, 1,1, 1, 1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1
            };

        const byte mbSignedInt[mbDataElementCount] ={
            0,0,0,0,0,0,0,0,0,0,                    // r1000 to r1009
            0,0,0,0,0,0,0,0,0,0,                    // r1010 to r1019

            0,0,0,0,  0,0,0,0,  0,0,0,0,  1,1,1,1,  // r1020 to r1035
            0, 0,0,0,  0,0,0,  0,0,0,0,
            0, 0,0,0,  0,0,0,

            0,0,0,0,0,0,0,
            0,0,0,0,0,0,0,0,0, // r1061 to r1069
            0,0,0,0            // r1070 to r1073
            };

    #elif defined(MODBUS_DEVICE_DSE)    
        //const uint8_t MODBUS_ID       = 1;       // Set on config page
        const uint16_t MODBUS_HBASE    = 0;
        const uint16_t MODBUS_BAUD    = 9600;
        const uint8_t MODBUS_REG_PER  = 12;     // Regs count to fetch in each block request.
        const uint8_t MODBUS_REGMAX   = 12;     // Number of registers to be fetched/ We only need 56 but have to fetch in blocks of 12 from the EM21 => Since learned this may be because of the default 64 byte buffer on SoftwareSerial.
        const uint8_t mbDataElementCount = 12;  // Number of parameters these registers fit into.
        const uint8_t mbResult2SendCount = 12;   // The number of values from what we collected to send to EMONCMS.

        const uint16_t mbNameIndex[] = {
            //0,1,2,3,4,5,6,7,8,21,22,27
            vL1,vL2,VL3, vL1L2,vL2L3,vL1L3, i1,i2,i3, kW,kVA, pfAvg
        };

        const byte mbResultMap[mbResult2SendCount] = { 0,1,2,3,4,5,6,7,8,9,10,11};
        const int mbScale[mbDataElementCount] =     {  1,1,1,  1,1,1,  1,1,1,  1,1,1 };
        const byte mbRegSize[mbDataElementCount] =  {  1,1,1,  1,1,1,  1,1,1,  1,1,1 };
        const byte mbDP[mbDataElementCount] =       {  1,1,1,  1,1,1,  1,1,1,  1,1,1 };
    #endif


    //###########################
    // Main MODBUS Code
    //###########################
    
    // What buffer to use here? 256 is safe because JKBMS is ok with 310.
    // Is this why we could only fetch 12 registers at a time? (Maybe not bcuase rPI can only do 6 Regs at a time.)
    // But that's 24 bytes and SoftwareSerial default is 64? To be tested on an EM21.

    //SoftwareSerial S(mb_RX_PIN, mb_TX_PIN, false); //,256); // *TODO* Work out the correct buffer size and fix the MODBUS_REG_PER values.
    SoftwareSerial S(mb_RX_PIN, mb_TX_PIN,false,256); // *TODO* Work out the correct buffer size and fix the MODBUS_REG_PER values.
    ModbusRTU mb;

    const uint8_t mbRegCount = MODBUS_REGMAX;
    uint16_t mbResult[mbRegCount];      // Register size is 16 bit. Some values use 2 registers.

    float cvD[mbDataElementCount];
    
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
        //mb.server(ecmsParams::ModbusID());       // Set on Device Config page.
    }

    // Returns TRUE if there was a successful handshake. See mbResult via callback for reply outcome.
    // Returns FALSE if the Modbus was busy and couldn't send.
    bool mbFetch(uint16_t baseReg, uint16_t numReg, uint16_t mbRegs[]){
    //    return true;  // For faking device present.
        if (!mb.slave()) { // Is the wire busy?
            mb.readHreg(ecmsParams::ModbusID(), baseReg, mbRegs, numReg, mbCallback); // Send Read Hreg from Modbus Server
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

        //Main MODBUS Fetch Registers Code
        //################################

        // This because (eg EM21) only allows for fetching "MODBUS_REG_PER" registers at a time :-(
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
        
        //Store the result codes from the modbus exchange for the debug html:
        mbCall.replace("\n","<br>");
        modbus_LastResult = mbCall;

        //and set up the debug html for the actual data values:
        modbus_LastResult += FPSTR("<table class=\"table\"><tbody>");

        // Convert the register buffer values to useful data.
        // A value may use 1 or 2 registers and may need scaling.
        //#######################################################
        char *mbDN[mbDataElementCount];
        float cvData[mbDataElementCount];
        uint16_t regAdr = 0;
        for (uint16_t i=0; i<mbDataElementCount; i++){
            if (mbRegSize[i] == 2) {

                //Two registers for this value
                if (mbSignedInt[i] == 0) { // It's not a signed int. 
                    cvData[i] = reg2uint32(&mbResult[regAdr]) / ((float) mbScale[i]);
                } else { // It's signed
                    cvData[i] = ((int16_t)reg2uint32(&mbResult[regAdr])) / ((float) mbScale[i]);
                }
            }
            if (mbRegSize[i] == 1) {  
                //Only one register for this value      
                cvData[i] = mbResult[regAdr] / ((float) mbScale[i]);
            }
            regAdr += mbRegSize[i];
        }

        //Register values are now translated to data in cvData.
        //Next, pick the results we want in the order we want.
        //####################################################
        for (uint16_t d=0; d<mbResult2SendCount; d++){
            byte s = mbResultMap[d];  // s is source index, d is destination.
                    
            //For emoncms:
            cvD[d] = cvData[s];
            mbDN[d] = (char *)mbDataNames::mbDataNames[mbNameIndex[s]];  // We get the Field names from our Global list indexed into by our device definition.

            //Build debug page html
            modbus_LastResult += FPSTR("<tr><td>");
            modbus_LastResult += FPSTR(mbDN[d]);
            modbus_LastResult += FPSTR("</td><td>");
            
            //Bug with this when compiling for esp32. So using dtostrf instead. Maybe broken for 8266 also?
            //modbus_LastResult += String(cvDFlt[i],mbDP[i]);
            char dbuff[10];
            dtostrf(cvData[s],1,mbDP[s],dbuff);  // dtostrf(float, min width, precision, stringbuffer)
            modbus_LastResult += dbuff;
            modbus_LastResult += FPSTR("</td></tr>");
        }
        modbus_LastResult += FPSTR("</tbody></table>");
    
        CONSOLE(F("Emoncms sending mbData..."));
        emoncms::send2emoncms(mbDN, cvD, mbDataElementCount);
        return true;
    }

#endif