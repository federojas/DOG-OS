#ifndef RTC_H
#define RTC_H

#include <stdint.h>

uint8_t getCurrentTime(uint64_t rtcID);
uint8_t _getRTCInfo(uint64_t rtcID);

#endif