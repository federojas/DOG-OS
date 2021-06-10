#include <prints.h>
#include <colors.h>
#include <infoReg.h>
#include <syscalls.h>

#define ZERO_EXCEPTION_ID 0
#define INVOP_EXCEPTION_ID 6

static void zero_division();
static void inv_op_code();
static void printRegs(uint64_t* rsp);


void exceptionDispatcher(int exception, uint64_t rsp) {
	switch (exception)
	{
	case ZERO_EXCEPTION_ID:
		zero_division();
		break;
	case INVOP_EXCEPTION_ID:
		inv_op_code();
		break;
	}
	printRegs((uint64_t *)rsp);
	return ;
}

static void zero_division() {
	printLine();
	sys_write("Excecpion 0: No se puede dividir por cero.", 42, BLACK, RED,0);
	printLine();
	printLine();
}

static void inv_op_code() {
	printLine();
	sys_write("Excecpion 6: Opcode invalido.", 29, BLACK, RED,0);
	printLine();
	printLine();
}

static void printRegs(uint64_t* rsp) {
	for (int i = 0; i < REGISTER_AMOUNT; i++) {
		sys_write(registerNames[i], strlen(registerNames[i]), BLACK, WHITE,0);
		printIntHex(rsp[i]);
		printLine();
	}
	printLine();
}
