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

        scanf("%s %s",command,parameters);
        int x = strcmp("help", command);
        printf("%d", x);
        // if(strcmp("help", command) == 0) {
        //     help(0,0);
        // }
    }
    return;
}
