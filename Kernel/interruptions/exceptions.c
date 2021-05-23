
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
	ncPrint("Exception 0: cannot divide by 0."); //VIDEO 
}

static void inv_op_code() {
	ncPrint("Exception 6: invalid op code."); //VIDEO 
}
