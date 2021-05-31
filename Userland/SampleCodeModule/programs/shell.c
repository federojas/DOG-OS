#include <shell.h>
#include <libc.h>
#include <commands.h>

void startShell(){
    shellWelcomeMessage();
    divZero(0,0);
    while(1){
    }
}

void shellWelcomeMessage(){
    printf("hola como estas!\n");
}

