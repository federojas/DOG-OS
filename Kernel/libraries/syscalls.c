// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <infoReg.h>
#include <keyboardDriver.h>
#include <pipes.h>
#include <processManager.h>
#include <syscalls.h>
#include <videoDriver.h>

static uint64_t registers[16] = {0};

// https://wiki.osdev.org/CMOS#Format_of_Bytes
uint8_t getCurrentTime(uint64_t rtcID) {
  uint8_t x = _getRTCInfo(rtcID);
  uint8_t result = ((x / 16) * 10) + (x & 0xf);
  return result;
}

void getCPUVendor(char *buffer) { cpuVendor(buffer); }

// https://gist.github.com/macton/4dd5fec2113be284796e

void getCPUFeatures(uint32_t buffer[4]) {
  // El arreglo buffer tendra los arreglos ecx y edx cuando corresponda con eax
  // =1 Consecuentes, estaran ebx y ecx cuando corresponda con eax = 7
  // con eax = 1: buffer[0] = edx buffer[1] = ecx
  // con eax = 7: buffer[2] = ebx buffer[3] = ecx

  _checkCPUFeatures(buffer);
}

void getMem(uint64_t direc, uint8_t *buffer, uint64_t bytes) {
  for (uint8_t i = 0; i < bytes; i++) {
    buffer[i] = (uint8_t)_getMem(direc + i);
  }
}

void sysWrite(char *str, uint8_t len, t_color bgColor, t_color ftColor,
              int usrLen) {
  if (str == 0 || len <= 0) {
    return;
  }

  int outputFD = getCurrentProcessOutputFD();

  if (usrLen > 1) {
    setUsernameLen(len);
  }

  if (outputFD == 1) {
    int i = 0;
    for (; i < len && str[i] != 0; i++) {
      printChar(str[i], ftColor, bgColor, 1);
    }
  } else {
    pipeWrite(outputFD, str);
  }
}

uint64_t sysRead() {
  int inputFD = getCurrentProcessInputFD();
  if (inputFD == 0) {
    if (currentProcessIsForeground() == 1) {
      return getChar();
    } else {
      return -1;
    }
  }
  return pipeRead(inputFD);
}

uint64_t *getRegisters() { return registers; }

void updateRegisters(uint64_t *rsp) {
  for (int i = 0; i < 16; i++) {
    registers[i] = rsp[i];
  }
}
