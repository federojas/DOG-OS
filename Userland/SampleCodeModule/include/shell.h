#ifndef SHELL_H
#define SHELL_H

#define USER_SIZE 25
#define BUFFER_SIZE 100
#define MAX_ARGUMENTS 3

void shellWelcomeMessage();
void startShell(int argc, char **argv);
void shellExecute(int argc, char **argv2);
void printUser();
#endif