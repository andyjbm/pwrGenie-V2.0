#if !defined(MAXIMUM_NUMBER_OF_CELLS)
    #define MAXIMUM_NUMBER_OF_CELLS     24 // Maximum number of cell info which can be converted. Must be before #include "JK-BMS.hpp".
#endif

#ifndef MILLISECONDS_BETWEEN_JK_DATA_FRAME_REQUESTS
    #define MILLISECONDS_BETWEEN_JK_DATA_FRAME_REQUESTS     2000
#endif

#if !defined(JK_BMS_TX_PIN)                       // Allow override by global symbol
    #define JK_BMS_TX_PIN                           4
#endif