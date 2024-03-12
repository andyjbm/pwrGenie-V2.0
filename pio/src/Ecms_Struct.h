#ifndef ECMS_STRUCT_H
   #define ECMS_STRUCT_H

   #include "Secrets.h"

   // Defined in Secrets.h, or here if you're not posting to github...
   //#define EMONCMS_ANDYJBM_APIKEY  "Not-Telling-You"
   //#define EMONCMS_LPSSPL_APIKEY   "Another-Blah-de-Blah"
   //#define EMONCMS_CARLO2_APIKEY   "Nopey-Nothing-2C-Here"
   //#define EMONCMS_ANDYS_SERVER   "your-emoncms-server.co.uk"
   
   #define EMONCMS_FACTORY_NODE   "99"

   #define EMONCMS_DEFAULT_URI    "/emoncms/input/post"
   #define EMONCMS_DEFAULT_URL    EMONCMS_ANDYS_SERVER
   #define EMONCMS_DEFAULT_APIKEY EMONCMS_CARLO2_APIKEY // Defined in Secrets.h

   #include "defs.h"

   // Emoncms Factory Reset Parameters
   struct emoncmsParams {
      int Port                = 443;
      char uri[URISIZE]       = EMONCMS_DEFAULT_URI;
      char server[URLSIZE]    = EMONCMS_DEFAULT_URL;
      char node[NODESIZE]     = EMONCMS_FACTORY_NODE;
      char apikey[APIKEYSIZE] = EMONCMS_DEFAULT_APIKEY;
   };
#endif