#include <prints.h>
#include <colors.h>
#include <infoReg.h>
#include <syscalls.h>

#define ZERO_EXCEPTION_ID 0
#define INVOP_EXCEPTION_ID 6

static void zeroDivision();
static void invOpCode();
static void printRegs(uint64_t* rsp);


void exceptionDispatcher(int exception, uint64_t rsp) {
	switch (exception)
	{
	case ZERO_EXCEPTION_ID:
		zeroDivision();
		break;
	case INVOP_EXCEPTION_ID:
		invOpCode();
		break;
	}
	printRegs((uint64_t *)rsp);
	return ;
}

static void zeroDivision() {
	printLine();
	sysWrite("Excecpion 0: No se puede dividir por cero.", 42, BLACK, RED,0);
	printLine();
	printLine();
}

static void invOpCode() {
	printLine();
	sysWrite("Excecpion 6: Opcode invalido.", 29, BLACK, RED,0);
	printLine();
	printLine();
}

static void printRegs(uint64_t* rsp) {
	for (int i = 0; i < REGISTER_AMOUNT; i++) {
		sysWrite(registerNames[i], strlen(registerNames[i]), BLACK, WHITE,0);
		printIntHex(rsp[i]);
		printLine();
	}
	printLine();
}
