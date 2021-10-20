/* sampleCodeModule.c */
#include <shell.h>
#include <syscalls.h>
#include <libc.h>

int main(){
	char *argv[] = {"Shell"};
    _syscall(SYS_CREATE_PROCESS_ID, (uint64_t) startShell, 1, (uint64_t) argv, 1, 0);
	return 0;
}
