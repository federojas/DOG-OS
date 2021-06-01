#include <shell.h>
#include <libc.h>
#include <commands.h>

void startShell(){
    shellWelcomeMessage(); 
    //CON HLT USAR SIN WHILE 1 
    while(1){
    readText();
    }

}

void shellWelcomeMessage(){
    printf("hola como estas!\n");
}

