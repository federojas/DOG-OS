#include <keyboard.h>
#include <naiveConsole.h>
#include <stdint.h>

void printPressedKey() {
    uint8_t x;
    if(_hasKey()) {
        x = _getKey();
        if(x >= 0x01 && x<=0x3A)
            ncPrintChar(x);
    }
}