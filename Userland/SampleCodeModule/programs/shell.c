#include <shell.h>
#include <libc.h>
#include <commands.h>

void startShell(){
    shellWelcomeMessage();
    while(1){
        readText();
    }
}

void shellWelcomeMessage(){
    printf("hola como estas!\n");
}

