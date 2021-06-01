#ifndef SYSCALLS_H
#define SYSCALLS_H

#include <stdint.h>
#include <colors.h>

uint8_t getCurrentTime(uint64_t rtcID);
uint8_t _getRTCInfo(uint64_t rtcID);
void sys_write(char * str, uint8_t len, t_color bgColor, t_color ftColor);
uint64_t sys_read(uint64_t buffer, uint64_t len);
void getCPUFeatures(uint32_t buffer[4]);
void _checkCPUFeatures(uint32_t buffer[4]); 
uint8_t _checkCPUID(void);
void getMem(uint64_t direc, uint8_t * buffer, uint64_t bytes);
uint8_t _getMem(uint64_t direc);
#endif