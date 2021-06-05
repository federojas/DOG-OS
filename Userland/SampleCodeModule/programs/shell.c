#include <shell.h>
#include <libc.h>
#include <commands.h>

#define BUFFER_SIZE 100
#define USER_SIZE 32
#define MAX_ARGUMENTS 3

void startShell(){
    
    shellExecute();
}

void shellWelcomeMessage(){
    printf("hola como estas!\n");
}

static int getCommandArgs(char* userInput, char* command, char* argv[MAX_ARGUMENTS]) {
    int argc = 0;
    int i = 0;
    for(i = 0; userInput[i] != 0 && userInput[i] != ' '; i++) {
        command[i] = userInput[i];
    }
    command[i] = 0;
    int argIdx = 0;
    while(userInput[i] != 0 && argc < MAX_ARGUMENTS) {
        i++;
        argIdx = 0;
        for(; userInput[i] != ' ' && userInput[i] != 0; i++, argIdx++) {
           argv[argc][argIdx] = userInput[i];
        }
        argv[argc][argIdx] = 0;
        argc++;
    }
    if(argc == MAX_ARGUMENTS && userInput[i] != 0)
        return -1;
    return argc;
}

void shellExecute(){
    char userInput[BUFFER_SIZE] = {0};
    char command[BUFFER_SIZE] = {0};
    char* argv[MAX_ARGUMENTS];
    int argc = 0;
    char userName[USER_SIZE] = "DefaultUser";
    while (1){
        printf("$%s > ", userName);

        userInput[0] = 0;

        scanf("%s", userInput);
        
        argc = getCommandArgs(userInput, command, argv);

        if(argc == -1) {
            printf("Ingreso argumentos de mas.\nLa maxima cantidad de argumentos permitida es: %d.\n", MAX_ARGUMENTS);
        }
        else if(strcmp("/help", command) == 0) {
            help(argc, argv);
        }
        else if(strcmp("/inforeg", command) == 0) {
            getInfoReg(argc, argv);
        }
        else if(strcmp("/cpufeatures", command) == 0) {
            getCPUFeatures(argc, argv);
        }
        else if(strcmp("/clear", command) == 0) {
            clear(argc, argv);
        }
        else if(strcmp("/exit", command) == 0) {
            exit(argc, argv);
        } 
        else if(strcmp("/opcode", command) == 0) {
            opCode(argc, argv);
        } 
        else if(strcmp("/date&time", command) == 0) {
            getCurrentDayTime(argc, argv);
        }
        else if(strcmp("/printmem", command) == 0) {
            getMem(argc, argv);
        }
        else if(strcmp("/divzero", command) == 0) {
            divZero(argc, argv);
        }
        else {
            printf("Comando invalido: use /help\n");
        }
    }
    return;
}
