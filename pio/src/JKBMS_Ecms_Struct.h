#ifndef JKBMS_ECMS_STRUCT_H 
    #define JKBMS_ECMS_STRUCT_H

    #include "Arduino.h"

#define jkbms_DataElementCount 30  // Number of parameters these fit into.

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

    const char AvCellV[] PROGMEM = "AvCellV";
    const char CellDif[] PROGMEM = "CellDif";
    const char BalI[]    PROGMEM = "BalmA";

    const char V[]       PROGMEM = "V";
    const char I[]       PROGMEM = "I";

    const char SOC[]     PROGMEM = "SOC";
    const char BalAct[]  PROGMEM = "BalAct";
    const char ChAct[]   PROGMEM = "ChAct";

    const char DisAct[]  PROGMEM = "DisAct";
    const char e5[]      PROGMEM = "e5";
        
     // Array of the above names   
    const char * const jkDataNames[jkbms_DataElementCount] = {
            c1,c2,c3,c4,c5,c6,c7,c8,c9,c10,c11,c12,c13,c14,c15,c16,c17,c18,c19,c20, //c21,c22,c23,c24,
            AvCellV, CellDif, BalI,
            V,I,SOC,BalAct,ChAct,DisAct,e5
    };

    struct jkData {
        float c1,c2,c3,c4,c5,c6,c7,c8,c9,c10,c11,c12,c13,c14,c15,c16,c17,c18,c19,c20; //c21,c22,c23,c24;
        float AvCellV, CellDif, BalI;
        float V,I,SOC,BalAct,ChAct,DisAct,e5;
    };

    jkData jkD;

#endif