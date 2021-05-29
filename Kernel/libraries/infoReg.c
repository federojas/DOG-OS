#include <infoReg.h>
#include <prints.h>

#define REGISTER_AMOUNT 16

static uint64_t registers[REGISTER_AMOUNT + 1 ] = {0};

static char* registerNames[] = {
"R15: ", "R14: ", "R13: ", "R12: ", "R11: ", "R10: ", "R9: ",
"R8: ", "RSI: ", "RDI: ", "RBP: ", "RDX: ", "RCX: ", "RBX: ",
"RAX: ", "RIP: ", "RSP: "
};

void getInfoReg() {

for (int i = 0; i < REGISTER_AMOUNT; i++) {
            printString(registerNames[i]);
            printIntHex(registers[i]);
            printLine();
      }
}