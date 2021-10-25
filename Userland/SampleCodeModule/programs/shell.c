#include <shell.h>
#include <libc.h>
#include <commands.h>
#include <stdint.h>
#include <memoryTest.h>
#include <processTest.h>
#include <priorityTest.h>
#include <semaphoreTest.h>

static char userName[USER_SIZE] = "DefaultUser";
static int shellStartup = 1;

void startShell(int argc, char **argv){ 
    if(shellStartup)
        //shellWelcomeMessage();
    shellExecute(0,0);
}

void shellWelcomeMessage(){
    printf("\n                 Le damos la bienvenida a\n\n");
    logo();
    printf("\n         Arquitectura de Computadoras --- 1C 2021\n\n");
    printf("\n  Utilice el comando /help para obtener un manual de usuario.\n\n\n\n");
    shellStartup = 0;
}

static int getCommandArgs(char* userInput, char* command, char argv[MAX_ARGUMENTS][BUFFER_SIZE]) {
    int argc = 0;
    int i = 0;
    for(i = 0; userInput[i] != 0 && userInput[i] != ' '; i++) {
        command[i] = userInput[i];
    }
    command[i] = 0;
    int argIdx = 0;

    for(int j = 0; j < MAX_ARGUMENTS; j++) {
            argv[j][0] = 0;
    }

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
 void printUser(){
    int len=strlen(userName);
    len+=4;
    printf("$ ");
    sendUserData(userName,len);
    printf(" > ");
}
void shellExecute(int argc2, char **argv2){
    char command[BUFFER_SIZE] = {0};
    char argv[MAX_ARGUMENTS][BUFFER_SIZE];
    char userInput[BUFFER_SIZE] = {0};
    int argc = 0;
    
    while (1){
        printUser();

        userInput[0] = 0;

        scanf("%s", userInput);
        
        argc = getCommandArgs(userInput, command, argv);

        if(argc == -1) {
            printf("\nIngreso argumentos de mas.\nLa maxima cantidad de argumentos permitida es: %d.\n\n", MAX_ARGUMENTS);
        }
        else if(strcmp("/mem", command) == 0) {
            testMemory(argc,argv);
        }
        else if(strcmp("/proc", command) == 0) {
            testProcesses(argc,argv);
        }
        else if(strcmp("/prio", command) == 0) {
            testPriority(argc,argv);
        }
        else if(strcmp("/sem", command) == 0) {
            testSync(argc,argv);
        }
        else if(strcmp("/nosem", command) == 0) {
            testNoSync(argc,argv);
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
        else if(strcmp("/user", command) == 0) {
            changeUser(argc, argv, userName);
        }
        else if(strcmp("/cpuvendor", command) == 0) {
            getCPUVendor(argc, argv);
        }
        else if(strcmp("/roots", command) == 0) {
            getRoots(argc, argv);
        }
        else if(strcmp("/dog", command) == 0) {
            logo();
        }
        else if(strcmp("/bgcolour", command) == 0){
            changeColour(argc, argv, 0);
        }
        else if(strcmp("/ftcolour", command) == 0){
            changeColour(argc, argv, 1);
        }
        else {
            printf("\nComando invalido: use /help\n\n");
        }
    }
    return ;
}
