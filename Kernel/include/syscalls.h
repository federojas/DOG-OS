#ifndef SYSCALLS_H
#define SYSCALLS_H

#include <stdint.h>
#include <colors.h>

uint8_t getCurrentTime(uint64_t rtcID);
uint8_t _getRTCInfo(uint64_t rtcID);
void sysWrite(char * str, uint8_t len, t_color bgColor, t_color ftColor, int usrLen);
uint64_t sysRead();
void getCPUFeatures(uint32_t buffer[4]);
void _checkCPUFeatures(uint32_t buffer[4]); 
uint8_t _checkCPUID(void);
void getMem(uint64_t direc, uint8_t * buffer, uint64_t bytes);
uint8_t _getMem(uint64_t direc);
void getRoots(float * results, float a, float b, float c);
uint8_t _checkCPUID(void);
void _exit(void);
uint64_t* getRegisters();
void updateRegisters(uint64_t* rsp);
void getCPUVendor(char * buffer);
void cpuVendor(char * buffer);
void *malloc(uint64_t nbytes);
void free(void *block);
void memoryDump();

#endif