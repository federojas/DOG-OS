#include <shell.h>
#include <libc.h>
#include <commands.h>

void startShell(){
    printf("A VER QUE PASA CON ESTE MENSAJE\n");
    
    // for(int i=0;i<40;i++){
    // shellWelcomeMessage();
    // } 
    // for(int i=0;i<45;i++){
    //     printf("linea: %d\n",i);
    // }
    
    //CON HLT USAR SIN WHILE 1 
    //getMem(1, 0, "1");
    while(1){
    readText();
    }
    

}

void shellWelcomeMessage(){
    printf("hola como estas!\n");
}

