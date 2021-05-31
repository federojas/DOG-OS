#include <shell.h>
#include <libc.h>
#include <commands.h>

void startShell(){
    shellWelcomeMessage();
    while(1){
    readText();
    }
//     int x = 3;
//     int y = 0;
//     x = x/y;
}

void shellWelcomeMessage(){
    printf("hola como estas!\n");
}

