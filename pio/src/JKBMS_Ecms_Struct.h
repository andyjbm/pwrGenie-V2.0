#ifndef JKBMS_ECMS_STRUCT_H 
    #define JKBMS_ECMS_STRUCT_H

    #include "Arduino.h"

#define jkbms_DataElementCount 27  // Number of parameters these fit into.

// Parameter names.
    const char c1[] PROGMEM = "C1";
    const char c2[] PROGMEM = "C2";
    const char c3[] PROGMEM = "C3";
    const char c4[] PROGMEM = "C4";
    const char c5[] PROGMEM = "C5";
    const char c6[] PROGMEM = "C6";
    const char c7[] PROGMEM = "C7";
    const char c8[] PROGMEM = "C8";
    const char c9[] PROGMEM = "C9";
    const char c10[] PROGMEM = "C10";
    const char c11[] PROGMEM = "C11";
    const char c12[] PROGMEM = "C12";
    const char c13[] PROGMEM = "C13";
    const char c14[] PROGMEM = "C14";
    const char c15[] PROGMEM = "C15";
    const char c16[] PROGMEM = "C16";
    const char c17[] PROGMEM = "C17";
    const char c18[] PROGMEM = "C18";
    const char c19[] PROGMEM = "C19";
    const char c20[] PROGMEM = "C20";
//    const char c21[] PROGMEM = "C21";
//    const char c22[] PROGMEM = "C22";
//    const char c23[] PROGMEM = "C23";
//    const char c24[] PROGMEM = "C24";

    const char V[] PROGMEM = "V";
    const char I[] PROGMEM = "I";

    const char e1[] PROGMEM = "e1";

    const char e2[] PROGMEM = "e2";
    const char e3[] PROGMEM = "e3";

    const char e4[] PROGMEM = "e4";
    const char e5[] PROGMEM = "e5";
        
     // Array of the above names   
    const char * const jkDataNames[jkbms_DataElementCount] = {
            c1,c2,c3,c4,c5,c6,c7,c8,c9,c10,c11,c12,c13,c14,c15,c16,c17,c18,c19,c20, //c21,c22,c23,c24,
            V,I,e1,e2,e3,e4,e5
    };

    struct jkData {
        float c1,c2,c3,c4,c5,c6,c7,c8,c9,c10,c11,c12,c13,c14,c15,c16,c17,c18,c19,c20; //c21,c22,c23,c24;
        float V,I,e1,e2,e3,e4,e5;
    };

    // const int jkScale[jkbms_DataElementCount] = {
    //     10, 10, 10,             // Voltages L-N
    //     10, 10, 10,             // Voltages L-L
    //     1000, 1000, 1000,       // Currents
    //     10, 10, 10,             // Real Power
    //     10, 10, 10,             // VA
    //     10, 10, 10,             // VAr
    //     10,10,1,1,1, // Scale?  // vLNAvg, vLLAvg,pSum, VASum, VArSum
    //     1000, 1000, 1000, 1000, // PF1,2,3, PF avg 
    //     1, 1,                   // Phase Seq, Hz,
    //     10, 10                  // kwh Total, kVAr Total
    // };

    jkData jkD;

#endif