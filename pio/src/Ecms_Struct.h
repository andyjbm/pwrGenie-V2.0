#pragma once
#ifndef ECMS_STRUCT_H
   #define ECMS_STRUCT_H

   #include "wmparams.h"

   extern ESPAsync_WiFiManager wm;

   // Get values from the wm Parameters Collection.
   namespace ecmsParams {
      inline int          Port()   {return wm.getParameter(pgParam::e_Port)->getParam_asInt();}
      inline const String uri()    {return wm.getParameter(pgParam::e_uri)->getParam_asString();};
      inline const String server() {return wm.getParameter(pgParam::e_server)->getParam_asString();}
      inline const String node()   {return wm.getParameter(pgParam::e_node)->getParam_asString();}
      inline const String apikey() {return wm.getParameter(pgParam::e_apikey)->getParam_asString();}

      inline u_int8_t     ModbusID()   {return wm.getParameter(pgParam::modbusID)->getParam_asInt();}
   };
#endif