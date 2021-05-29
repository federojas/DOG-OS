#include <prints.h>
#include <colors.h>
#define ZERO_EXCEPTION_ID 0
#define INVOP_EXCEPTION_ID 6

static void zero_division();
static void inv_op_code();

void exceptionDispatcher(int exception) {
	switch (exception)
	{
	case ZERO_EXCEPTION_ID:
		zero_division();
		break;
	case INVOP_EXCEPTION_ID:
		inv_op_code();
		break;
	}
}

static void zero_division() {
	printStringColor("Exception 0: Cannot divide by zero.", BLACK, RED);
}

static void inv_op_code() {
	printStringColor("Exception 6: Invalid opcode.", BLACK, RED);
}
