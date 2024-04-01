/*
 * JK-BMS.hpp
 *
 * Functions to read, convert and print JK-BMS data
 *
 *
 *  Copyright (C) 2023-2024  Armin Joachimsmeyer
 *  Email: armin.joachimsmeyer@gmail.com
 *
 *  This file is part of ArduinoUtils https://github.com/ArminJo/JK-BMSToPylontechCAN.
 *
 *  JK-BMSToPylontechCAN is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *  See the GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program. If not, see <http://www.gnu.org/licenses/gpl.html>.
 *
 */

#pragma once
#ifndef _JK_BMS_HPP
#define _JK_BMS_HPP

#include "jk-bms.h"
#include "HexDump.hpp"

JKLastReplyStruct lastJKReply;

#define JKBMS_DEBUG

/*
 * 1.85 ms
 */

#ifdef JKBMS_SSerial
    void requestJK_BMSStatusFrame(SoftwareSerial *aSerial, uint8_t * JK_RequestFrame, uint16_t jkSize, bool aDebugModeActive) {
#else
    void requestJK_BMSStatusFrame(HardwareSerial *aSerial, uint8_t * JK_RequestFrame, uint16_t jkSize, bool aDebugModeActive) {
#endif
    if (aDebugModeActive) {
        CONSOLE(F("Sending JKBMS requestFrame... "));
        //printBufferHex(JK_RequestFrame, jkSize);
    }
    //Serial.flush();

    for (uint8_t i = 0; i < jkSize; ++i) {
        aSerial->write(JK_RequestFrame[i]);
    }
}

void initJKReplyFrameBuffer() {
    sReplyFrameBufferIndex = 0;
}

// Prints formatted reply buffer raw content
void printJKReplyFrameBuffer() {
    uint8_t *tBufferAddress = JKReplyFrameBuffer;
    printBufferHex(tBufferAddress, JK_BMS_FRAME_HEADER_LENGTH);

    tBufferAddress += JK_BMS_FRAME_HEADER_LENGTH;
    printBufferHex(tBufferAddress, JK_BMS_FRAME_CELL_INFO_LENGTH);

    tBufferAddress += JK_BMS_FRAME_CELL_INFO_LENGTH;
    uint8_t tCellInfoLength = JKReplyFrameBuffer[JK_BMS_FRAME_INDEX_OF_CELL_INFO_LENGTH];
    printBufferHex(tBufferAddress, tCellInfoLength); // Cell info
    CONSOLELN();

    tBufferAddress += tCellInfoLength;
    int16_t tRemainingDataLength = ((int16_t) sReplyFrameBufferIndex + 1)
            - (JK_BMS_FRAME_HEADER_LENGTH + JK_BMS_FRAME_CELL_INFO_LENGTH + JK_BMS_FRAME_TRAILER_LENGTH + tCellInfoLength);
    if (tRemainingDataLength <= 0) {
        return;
    }
    printBufferHex(tBufferAddress, tRemainingDataLength);

    tBufferAddress += tRemainingDataLength;
    printBufferHex(tBufferAddress, JK_BMS_FRAME_TRAILER_LENGTH); // Trailer
}

/*
 * Is assumed to be called if Serial.available() is true
 * @return  JK_BMS_RECEIVE_OK,       if still receiving;
 *          JK_BMS_RECEIVE_FINISHED, if complete frame was successfully read
 *          JK_BMS_RECEIVE_ERROR,    if frame has errors.
 * Reply starts 0.18 ms to 0.45 ms after request was received
 */
#ifdef JKBMS_SSerial
jkbms_readJKResultCode readJK_BMSStatusFrameByte(SoftwareSerial *aSerial) {
#else
jkbms_readJKResultCode readJK_BMSStatusFrameByte(HardwareSerial *aSerial) {
#endif
    uint8_t tReceivedByte = aSerial->read();
    JKReplyFrameBuffer[sReplyFrameBufferIndex] = tReceivedByte;

    // Plausi check and get length of frame
    if (sReplyFrameBufferIndex == 0) {
        // start byte 1
        if (tReceivedByte != JK_FRAME_START_BYTE_0) {
            CONSOLELN(F("Error start frame token != 0x4E"));
            return JK_BMS_RECEIVE_ERROR;
        }
    } else if (sReplyFrameBufferIndex == 1) {
        if (tReceivedByte != JK_FRAME_START_BYTE_1) {
            // Error
            return JK_BMS_RECEIVE_ERROR;
        }

    } else if (sReplyFrameBufferIndex == 3) {
        // length of frame
        sReplyFrameLength = (JKReplyFrameBuffer[2] << 8) + tReceivedByte;

    } else if (sReplyFrameLength > MINIMAL_JK_BMS_FRAME_LENGTH && sReplyFrameBufferIndex == sReplyFrameLength - 3) {
        // Check end token 0x68
        if (tReceivedByte != JK_FRAME_END_BYTE) {
            CONSOLE(F("Error end frame token"));   printBytePaddedHex(tReceivedByte);
            CONSOLE(F(" at index"));               printBytePaddedHex(sReplyFrameBufferIndex);
            CONSOLE(F(" is != 0x68. sReplyFrameLength="), sReplyFrameLength, F("|")); printBytePaddedHex(sReplyFrameLength);
            CONSOLELN();
            return JK_BMS_RECEIVE_ERROR;
        }

    } else if (sReplyFrameLength > MINIMAL_JK_BMS_FRAME_LENGTH && sReplyFrameBufferIndex == sReplyFrameLength + 1) {
        /*
         * Frame received completely, perform checksum check
         */
        uint16_t tComputedChecksum = 0;
        for (uint16_t i = 0; i < sReplyFrameLength - 2; i++) {
            tComputedChecksum = tComputedChecksum + JKReplyFrameBuffer[i];
        }
        uint16_t tReceivedChecksum = (JKReplyFrameBuffer[sReplyFrameLength] << 8) + tReceivedByte;
        if (tComputedChecksum != tReceivedChecksum) {
            CONSOLE(F("Checksum error, computed checksum="));  printBytePaddedHex(tComputedChecksum);
            CONSOLE(F(", received checksum="));                printBytePaddedHex(tReceivedChecksum);
            CONSOLELN();
            return JK_BMS_RECEIVE_ERROR;
        } else {
            return JK_BMS_RECEIVE_FINISHED;
        }
    }
    sReplyFrameBufferIndex++;
    return JK_BMS_RECEIVE_OK;
}

/*
 * Highest bit is set means charge
 * @return Charge is positive, discharge is negative
 */
int16_t getCurrent(uint16_t aJKRAWCurrent) {
    uint16_t tCurrent = swap(aJKRAWCurrent);
    if (tCurrent == 0 || (tCurrent & 0x8000) == 0x8000) {
        // Charge - NO two's complement!
        return (tCurrent & 0x7FFF);
    }
    // discharge
    return tCurrent * -1;

}

int16_t getJKTemperature(uint16_t aJKRAWTemperature) {
    uint16_t tTemperature = swap(aJKRAWTemperature);
    if (tTemperature <= 100) {
        return tTemperature;
    }
    return 100 - tTemperature;
}

// Identity function to avoid swapping if accidentally called
uint8_t swap(uint8_t aByte) {
    return (aByte);
}

uint16_t swap(uint16_t aWordToSwapBytes) {
    return ((aWordToSwapBytes << 8) | (aWordToSwapBytes >> 8));
}

int16_t swap(int16_t aWordToSwapBytes) {
    return ((aWordToSwapBytes << 8) | (aWordToSwapBytes >> 8));
}

uint32_t swap(uint32_t aLongToSwapBytes) {
    return ((aLongToSwapBytes << 24) | ((aLongToSwapBytes & 0xFF00) << 8) | ((aLongToSwapBytes >> 8) & 0xFF00)
            | (aLongToSwapBytes >> 24));
}

/*
 * Convert the big endian cell voltage data from JKReplyFrameBuffer to little endian data in JKConvertedCellInfo
 * and compute minimum, maximum, delta, and average
 */
void fillJKConvertedCellInfo() {
//    uint8_t *tJKCellInfoReplyPointer = &TestJKReplyStatusFrame[11];
    uint8_t *tJKCellInfoReplyPointer = &JKReplyFrameBuffer[JK_BMS_FRAME_INDEX_OF_CELL_INFO_LENGTH];

    uint8_t tNumberOfCellInfo = (*tJKCellInfoReplyPointer++) / 3;
    JKConvertedCellInfo.ActualNumberOfCellInfoEntries = tNumberOfCellInfo;
    if (tNumberOfCellInfo > MAXIMUM_NUMBER_OF_CELLS) {
        CONSOLELN(F("Error: Program compiled with \"MAXIMUM_NUMBER_OF_CELLS=" STR(MAXIMUM_NUMBER_OF_CELLS) "\", but"), tNumberOfCellInfo, F("cell info was sent."));
        return;
    }

    uint16_t tVoltage;
    uint32_t tMillivoltSum = 0;
    uint8_t tNumberOfNonNullCellInfo = 0;
    uint16_t tMinimumMillivolt = 0xFFFF;
    uint16_t tMaximumMillivolt = 0;

    for (uint8_t i = 0; i < tNumberOfCellInfo; ++i) {
        tJKCellInfoReplyPointer++;                                  // Skip Cell number
        uint8_t tHighByte = *tJKCellInfoReplyPointer++;             // Copy CellMillivolt
        tVoltage = tHighByte << 8 | *tJKCellInfoReplyPointer++;
        JKConvertedCellInfo.CellInfoStructArray[i].CellMillivolt = tVoltage;
        if (tVoltage > 0) {
            tNumberOfNonNullCellInfo++;
            tMillivoltSum += tVoltage;
            if (tMinimumMillivolt > tVoltage) {
                tMinimumMillivolt = tVoltage;
                JKConvertedCellInfo.IndexOfMinimumCellMillivolt = i;
            }
            if (tMaximumMillivolt < tVoltage) {
                tMaximumMillivolt = tVoltage;
                JKConvertedCellInfo.IndexOfMaximumCellMillivolt = i;
            }
        }
    }
    JKConvertedCellInfo.MinimumCellMillivolt = tMinimumMillivolt;
    JKConvertedCellInfo.MaximumCellMillivolt = tMaximumMillivolt;
    JKConvertedCellInfo.DeltaCellMillivolt = tMaximumMillivolt - tMinimumMillivolt;
    JKConvertedCellInfo.AverageCellMillivolt = tMillivoltSum / tNumberOfNonNullCellInfo;

#if !defined(NO_CELL_STATISTICS) && !defined(USE_NO_LCD)
    /*
     * Mark and count minimum and maximum cell voltages
     */
    for (uint8_t i = 0; i < tNumberOfCellInfo; ++i) {
        if (JKConvertedCellInfo.CellInfoStructArray[i].CellMillivolt == tMinimumMillivolt) {
            JKConvertedCellInfo.CellInfoStructArray[i].VoltageIsMinMaxOrBetween = cellV::IS_MINIMUM;
            if (sJKFAllReplyPointer->BMSStatus.StatusBits.BalancerActive) {
                CellMinimumArray[i]++; // count for statistics
            }
        } else if (JKConvertedCellInfo.CellInfoStructArray[i].CellMillivolt == tMaximumMillivolt) {
            JKConvertedCellInfo.CellInfoStructArray[i].VoltageIsMinMaxOrBetween = cellV::IS_MAXIMUM;
            if (sJKFAllReplyPointer->BMSStatus.StatusBits.BalancerActive) {
                CellMaximumArray[i]++;
            }
        } else {
            JKConvertedCellInfo.CellInfoStructArray[i].VoltageIsMinMaxOrBetween = cellV::IS_BETWEEN_MINIMUM_AND_MAXIMUM;
        }
    }
#endif

#if !defined(NO_CELL_STATISTICS)
    /*
     * Process minimum statistics
     */
    uint32_t tCellStatisticsSum = 0;
    bool tDoDaylyScaling = false;
    for (uint8_t i = 0; i < tNumberOfCellInfo; ++i) {
        /*
         * After 43200 counts (a whole day being the minimum / maximum) we do scaling
         */
        uint16_t tCellStatisticsCount = CellMinimumArray[i];
        tCellStatisticsSum += tCellStatisticsCount;
        if (tCellStatisticsCount > (60UL * 60UL * 24UL * 1000UL / MILLISECONDS_BETWEEN_JK_DATA_FRAME_REQUESTS)) {
            /*
             * After 43200 counts (a whole day being the minimum / maximum) we do scaling
             */
            tDoDaylyScaling = true;
        }
    }

// Here, we demand 2 minutes of balancing as minimum
    if (tCellStatisticsSum > 60) {
        for (uint8_t i = 0; i < tNumberOfCellInfo; ++i) {
            CellMinimumPercentageArray[i] = ((uint32_t) (CellMinimumArray[i] * 100UL)) / tCellStatisticsSum;
        }
    }

    if (tDoDaylyScaling) {
        /*
         * Do scaling by dividing all values by 2 resulting in an Exponential Moving Average filter for values
         */
        CONSOLELN(F("Do scaling of minimum counts"));
        for (uint8_t i = 0; i < tNumberOfCellInfo; ++i) {
            CellMinimumArray[i] = CellMinimumArray[i] / 2;
        }
    }

     /*
     * Process maximum statistics
     */
    tCellStatisticsSum = 0;
    tDoDaylyScaling = false;
    for (uint8_t i = 0; i < tNumberOfCellInfo; ++i) {
        /*
         * After 43200 counts (a whole day being the minimum / maximum) we do scaling
         */
        uint16_t tCellStatisticsCount = CellMaximumArray[i];
        tCellStatisticsSum += tCellStatisticsCount;
        if (tCellStatisticsCount > (60UL * 60UL * 24UL * 1000UL / MILLISECONDS_BETWEEN_JK_DATA_FRAME_REQUESTS)) {
            /*
             * After 43200 counts (a whole day being the minimum / maximum) we do scaling
             */
            tDoDaylyScaling = true;
        }
    }

    // Here, we demand 2 minutes of balancing as minimum
    if (tCellStatisticsSum > 60) {
        for (uint8_t i = 0; i < tNumberOfCellInfo; ++i) {
            CellMaximumPercentageArray[i] = ((uint32_t) (CellMaximumArray[i] * 100UL)) / tCellStatisticsSum;
        }
    }
    if (tDoDaylyScaling) {
        /*
         * Do scaling by dividing all values by 2 resulting in an Exponential Moving Average filter for values
         */
        CONSOLELN(F("Do scaling of maximum counts"));
        for (uint8_t i = 0; i < tNumberOfCellInfo; ++i) {
            CellMaximumArray[i] = CellMaximumArray[i] / 2;
        }
    }
#endif // NO_CELL_STATISTICS

#if defined(JKBMS_DEBUG)
    CONSOLELN(tNumberOfCellInfo, F("cell voltages processed."));
#endif

// // During JK-BMS startup all cell voltages are sent as zero for around 6 seconds
//     if (tNumberOfNonNullCellInfo < tNumberOfCellInfo && !JKComputedData.BMSIsStarting) {
//         CONSOLELN(F("Problem:"), tNumberOfCellInfo, F(" cells configured in BMS, but only"), tNumberOfNonNullCellInfo, F(" cells seems to be connected"))
//     }

}

#if !defined(NO_CELL_STATISTICS)
/*
 * Print formatted cell info on Serial
 */
void printJKCellStatisticsInfo() {
    uint8_t tNumberOfCellInfo = JKConvertedCellInfo.ActualNumberOfCellInfoEntries;

    /*
     * Cell statistics
     */
    char tStringBuffer[18]; // "12=12 % |  4042, "

    CONSOLELN(F("Cell Minimum percentages"));
    for (uint8_t i = 0; i < tNumberOfCellInfo; ++i) {
        if (i != 0 && (i % 8) == 0) {
            CONSOLELN();
        }
        sprintf_P(tStringBuffer, PSTR("%2u=%2u %% |%5u, "), i + 1, CellMinimumPercentageArray[i], CellMinimumArray[i]);
       CONSOLE(tStringBuffer);
    }
    CONSOLELN();

    CONSOLELN(F("Cell Maximum percentages"));
    for (uint8_t i = 0; i < tNumberOfCellInfo; ++i) {
        if (i != 0 && (i % 8) == 0) {
            CONSOLELN();
        }
        sprintf_P(tStringBuffer, PSTR("%2u=%2u %% |%5u, "), i + 1, CellMaximumPercentageArray[i], CellMaximumArray[i]);
        CONSOLE(tStringBuffer);
    }
    CONSOLELN();
    CONSOLELN();
}

#endif // NO_CELL_STATISTICS

void fillJKComputedData() {
    JKComputedData.TemperaturePowerMosFet = getJKTemperature(sJKFAllReplyPointer->TemperaturePowerMosFet);
    int16_t tMaxTemperature = JKComputedData.TemperaturePowerMosFet;

    JKComputedData.TemperatureSensor1 = getJKTemperature(sJKFAllReplyPointer->TemperatureSensor1);
    if (tMaxTemperature < JKComputedData.TemperatureSensor1) {
        tMaxTemperature = JKComputedData.TemperatureSensor1;
    }

    JKComputedData.TemperatureSensor2 = getJKTemperature(sJKFAllReplyPointer->TemperatureSensor2);
    if (tMaxTemperature < JKComputedData.TemperatureSensor2) {
        tMaxTemperature = JKComputedData.TemperatureSensor2;
    }
    JKComputedData.TemperatureMaximum = tMaxTemperature;

    JKComputedData.TotalCapacityAmpereHour = swap(sJKFAllReplyPointer->TotalCapacityAmpereHour);
// 16 bit multiplication gives overflow at 640 Ah
    JKComputedData.RemainingCapacityAmpereHour = ((uint32_t) JKComputedData.TotalCapacityAmpereHour
            * sJKFAllReplyPointer->SOCPercent) / 100;

// Two values which are zero during JK-BMS startup for around 16 seconds
    JKComputedData.BMSIsStarting = (sJKFAllReplyPointer->SOCPercent == 0 && sJKFAllReplyPointer->Cycles == 0);

    JKComputedData.BatteryFullVoltage10Millivolt = swap(sJKFAllReplyPointer->BatteryOvervoltageProtection10Millivolt);
    JKComputedData.BatteryVoltage10Millivolt = swap(sJKFAllReplyPointer->Battery10Millivolt);
//    JKComputedData.BatteryVoltageDifferenceToFull10Millivolt = JKComputedData.BatteryFullVoltage10Millivolt
//            - JKComputedData.BatteryVoltage10Millivolt;

    JKComputedData.BatteryEmptyVoltage10Millivolt = swap(sJKFAllReplyPointer->BatteryUndervoltageProtection10Millivolt);
    JKComputedData.BatteryVoltageDifferenceToEmpty10Millivolt = JKComputedData.BatteryVoltage10Millivolt
            - JKComputedData.BatteryEmptyVoltage10Millivolt;

    JKComputedData.BatteryVoltageFloat = JKComputedData.BatteryVoltage10Millivolt;
    JKComputedData.BatteryVoltageFloat /= 100;

    JKComputedData.Battery10MilliAmpere = getCurrent(sJKFAllReplyPointer->Battery10MilliAmpere);
    JKComputedData.BatteryLoadCurrentFloat = JKComputedData.Battery10MilliAmpere;
    JKComputedData.BatteryLoadCurrentFloat /= 100;

    JKComputedData.BatteryLoadPower = JKComputedData.BatteryVoltageFloat * JKComputedData.BatteryLoadCurrentFloat;

#if !defined(NO_CELL_STATISTICS)
    /*
     * Increment sBalancingCount and fill sBalancingTimeString
     */
    if (sJKFAllReplyPointer->BMSStatus.StatusBits.BalancerActive) {
        sBalancingCount++;
        sprintf_P(sBalancingTimeString, PSTR("%3uD%02uH%02uM"), (uint16_t) (sBalancingCount / (60 * 24 * 30UL)),
                (uint16_t) ((sBalancingCount / (60 * 30)) % 24), (uint16_t) (sBalancingCount / 30) % 60);
    }
#endif // NO_CELL_STATISTICS
}

void printJKCellInfoOverview() {
    CONSOLE(F(" Minimum at"), JKConvertedCellInfo.IndexOfMinimumCellMillivolt);
    CONSOLE(F(":"), JKConvertedCellInfo.MinimumCellMillivolt);
    CONSOLE(F("mV, Maximum at"), JKConvertedCellInfo.IndexOfMaximumCellMillivolt);
    CONSOLE(F(":"), JKConvertedCellInfo.MaximumCellMillivolt);
    CONSOLE(F("mV, Delta:"), JKConvertedCellInfo.DeltaCellMillivolt);
    CONSOLE(F("mV, Average:"), JKConvertedCellInfo.AverageCellMillivolt);
    CONSOLELN(F("mV"));
}
/*
 * Print formatted cell info on Serial
 */
void printJKCellInfo() {
    uint8_t tNumberOfCellInfo = JKConvertedCellInfo.ActualNumberOfCellInfoEntries;
    CONSOLE(tNumberOfCellInfo, F(" Cells,"));

    // Print summary
    printJKCellInfoOverview();

    // Individual cell voltages
    for (uint8_t i = 0; i < tNumberOfCellInfo; ++i) {
        if (i != 0 && (i % 8) == 0) {CONSOLELN();}  // Newline every 8 cells
        if (i < 9) {CONSOLE(' ');}                  // Pad the index with leading space.

        CONSOLE(i+1);
        CONSOLE(':', JKConvertedCellInfo.CellInfoStructArray[i].CellMillivolt);
        CONSOLE(F("mV, "));

#if defined(LOCAL_TRACE)
        CONSOLE(F(" |));
        printWordPaddedHex(JKConvertedCellInfo.CellInfoStructArray[i].CellMillivolt);
#endif
        
    }
    CONSOLELN();

    CONSOLELN();
}

void printVoltageProtectionInfo() {
    JKReplyStruct *tJKFAllReplyPointer = sJKFAllReplyPointer;
    /*
     * Voltage protection
     */
    CONSOLE(F("Battery Overvoltage Protection[V]="), String(JKComputedData.BatteryFullVoltage10Millivolt / 100.0, 2))
    CONSOLELN(F(", Undervoltage="), String(JKComputedData.BatteryEmptyVoltage10Millivolt / 100.0, 2))

    CONSOLE(F("Cell Overvoltage Protection[mV]="),  swap(tJKFAllReplyPointer->CellOvervoltageProtectionMillivolt));
    CONSOLE(F(", Recovery="),                       swap(tJKFAllReplyPointer->CellOvervoltageRecoveryMillivolt));
    CONSOLELN(F(", Delay[s]="),                     swap(tJKFAllReplyPointer->CellOvervoltageDelaySeconds));

    CONSOLE(F("Cell Undervoltage Protection[mV]="), swap(tJKFAllReplyPointer->CellUndervoltageProtectionMillivolt));
    CONSOLE(F(", Recovery="),                       swap(tJKFAllReplyPointer->CellUndervoltageRecoveryMillivolt));
    CONSOLELN(F(", Delay[s]="),                     swap(tJKFAllReplyPointer->CellUndervoltageDelaySeconds));

    CONSOLELN(F("Cell Voltage Difference Protection[mV]="), swap(tJKFAllReplyPointer->VoltageDifferenceProtectionMillivolt));

    CONSOLE(F("Discharging Overcurrent Protection[A]="),    swap( tJKFAllReplyPointer->DischargeOvercurrentProtectionAmpere));
    CONSOLELN(F(", Delay[s]="),                             swap(tJKFAllReplyPointer->DischargeOvercurrentDelaySeconds));

    CONSOLE(F("Charging Overcurrent Protection[A]="),       swap(tJKFAllReplyPointer->ChargeOvercurrentProtectionAmpere));
    CONSOLELN(F(", Delay[s]="),                             swap(tJKFAllReplyPointer->ChargeOvercurrentDelaySeconds));
    CONSOLELN();
}

void printTemperatureProtectionInfo() {
    JKReplyStruct *tJKFAllReplyPointer = sJKFAllReplyPointer;
    /*
     * Temperature protection
     */
    CONSOLE(F("Power MosFet Temperature Protection="), swap(tJKFAllReplyPointer->PowerMosFetTemperatureProtection))
    CONSOLELN(F(", Recovery="), swap(tJKFAllReplyPointer->PowerMosFetRecoveryTemperature))

    CONSOLE(F("Sensor1 Temperature Protection="), swap(tJKFAllReplyPointer->Sensor1TemperatureProtection))
    CONSOLELN(F(", Recovery="), swap(tJKFAllReplyPointer->Sensor1RecoveryTemperature))

    CONSOLELN(F("Sensor1 to Sensor2 Temperature Difference Protection="),
            swap(tJKFAllReplyPointer->BatteryDifferenceTemperatureProtection))

    CONSOLE(F("Charge Overtemperature Protection="), swap(tJKFAllReplyPointer->ChargeOvertemperatureProtection))
    CONSOLELN(F(", Discharge="), swap(tJKFAllReplyPointer->DischargeOvertemperatureProtection))

    CONSOLE(F("Charge Undertemperature Protection="), swap(tJKFAllReplyPointer->ChargeUndertemperatureProtection))
    CONSOLELN(F(", Recovery="), swap(tJKFAllReplyPointer->ChargeRecoveryUndertemperature))

    CONSOLE(F("Discharge Undertemperature Protection="), swap(tJKFAllReplyPointer->DischargeUndertemperatureProtection))
    CONSOLELN(F(", Recovery="), swap(tJKFAllReplyPointer->DischargeRecoveryUndertemperature))
    CONSOLELN();
}

void printBatteryInfo() {
    JKReplyStruct *tJKFAllReplyPointer = sJKFAllReplyPointer;
        
    // Set 'end of string' tokens for the preceding char arrays. (See the struct)    
    tJKFAllReplyPointer->TokenSystemWorkingMinutes  = '\0';
    tJKFAllReplyPointer->TokenProtocolVersionNumber = '\0';
    tJKFAllReplyPointer->TokenManufacturerDate      = '\0';

    CONSOLELN(F("Manufacturer Date="), tJKFAllReplyPointer->ManufacturerDate);
    CONSOLELN(F("Manufacturer Id="),   tJKFAllReplyPointer->ManufacturerId);      // First 8 characters of the manufacturer id entered in the app field "User Private Data"
    CONSOLELN(F("Device ID String="),  tJKFAllReplyPointer->DeviceIdString);      // First 8 characters of ManufacturerId
    CONSOLELN(F("Device Address="),    tJKFAllReplyPointer->BoardAddress);
    CONSOLE(F("Total Battery Capacity[Ah]="),     JKComputedData.TotalCapacityAmpereHour);          // 0xAA
    CONSOLELN(F(", Low Capacity Alarm Percent="), tJKFAllReplyPointer->LowCapacityAlarmPercent);    // 0xB1
    CONSOLELN(F("Charging Cycles="),                swap(tJKFAllReplyPointer->Cycles));
    CONSOLELN(F("Total Charging Cycle Capacity="),  swap(tJKFAllReplyPointer->TotalBatteryCycleCapacity));
    CONSOLE(F("# Battery Cells="),                  swap(tJKFAllReplyPointer->NumberOfBatteryCells));   // 0x8A Total number of battery strings
    CONSOLELN(F(", Cell Count="),                   tJKFAllReplyPointer->BatteryCellCount);             // 0xA9 Battery string count settings
    CONSOLELN();
}

void printBMSInfo() {
    JKReplyStruct *tJKFAllReplyPointer = sJKFAllReplyPointer;

    tJKFAllReplyPointer->TokenStartCurrentCalibration =     '\0'; // set end of string token
    tJKFAllReplyPointer->TokenDedicatedChargerSwitchState = '\0'; // set end of string token
    
    CONSOLELN(F("Protocol Version Number="),   tJKFAllReplyPointer->ProtocolVersionNumber)
    CONSOLELN(F("Software Version Number="),   tJKFAllReplyPointer->SoftwareVersionNumber)
    CONSOLELN(F("Modify Parameter Password="), tJKFAllReplyPointer->ModifyParameterPassword)
    CONSOLELN(F("# External Temperature Sensors="), tJKFAllReplyPointer->NumberOfTemperatureSensors); // 0x86

    CONSOLELN();
}

void printMiscellaneousInfo() {
    JKReplyStruct *tJKFAllReplyPointer = sJKFAllReplyPointer;

    CONSOLELN(F("Balance Starting Cell Voltage[mV]="),         swap(tJKFAllReplyPointer->BalancingStartMillivolt))
    CONSOLELN(F("Balance Triggering Voltage Difference[mV]="), swap(tJKFAllReplyPointer->BalancingStartDifferentialMillivolt))
    CONSOLELN();
    CONSOLELN(F("Current Calibration[mA]="),            swap(tJKFAllReplyPointer->CurrentCalibrationMilliampere))
    CONSOLELN(F("Sleep Wait Time[s]="),                 swap(tJKFAllReplyPointer->SleepWaitingTimeSeconds))
    CONSOLELN();
    CONSOLELN(F("Dedicated Charge Switch Active="),     tJKFAllReplyPointer->DedicatedChargerSwitchIsActive)
    CONSOLELN(F("Start Current Calibration State="),    tJKFAllReplyPointer->StartCurrentCalibration)
    CONSOLELN(F("Battery Actual Capacity[Ah]="),        swap(tJKFAllReplyPointer->ActualBatteryCapacityAmpereHour))
    CONSOLELN();
}

/*
 * Token 0x8B. Prints info only if errors existent and changed from last value
 * Stores error string for LCD in sErrorStringForLCD
 */
void handleAndPrintAlarmInfo() {
    JKReplyStruct *tJKFAllReplyPointer = sJKFAllReplyPointer;

    /*
     * Do it only once per change
     */
    if (tJKFAllReplyPointer->AlarmUnion.AlarmsAsWord != lastJKReply.AlarmUnion.AlarmsAsWord) {
        // ChargeOvervoltageAlarm is displayed separately
        if (!tJKFAllReplyPointer->AlarmUnion.AlarmBits.ChargeOvervoltageAlarm) {
            sSwitchPageToShowError = true; // This forces a switch to Overview page
            sErrorStatusIsError = true; //  This forces the beep
        }

        if (tJKFAllReplyPointer->AlarmUnion.AlarmsAsWord == 0) {
            sCurrentErrorString = NULL; // reset error string
            sErrorStatusIsError = false;
            CONSOLELN(F("All alarms are cleared now"));
        } else {
            uint16_t tAlarms = swap(tJKFAllReplyPointer->AlarmUnion.AlarmsAsWord);
            CONSOLELN(F("*** ALARM FLAGS ***"));
            CONSOLE(sUpTimeString, F(": Alarm bits=")); printWordPaddedHex(tAlarms); // print uptime to have a timestamp for the alarm
            CONSOLELN();

            uint16_t tAlarmMask = 1;
            for (uint_fast8_t i = 0; i < NUMBER_OF_DEFINED_ALARM_BITS; ++i) {
                if (tAlarms & tAlarmMask) {
                    CONSOLE(F("Alarm bit=0b"));
                    CONSOLE(tAlarmMask, BIN);
                    CONSOLE(F(" -> "));
                    sCurrentErrorString = (char *) (pgm_read_dword(&JK_BMSErrorStringsArray[i]));
                    sErrorStringForLCD = sCurrentErrorString;
                    CONSOLELN(reinterpret_cast<const __FlashStringHelper*>(sCurrentErrorString));
                }
                tAlarmMask <<= 1;
            }
            CONSOLELN();
        }
    }
}

void printEnabledState(bool aIsEnabled) {
    if (aIsEnabled) {
        CONSOLE(F(" enabled"));
    } else {
        CONSOLE(F(" disabled"));
    }
}

void printActiveState(bool aIsActive) {
    if (!aIsActive) {
        CONSOLE(F(" not"));
    }
    CONSOLE(F(" active"));
}

void printJKStaticInfo() {

    CONSOLELN(F("*** BMS INFO ***"));
    printBMSInfo();

    CONSOLELN(F("*** BATTERY INFO ***"));
    printBatteryInfo();

    CONSOLELN(F("*** VOLTAGE PROTECTION INFO ***"));
    printVoltageProtectionInfo();

    CONSOLELN(F("*** TEMPERATURE PROTECTION INFO ***"));
    printTemperatureProtectionInfo();

    CONSOLELN(F("*** MISC INFO ***"));
    printMiscellaneousInfo();
}

void computeUpTimeString() {
    if (sJKFAllReplyPointer->SystemWorkingMinutes != lastJKReply.SystemWorkingMinutes) {
        sUpTimeStringMinuteHasChanged = true;

        uint32_t tSystemWorkingMinutes = swap(sJKFAllReplyPointer->SystemWorkingMinutes);
// 1 kByte for sprintf  creates string "1234D23H12M"
        sprintf_P(sUpTimeString, PSTR("%4uD%02uH%02uM"), (uint16_t) (tSystemWorkingMinutes / (60 * 24)),
                (uint16_t) ((tSystemWorkingMinutes / 60) % 24), (uint16_t) (tSystemWorkingMinutes % 60));
        if (sLastUpTimeTenthOfMinuteCharacter != sUpTimeString[8]) {
            sLastUpTimeTenthOfMinuteCharacter = sUpTimeString[8];
            sUpTimeStringTenthOfMinuteHasChanged = true;
        }
    }
}

const char sCSVCaption[] PROGMEM
        = "Cell_1;Cell_2;Cell_3;Cell_4;Cell_5;Cell_6;Cell_7;Cell_8;Cell_9;Cell_10;Cell_11;Cell_12;Cell_13;Cell_14;Cell_15;Cell_16;Cell_17;Cell_18;Cell_19;Cell_20;Cell_21;Cell_22;Cell_23;Cell_24;Voltage;Current;SOC;Balancing";

/*
 * Print received data
 * Use converted cell voltage info from JKConvertedCellInfo
 * All other data are used unconverted and are therefore printed by swap() functions.
 */
void printJKDynamicInfo() {
    JKReplyStruct *tJKFAllReplyPointer = sJKFAllReplyPointer;

#if !defined(DISABLE_MONITORING)
#  if defined(MONOTORING_PERIOD_FAST)
    // Print every dataset, i.e. every 2 seconds, and caption every minute
    printMonitoringInfo();
    if (sUpTimeStringMinuteHasChanged) {
        sUpTimeStringMinuteHasChanged = false;
        CONSOLELN(reinterpret_cast<const __FlashStringHelper*>(sCSVCaption));
    }
#  else
    // print every minute and caption every 10 minutes
    if (sUpTimeStringMinuteHasChanged) {
        sUpTimeStringMinuteHasChanged = false;
        #if !defined(DISABLE_MONITORING)
            CONSOLELN(F("CSV:"), setCSVString());
        #endif
    }
#  endif
#endif

    /*
     * Print it every ten minutes
     */
    if (sUpTimeStringTenthOfMinuteHasChanged) {
        sUpTimeStringTenthOfMinuteHasChanged = false;
#if !defined(DISABLE_MONITORING) && !defined(MONOTORING_PERIOD_FAST)
        CONSOLELN(reinterpret_cast<const __FlashStringHelper*>(sCSVCaption));
#endif
        CONSOLELN(F("Total Runtime:"), swap(sJKFAllReplyPointer->SystemWorkingMinutes), F("minutes ->"), sUpTimeString);
        CONSOLELN(F("*** CELL INFO ***"));
        printJKCellInfo();

#if !defined(NO_CELL_STATISTICS)
        if (sBalancingCount > MINIMUM_BALANCING_COUNT_FOR_DISPLAY) {
            CONSOLELN(F("*** CELL STATISTICS ***"));
            
            // Prepare seconds for TimeString
            char tString[4]; // "03S" is 3 bytes long plus \0
            sprintf_P(tString, PSTR("%02uS"), (uint16_t) (sBalancingCount % 30) * 2);

            CONSOLE(F("Total balancing time="), (sBalancingCount * (MILLISECONDS_BETWEEN_JK_DATA_FRAME_REQUESTS / 1000)), \
                      F("s ->"), sBalancingTimeString)
            CONSOLELN(tString) // aviod space between sBalancingTimeString and tString
            
            printJKCellStatisticsInfo();
        }
#endif

#if !defined(SUPPRESS_LIFEPO4_PLAUSI_WARNING)
        if (swap(tJKFAllReplyPointer->CellOvervoltageProtectionMillivolt) > 3450) {
            // https://www.evworks.com.au/page/technical-information/lifepo4-care-guide-looking-after-your-lithium-batt/
            CONSOLE(F("Warning: CellOvervoltageProtectionMillivolt value"), swap(tJKFAllReplyPointer->CellOvervoltageProtectionMillivolt));
            CONSOLELN(F(" mV > 3450 mV is not recommended for LiFePO4 chemistry."));
            CONSOLELN(F("There is less than 1% extra capacity above 3.5V."));
        }
        if (swap(tJKFAllReplyPointer->CellUndervoltageProtectionMillivolt) < 3000) {
            // https://batteryfinds.com/lifepo4-voltage-chart-3-2v-12v-24v-48v/
            CONSOLE(F("Warning: CellUndervoltageProtectionMillivolt value"), swap(tJKFAllReplyPointer->CellUndervoltageProtectionMillivolt));
            CONSOLELN(F(" mV < 3000 mV is not recommended for LiFePO4 chemistry."));
            CONSOLELN(F("There is less than 10% capacity below 3.0V and 20% capacity below 3.2V."));
        }
#endif
    } // Print it every ten minutes

    /*
     * Temperatures
     * Print only if temperature changed more than 1 degree
     */
#if defined(JKBMS_DEBUG)
    CONSOLE(F("TokenTemperaturePowerMosFet="));
    printWordPaddedHex(sJKFAllReplyPointer->TokenTemperaturePowerMosFet);
    CONSOLELN();
#endif

    if (abs(JKComputedData.TemperaturePowerMosFet - JKLastPrintedData.TemperaturePowerMosFet) > 2
            || abs(JKComputedData.TemperatureSensor1 - JKLastPrintedData.TemperatureSensor1) > 2
            || abs(JKComputedData.TemperatureSensor2 - JKLastPrintedData.TemperatureSensor2) > 2)
    {
        CONSOLE(F("Temperature: Power MosFet="), JKComputedData.TemperaturePowerMosFet);
        CONSOLE(F(", Sensor 1="),                JKComputedData.TemperatureSensor1);
        CONSOLELN(F(", Sensor 2="),              JKComputedData.TemperatureSensor2);

        JKLastPrintedData.TemperaturePowerMosFet = JKComputedData.TemperaturePowerMosFet;
        JKLastPrintedData.TemperatureSensor1 = JKComputedData.TemperatureSensor1;
        JKLastPrintedData.TemperatureSensor2 = JKComputedData.TemperatureSensor2;
    }

    /*
     * SOC
     */
    if (tJKFAllReplyPointer->SOCPercent != lastJKReply.SOCPercent
            || JKComputedData.RemainingCapacityAmpereHour != JKLastPrintedData.RemainingCapacityAmpereHour) {
        CONSOLE(F("SOC:"), tJKFAllReplyPointer->SOCPercent);
        CONSOLELN(F("% -> Remaining Capacity[Ah]:"), JKComputedData.RemainingCapacityAmpereHour);
        JKLastPrintedData.RemainingCapacityAmpereHour = JKComputedData.RemainingCapacityAmpereHour;
    }

    /*
     * Charge and Discharge values
     */
    if (abs(JKComputedData.BatteryVoltageFloat - JKLastPrintedData.BatteryVoltageFloat) > 0.03
            || abs(JKComputedData.BatteryLoadPower - JKLastPrintedData.BatteryLoadPower) >= 50) {
        CONSOLE(F("Battery Voltage[V]:"), String(JKComputedData.BatteryVoltageFloat, 2))
        CONSOLE(F("  Current[A]:"), String(JKComputedData.BatteryLoadCurrentFloat, 2))
        CONSOLE(F("  Power[W]:"), JKComputedData.BatteryLoadPower);
        CONSOLELN(F("  Difference to empty[V]:"), String(JKComputedData.BatteryVoltageDifferenceToEmpty10Millivolt / 100.0, 1))
        JKLastPrintedData.BatteryVoltageFloat = JKComputedData.BatteryVoltageFloat;
        JKLastPrintedData.BatteryLoadPower = JKComputedData.BatteryLoadPower;
    }

    /*
     * Charge, Discharge and Balancer flags
     */
    if (tJKFAllReplyPointer->BMSStatus.StatusBits.ChargeMosFetActive != lastJKReply.BMSStatus.StatusBits.ChargeMosFetActive
            || tJKFAllReplyPointer->BMSStatus.StatusBits.DischargeMosFetActive
                    != lastJKReply.BMSStatus.StatusBits.DischargeMosFetActive) {
        /*
         * This happens quite seldom!
         */
        CONSOLE(F("Charging MosFet"));
        printEnabledState(tJKFAllReplyPointer->ChargeIsEnabled);
        CONSOLE(',');
        printActiveState(tJKFAllReplyPointer->BMSStatus.StatusBits.ChargeMosFetActive);
        CONSOLE(F(" | Discharging MosFet"));
        printEnabledState(tJKFAllReplyPointer->DischargeIsEnabled);
        CONSOLE(',');
        printActiveState(tJKFAllReplyPointer->BMSStatus.StatusBits.DischargeMosFetActive);
        CONSOLE(F(" | Balancing")); // including balancer state to be complete :-)
        printEnabledState(tJKFAllReplyPointer->BalancingIsEnabled);
        CONSOLE(',');
        printActiveState(tJKFAllReplyPointer->BMSStatus.StatusBits.BalancerActive);
        CONSOLELN(); // printActiveState does no println()
    } else if (tJKFAllReplyPointer->BMSStatus.StatusBits.BalancerActive != lastJKReply.BMSStatus.StatusBits.BalancerActive) {
        /*
         * Only Balancer, since it happens very often
         */
        CONSOLE(F("Balancing"));
        printActiveState(tJKFAllReplyPointer->BMSStatus.StatusBits.BalancerActive);
        if (tJKFAllReplyPointer->BMSStatus.StatusBits.BalancerActive) {
            printJKCellInfoOverview();
        } else {
            CONSOLELN(); // printActiveState does no println()
        }
    }
}

#if !defined(DISABLE_MONITORING)
/*
 * Fills sStringBuffer with CSV data
 * Can be used for SD Card also, otherwise direct printing would be more efficient.
 * Print all (cell voltages - 3000 mV), voltage, current, SOC in CSV format
 * E.g. 185;185;186;185;185;206;185;214;183;185;201;186;186;186;185;186;5096;-565;54;1
 *
 */
String setCSVString() {
    JKReplyStruct *tJKFAllReplyPointer = sJKFAllReplyPointer;
    char sStringBuffer[CSV_STRINGBUFFERSIZE];    // buffer for CSV Text
    /*
     * Individual cell voltages
     */
    uint_fast8_t tBufferIndex = 0;

//    for (uint8_t i = 0; i < JKConvertedCellInfo.ActualNumberOfCellInfoEntries; ++i) {
    if (sizeof(sStringBuffer) > (5 * MAXIMUM_NUMBER_OF_CELLS)) {
        for (uint8_t i = 0; i < MAXIMUM_NUMBER_OF_CELLS; ++i) { // only 16 fits into sStringBuffer[90] 
            // check for valid data, otherwise we will get a string buffer overflow
            if (JKConvertedCellInfo.CellInfoStructArray[i].CellMillivolt > 2500) {
                tBufferIndex += sprintf_P(&sStringBuffer[tBufferIndex], PSTR("%d;"),
                        (int16_t) (JKConvertedCellInfo.CellInfoStructArray[i].CellMillivolt));
            }
        }
    }

    if ((uint_fast8_t) (tBufferIndex + 17) >= sizeof(sStringBuffer)) {
        CONSOLELN(F("String buffer overflow, tBufferIndex="), tBufferIndex, F("sizeof(sStringBuffer)="), sizeof(sStringBuffer));
        sStringBuffer[0] = '\0';
    } else {
        // maximal string 5096;-20.00;100;1 -> 17 characters
        char tCurrentAsFloatString[7];
        dtostrf(JKComputedData.BatteryLoadCurrentFloat, 4, 2, &tCurrentAsFloatString[0]);
        tBufferIndex += sprintf_P(&sStringBuffer[tBufferIndex], PSTR("%u;%s;%d;%d"), JKComputedData.BatteryVoltage10Millivolt,
                tCurrentAsFloatString, tJKFAllReplyPointer->SOCPercent, tJKFAllReplyPointer->BMSStatus.StatusBits.BalancerActive);
    }
    return sStringBuffer;
}
#endif // !defined(DISABLE_MONITORING)

#endif // _JK_BMS_HPP