#include <shell.h>
#include <libc.h>
#include <commands.h>

#define BUFFER_SIZE 100
#define USER_SIZE 32

void startShell(){
    
    shellExecute();
}

void shellWelcomeMessage(){
    printf("hola como estas!\n");
}

void shellExecute(){
    char command[BUFFER_SIZE] = {0};
    char parameters[BUFFER_SIZE] = {0};
    char userName[USER_SIZE] = "DefaultUser";
    int aux=0;
    while (1){
        printf("$%s > ", userName);
        command[0]=0;
        parameters[0]=0;

        scanf("%s",command);
        if(strcmp("/help", command) == 0) {
            help(0,0);
        }
        if(strcmp("/inforeg", command) == 0) {
            getInfoReg(0,0);
        }
        if(strcmp("/cpufeatures", command) == 0) {
            getCPUFeatures(0,0);
        }
        if(strcmp("/clear", command) == 0) {
            clear(0,0);
        }
        if(strcmp("/exit", command) == 0) {
            exit(0,0);
        }
    }
    return;
}
