// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/* sampleCodeModule.c */
#include <shell.h>
#include <userSyscalls.h>
#include <libc.h>

int main(){
	char *argv[] = {"Shell"};
    newProcess(startShell, 1, argv, FOREGROUND, NULL);
	return 0;
}
