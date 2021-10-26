#ifndef SHELL_H
#define SHELL_H

#define USER_SIZE 25
#define BUFFER_SIZE 100
#define MAX_ARGUMENTS 5
#define COMMAND_COUNT 20

void startShell(int argc, char **argv);
void printUser();

typedef struct t_command {
    void (*commandFn)(int, char[MAX_ARGUMENTS][BUFFER_SIZE] );
    char * name;
    char * description;
} t_command;

typedef struct t_shell {
    char userName[USER_SIZE]; 
    t_command commands[COMMAND_COUNT];
} t_shell;
#endif