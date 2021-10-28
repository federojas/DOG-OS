#ifndef SHELL_H
#define SHELL_H

#define USER_SIZE 25
#define BUFFER_SIZE 1000
#define MAX_ARGUMENTS 5
#define COMMAND_COUNT 33
#define TEST_COMMAND_COUNT 5
#define TEST_COMMAND_START (COMMAND_COUNT - TEST_COMMAND_COUNT)

#define C1_WIDTH 13
#define C2_WIDTH 38

void startShell(int argc, char **argv);
void printUser();

typedef struct t_command {
    void (*commandFn)(int, char ** );
    char * name;
    char * description;
} t_command;

typedef struct t_shell {
    char userName[USER_SIZE]; 
    t_command commands[COMMAND_COUNT];
} t_shell;
#endif