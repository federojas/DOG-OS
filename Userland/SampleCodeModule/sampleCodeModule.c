/* sampleCodeModule.c */
#include <shell.h>
#include <syscalls.h>
#include <libc.h>

int main(){
	char *argv[] = {"Shell"};
    newProcess(shellExecute, 1, argv, BACKGROUND, 0);
	return 0;
}
