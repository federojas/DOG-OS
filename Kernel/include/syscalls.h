#ifndef SYSCALLS_H
#define SYSCALLS_H

#include <stdint.h>
#include <colors.h>

#define SYS_RTC_ID 1
#define SYS_CPUFEATURES_ID 2
#define SYS_READ_ID 3
#define SYS_WRITE_ID 4
#define SYS_INFOREG_ID 5
#define SYS_PRINTMEM_ID 6
#define SYS_CPUID_ID 7
#define SYS_CLEAR_ID 8
#define SYS_EXIT_ID 9
#define SYS_CPUVENDOR_ID 10

uint8_t getCurrentTime(uint64_t rtcID);
uint8_t _getRTCInfo(uint64_t rtcID);
void sys_write(char * str, uint8_t len, t_color bgColor, t_color ftColor, int userSize);
//uint64_t sys_read(uint64_t buffer, uint64_t len); MODO ESPERA ACTIVO
uint64_t sys_read(); //MODO HLT
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
#endif