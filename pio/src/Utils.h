#pragma once
#ifndef LLG_UTILS_H
  #define LLG_UTILS_H

  #include <Arduino.h>

  #if 0
  void validateInput(const char *input, char *output)
  {
    String tmp = input;
    tmp.trim();
    tmp.replace(' ', '_');
    tmp.toCharArray(output, tmp.length() + 1);
  }

  void validateInput(const char *input, String &output)
  {
    String tmp = input;
    tmp.trim();
    tmp.replace(' ', '_');
    output = tmp; //tmp.toCharArray(output, tmp.length() + 1);
  }
  
  char *validateCharStr(const char *input)
  {
    char output[MAXSIZE];
    String tmp = input;
    tmp.trim();
    tmp.replace(' ', '_');
    tmp.toCharArray(output, tmp.length() + 1);
    return output;
  }

  int validateInt(const char *input)
  {
    String tmp = input;
    tmp.trim();
    tmp.replace(',', '.');
    return tmp.toInt();
  }
  float validateFloat(const char *input)
  {
    String tmp = input;
    tmp.trim();
    tmp.replace(',', '.');
    return tmp.toFloat();
  }

#endif

  // String htmlencode(String str)
  // {
  //   String encodedstr = "";
  //   char c;

  //   for (uint16_t i = 0; i < str.length(); i++)
  //   {
  //     c = str.charAt(i);

  //     if (isalnum(c)) {
  //       encodedstr += c;
  //     } else {
  //       encodedstr += "&#";
  //       encodedstr += String((uint8_t)c);
  //       encodedstr += ';';
  //     }
  //   }
  //   return encodedstr;
  // }

  float ReadPsuVolts(float vfactor) {
    //return 100; // Temp while we debug wifi
    analogRead(A0); // Dump the 1st read because wifi also uses ADC.
    int ar = analogRead(A0);
    float psuVolts = ((float)(ar) / vfactor);
    float revEng = ((float)(ar) / psuVolts);
    CONSOLELN(F("AnalogRead:"), ar, F("psuVolts:"), psuVolts, F("vfactor:"), vfactor, F("Rev Eng:"), revEng);

    if (psuVolts < 0 || psuVolts > 99) psuVolts=999; // Check for broken config or psuVolts out of range.
    return psuVolts;
  }

//  void testhostByName(){
//  IPAddress remote_addr;  
//    if (WiFi.hostByName(((String)EMONCMS_DEFAULT_URL).c_str(), remote_addr)) {
//          CONSOLE(F("Remote addr: ")); CONSOLE(remote_addr);
//        }else{
//          CONSOLE(F("hostByName Failed..."));
//        }
//  }

//  uint32_t getS2MiniChipId(){
//    uint32_t chipId = 0;
//      for(int i=0; i<17; i=i+8) {
//      chipId |= ((ESP.getEfuseMac() >> (40 - i)) & 0xff) << i;
//    }
//    return chipId;
//  }

#endif