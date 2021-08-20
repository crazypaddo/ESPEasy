#ifndef DATASTRUCTS_RTC_STRUCTS_H
#define DATASTRUCTS_RTC_STRUCTS_H

#include "../../ESPEasy_common.h"

// this offsets are in blocks, bytes = blocks * 4
#define RTC_BASE_STRUCT 64
#define RTC_BASE_USERVAR 74
#define RTC_BASE_CACHE 124

#ifdef ESP8266
#define RTC_CACHE_DATA_SIZE 240  // 10 elements
#endif
#ifdef ESP32
// TODO TD-er: ESP32 can store much more samples in its RTC
// However we must make sure the data can be flushed on demand or else 
// one may have to wait for a long time to be able to read the data from the filesystem
#define RTC_CACHE_DATA_SIZE 240 // 10 elements
#endif
#define CACHE_FILE_MAX_SIZE 24000

/*********************************************************************************************\
 * RTCStruct
\*********************************************************************************************/
//max 40 bytes: ( 74 - 64 ) * 4
struct RTCStruct
{
  RTCStruct() = default;

  RTCStruct(const RTCStruct& other) = delete;

  RTCStruct& operator=(const RTCStruct&) = default; 

  void init();

  void clearLastWiFi();

  bool lastWiFi_set() const;


  uint8_t ID1 = 0;
  uint8_t ID2 = 0;
  uint8_t lastWiFiChannel = 0;
  uint8_t factoryResetCounter = 0;
  uint8_t deepSleepState = 0;
  uint8_t bootFailedCount = 0;
  uint8_t flashDayCounter = 0;
  uint8_t lastWiFiSettingsIndex = 0;
  unsigned long flashCounter = 0;
  unsigned long bootCounter = 0;
  unsigned long lastMixedSchedulerId = 0;
  uint8_t lastBSSID[6] = { 0 };
  uint8_t unused1 = 0;  // Force alignment to 4 bytes
  uint8_t unused2 = 0;
  unsigned long lastSysTime = 0;
};


#endif // DATASTRUCTS_RTC_STRUCTS_H
