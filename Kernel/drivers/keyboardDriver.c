#include <keyboardDriver.h>
#include <stdint.h>
#include <naiveConsole.h>
#include <prints.h>
#include <colors.h>

#define PRESS 1
#define RELEASE 2
#define ERROR -1

#define BUFF_LEN 30
static int buffSize=0; //cantidad de elementos del buffer
static int ridx=0; //posicion de escritura
static int widx=0; //posicion de lectura

#define LEFT_SHIFT 0x2A
#define RIGHT_SHIFT 0x36
#define CAPS_LOCK 0x3A
#define CTRL 0x1D

static char buffer[BUFF_LEN];

static int shift = 0;
static int ctrl = 0;
static int capsLock = 0;

//https://www.qbasic.net/en/reference/general/scan-codes.htm
static char charTable[58][2] = {{0, 0}, {0, 0}, {'1', '!'}, {'2', '@'}, 
    {'3', '#'}, {'4', '$'}, {'5', '%'}, {'6', '^'}, 
    {'7', '&'}, {'8', '*'}, {'9', '('}, {'0', ')'}, 
    {'-', '_'}, {'=', '+'}, {'\b', '\b'}, {'\t', '\t'}, 
    {'q', 'Q'}, {'w', 'W'}, {'e', 'E'}, {'r', 'R'}, {'t', 'T'}, 
    {'y', 'Y'}, {'u', 'U'}, {'i', 'I'}, {'o', 'O'}, {'p', 'P'}, 
    {'[', '{'}, {']', '}'}, {'\n', '\n'}, {0, 0}, {'a', 'A'}, 
    {'s', 'S'}, {'d', 'D'}, {'f', 'F'}, {'g', 'G'}, {'h', 'H'},
    {'j', 'J'}, {'k', 'K'}, {'l', 'L'}, {';', ':'}, {'\'', '\"'},
    {'`', '~'}, {0, 0}, {'\\', '|'}, {'z', 'Z'}, {'x', 'X'}, {'c', 'C'}, 
    {'v', 'V'}, {'b', 'B'}, {'n', 'N'}, {'m', 'M'}, {',', '<'}, {'.', '>'}, 
    {'/', '?'}, {0, 0}, {0, 0}, {0, 0}, {' ', ' '}};

uint8_t getAction(uint8_t scanCode) {
    if(scanCode >= 0x01 && scanCode <= 0x3A) {
        return PRESS;
    }
    else if (scanCode >= 0x81 && scanCode <= 0xBA) {
        return RELEASE;
    }
    return ERROR;
}

void keyboardHandler() {
    uint8_t scanCode;
    uint8_t currentAction;
    if(_hasKey()) {
        scanCode = _getKey();
        currentAction = getAction(scanCode);
        if(currentAction == PRESS) {
            if(scanCode == LEFT_SHIFT || scanCode == RIGHT_SHIFT) {
                shift = 1;
            } else if (scanCode == CAPS_LOCK) {
                capsLock = capsLock == 1 ? 0 : 1;
            } else if (scanCode == CTRL) {
                ctrl = 1;
            } else if(scanCode >= 58 || scanCode & 0x80) {
                return ; //NO ASCII
            }
            else {
                if (charTable[scanCode][0] != 0) {
                    if((shift && !capsLock) || (shift && capsLock && !(charTable[scanCode][0] >= 'a' && charTable[scanCode][0] <= 'z')) || (!shift && capsLock && charTable[scanCode][0] >= 'a' && charTable[scanCode][0] <= 'z') )
                        putChar(charTable[scanCode][1]);
                    else
                        putChar(charTable[scanCode][0]);        
                }
            }
        } else if(currentAction == RELEASE) {
            if(scanCode == LEFT_SHIFT + 0x80 || scanCode == RIGHT_SHIFT + 0x80) {
                shift = 0;
            } else if (scanCode == CTRL + 0x80) {
                ctrl = 0;
            }
        }
    }
}

void putCharInBuffer(char c){
    if(c!=0){
        buffer[widx]=c;
        
        //HAY QUE TENER CUIDADO DE QUE LOS INDICES NO PASEN EL SIZE DEL BUFFER
        widx++;
        if(buffSize<BUFF_LEN){
            buffSize++;
        }
        
    }
}

//getChar no hay nada adentro llamar a hlt para bloquear funcion hastas proxima vez que toques tecla
//hlt te deja bloqueado hasta interrupcion, es mejor quedarse bloqeuado que while 1
//hlt, adentro while 