/* sampleCodeModule.c */
#include <shell.h>
#include <userSyscalls.h>
#include <libc.h>

int main(){
	char *argv[] = {"Shell"};
    newProcess(shellExecute, 1, argv, FOREGROUND, NULL);
	return 0;
}
