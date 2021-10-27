#ifndef SHELL_H
#define SHELL_H

#define USER_SIZE 25
#define BUFFER_SIZE 100
#define MAX_ARGUMENTS 5
#define COMMAND_COUNT 26
#define TEST_COMMMAND_COUNT 5

#define C1_WIDTH 13
#define C2_WIDTH 38
#define HELP_MAIN 0
#define HELP_TEST 1

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
    t_command testCommands[TEST_COMMMAND_COUNT];
} t_shell;
#endif