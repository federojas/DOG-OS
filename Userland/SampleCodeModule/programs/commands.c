#include <commands.h>
#include <stdint.h>
#include <libc.h>
#include <syscalls.h>
#include <RTCID.h>
#include <infoReg.h>

#define YEAR 20 //Debe escribir aca los digitos de su año (excepto los ultimos dos)
#define BYTES 32 //Cantidad de bytes para el mem dump
#define FLOAT_PRECISION 8 //Precision para los puntos flotantes
//#define LAST_MEM_POSITION 536870911 //512MB mem que se le pasa en run.sh

//returns current date and time
void getCurrentDayTime(int argc, char argv[MAX_ARGUMENTS][BUFFER_SIZE]) {
	if (argc != 0) {
		printf("\nCantidad invalida de argumentos.\n\n");
		return;
	}
    printf("\nFecha de hoy: ");
    uint64_t result = _syscall(SYS_RTC_ID, DAY_RTC_ID, 0, 0, 0, 0);
    printf("%d", result);
	putChar('/');

	result = _syscall(SYS_RTC_ID, MONTH_RTC_ID, 0, 0, 0, 0);
	printf("%d", result);
	putChar('/');

	result = _syscall(SYS_RTC_ID, YEAR_RTC_ID, 0, 0, 0, 0);
	printf("%d", YEAR);
	printf("%d\n", result);

	printf("\nHora: ");
	result = _syscall(SYS_RTC_ID, HOUR_RTC_ID, 0, 0, 0, 0);
	printf("%d", result);
	putChar(':');

	result = _syscall(SYS_RTC_ID, MINUTE_RTC_ID, 0, 0, 0, 0);
	if(result < 10){
		putChar('0'); 
	}
	printf("%d", result);
	putChar(':'); 

	result = _syscall(SYS_RTC_ID, SECOND_RTC_ID, 0, 0, 0, 0);
	if(result < 10){
		putChar('0'); 
	}
	printf("%d\n\n", result); 	
}

static void print_feature(uint8_t feature, const char * string){
    printf("%s", string);
	if(feature != 0) {
        printf(": SI\n");
    }
    else {
        printf(": NO\n");
    }
}

void getCPUFeatures(int argc, char argv[MAX_ARGUMENTS][BUFFER_SIZE]){
	if (argc != 0) {
		printf("\nCantidad invalida de argumentos.\n\n");
		return;
	}
	uint8_t check = _syscall(SYS_CPUID_ID, 0, 0, 0, 0, 0);
	if(check) {
		printf("\nEste procesador soporta CPUID.\n");
	} else {
		printf("\nEste processor no soporta CPUID.\n");
		return ;
	}
	uint32_t features[4];
	(void)_syscall(SYS_CPUFEATURES_ID, (uint64_t) features, 0, 0, 0, 0);

	// printf("\nValor de EDX con EAX en 1: %x\n", features[0]);
	// printf("Valor de ECX con EAX en 1: %x\n", features[1]);
	// printf("Valor de EBX con EAX en 7 y ECX en 0: %x\n", features[2]);
	// printf("Valor de ECX con EAX en 7 y ECX en 0: %x\n", features[3]);

	printf("\nCaracteristicas del CPU:\n\n");

    print_feature(features[0] >> 23 & 1, "MMX (Multi-Media Extension)");
    print_feature(features[0] >> 25 & 1, "SSE (Streaming SIMD Extension 1)");
    print_feature(features[0] >> 26 & 1, "SSE2 (Streaming SIMD Extension 2)");
    print_feature(features[1] >> 0 & 1, "SSE3 (Streaming SMD Extension 3)");
    print_feature(features[1] >> 19 & 1, "SSE4.1 (Streaming SIMD Extensions 4.1)");
    print_feature(features[1] >> 20 & 1, "SSE4.2 (Streaming SIMD Extensions 4.2)");
    print_feature(features[1] >> 25 & 1, "AES (Advanced Encryption Standard)");
    print_feature(features[1] >> 1 & 1, "PCLMULQDQ (Carry-less Multiplication)");
    print_feature(features[1] >> 28 & 1, "AVX (Advanced Vector Extensions)");
    print_feature(features[1] >> 29 & 1, "F16C (half-precision) FP feature");
    print_feature(features[1] >> 12 & 1, "FMA3 (Fused Multiply-Add 3-operand Form)");

	
    printf("\nCaracteristicas extendidas del CPU:\n\n");
	

    print_feature((features[2] >> 5) & 1, "AVX2 (Advanced Vector Extensions 2)");
    print_feature((features[3] >> 10) & 1, "VPCLMULQDQ (CLMUL instruction set (VEX-256/EVEX)");
    print_feature((features[3] >> 9) & 1, "VAES (Vector AES instruction set (VEX-256/EVEX))");
	newLine();
}

void getInfoReg(int argc, char argv[MAX_ARGUMENTS][BUFFER_SIZE]) {
	if (argc != 0) {
		printf("\nCantidad invalida de argumentos.\n\n");
		return;
    }
	uint64_t* registers = (uint64_t*) _syscall(SYS_INFOREG_ID, 0, 0, 0, 0, 0);
	newLine();
	for (int i = 0; i < REGISTER_AMOUNT; i++) {
		printf("%s", registerNames[i]);
		printf("%x\n\n", registers[i]);
    }
	newLine();
}

void getMem(int argc, char argv[MAX_ARGUMENTS][BUFFER_SIZE]) {
	if (argc != 1) {
		printf("\nCantidad invalida de argumentos.\n\n");
		return;
    }
	uint64_t memDir = strToHex(argv[0]);
	if(memDir == -1 /* || memDir + 32 > LAST_MEM_POSITION */ ) {
		printf("\nEl argumento ingresado es invalido. Use /help.\n\n");
        return;
	}

	printf("\nDump de 32 bytes a partir de la direccion: %s\n\n", argv[0]);
	uint8_t buffer[BYTES];
	char print[10];
	(void)_syscall(SYS_PRINTMEM_ID, memDir, (uint64_t) buffer, BYTES, 0,0);
	for(int i = 0; i < BYTES; i++) {
		if(i == 16) {
			newLine();
		}
		intToStr(buffer[i], print, 16);
		printf("%s ", print);
	}   
	newLine();
	newLine();
}

void divZero(int argc, char argv[MAX_ARGUMENTS][BUFFER_SIZE]) {
	if (argc != 0) {
		printf("\nCantidad invalida de argumentos.\n\n");
		return;
    }
	int x = 3;
	int y = 0;
	x = x/y;
}

// https://mudongliang.github.io/x86/html/file_module_x86_id_318.html
void opCode(int argc, char argv[MAX_ARGUMENTS][BUFFER_SIZE]) {
	if (argc != 0) {
		printf("\nCantidad invalida de argumentos.\n\n");
		return;
    }
	_opcodeExp();
}

void getRoots(int argc, char argv[MAX_ARGUMENTS][BUFFER_SIZE]) {
	if (argc != 3) {
		printf("\nCantidad invalida de argumentos.\n\n");
		return;
    }

	double a;
	double b;
	double c;

	strToDouble(argv[0], &a);
	strToDouble(argv[1], &b);
	strToDouble(argv[2], &c);

	double min = 1.0;
	min /= pow(10, FLOAT_PRECISION);

	double aux1;
	double aux2;
	double aux3;
	
	aux1 = a > 0 ? a : -1*a;
	aux2 = b > 0 ? b : -1*b;
	aux3 = c > 0 ? c : -1*c;

	if (a == 0) {
		printf("\nEl coeficiente de x^2 no puede ser 0.\n\n");
		return;
	}

	if( aux1 < min || (aux2 < min && b!=0) || (aux3 < min && c!=0) ){
		printf("\nLos argumentos pueden tener como maximo %d decimales.\n\n", FLOAT_PRECISION);
		return ;
	}

	if(b*b - 4*a*c < 0){
		printf("\nEl polinomio no puede tener raices imaginarias.\nSu discriminante no puede ser menor que 0.\n\n");
		return ;
	}

	double root1;
	double root2;

	_quadratic(&a, &b, &c, &root1, &root2);

	char res1[BUFFER_SIZE];
	char res2[BUFFER_SIZE];

	doubleToStr(root1, res1, FLOAT_PRECISION);	
	doubleToStr(root2, res2, FLOAT_PRECISION);

	printf("\nLas raices son: %s y %s\n\n", res1, res2);
}

void clear(int argc, char argv[MAX_ARGUMENTS][BUFFER_SIZE]) {
	if (argc != 0) {
		printf("\nCantidad invalida de argumentos.\n\n");
		return;
    }
	(void)_syscall(SYS_CLEAR_ID,0,0,0,0,0);
}
void exit(int argc, char argv[MAX_ARGUMENTS][BUFFER_SIZE]) {
	if (argc != 0) {
		printf("\nCantidad invalida de argumentos.\n\n");
		return;
    }
	printf("\nMuchas gracias por utilizar DOG-OS, esperamos su regreso.\n");
	printf("\nUsted cerro el TP, el mismo ya no funcionara.\n\n");
	logo();
	(void)_syscall(SYS_EXIT_ID,0,0,0,0,0);
}

//https://patorjk.com/software/taag/#p=display&f=Slant&t=DOG-OS
void logo() {
	printf("              ____  ____  ______      ____  _____\n");
	printf("             / __ \\/ __ \\/ ____/     / __ \\/ ___/\n");
	printf("            / / / / / / / / ________/ / / /\\__ \\ \n");
	printf("           / /_/ / /_/ / /_/ /_____/ /_/ /___/ /\n");
	printf("          /_____/\\____/\\____/      \\____//____/\n\n");
}

void changeUser(int argc, char argv[MAX_ARGUMENTS][BUFFER_SIZE], char userName[USER_SIZE]) {
	if (argc != 1) {
		printf("\nCantidad invalida de argumentos.\n\n");
		return;
    }
	if(strlen(argv[0]) > USER_SIZE - 1) {
		printf("\nEl nombre de usuario puede tener un maximo de %d caracteres.\n\n", USER_SIZE - 1);
		return;
	}
	strcpy(userName, argv[0]);
	setFirstChange(1);
}

void getCPUVendor(int argc, char argv[MAX_ARGUMENTS][BUFFER_SIZE]) {
	if (argc != 0) {
		printf("\nCantidad invalida de argumentos.\n\n");
		return;
    }
	char buffer[9];
	(void)_syscall(SYS_CPUVENDOR_ID,(uint64_t) buffer,0,0,0,0);
	printf("\nID de fabricante: %s\n\n", buffer);
}

void changeColour(int argc, char argv[MAX_ARGUMENTS][BUFFER_SIZE], int ft) {
	if (argc != 1) {
		printf("\nCantidad invalida de argumentos.\n\n");
		return;
    }
	int aux = strToInt(argv[0], 0);
	if(ft == 1) {
		switch(aux){
			case 1:
				setFTC(WHITE);
				break;
			case 2:
				setFTC(BLACK);
				break;
			case 3:
				setFTC(RED);
				break;
			case 4:
				setFTC(GREEN);
				break;
			case 5:
				setFTC(BLUE);
				break;
			default:
				printf("\nEl codigo de color que ingreso es invalido. Use /help.\n\n");
				break;
		}
	} else {
		switch(aux){
			case 1:
				setBGC(WHITE);
				break;
			case 2:
				setBGC(BLACK);
				break;
			case 3:
				setBGC(RED);
				break;
			case 4:
				setBGC(GREEN);
				break;
			case 5:
				setBGC(BLUE);
				break;
			default:
				printf("\nEl codigo de color que ingreso es invalido. Use /help.\n\n");
				break;
		}
	}
}

void help(int argc, char argv[MAX_ARGUMENTS][BUFFER_SIZE]) {
	if (argc != 0) {
		printf("\nCantidad invalida de argumentos.\n\n");
		return;
    }
	printf("\nUse ctrl + tab para cambiar de pantalla.\n");	
	printf("\nTabla de colores: \n");
	printf("\nBLANCO | NEGRO | ROJO | VERDE | AZUL\n");
	printf("  1    |   2   |  3   |   4   |  5\n");
	printf("\nLista de comandos: \n");
	printf("\n/help : Listado de comandos\n");
	printf("\n/clear : Limpia la pantalla actual\n");
	printf("\n/user : Cambia el nombre de usuario.\nIngrese el nombre como un solo argumento.\n");
    printf("\n/inforeg : Estado de todos los resgitros.\nUse ctrl + r para capturar los mismos.\n");
    printf("\n/cpufeatures : Caracteristicas del CPU\n");
	printf("\n/date&time : Fecha y hora actual\n");
	printf("\n/printmem : Volcado de memoria de 32 bytes a partir de\ndireccion de memoria en hexa ingresada como argumento.\n");
	//printf("La direccion debe estar comprendida en el rango: 0 - %x\n", LAST_MEM_POSITION - 32); CHEQUEO MAXMEM
	printf("\n/divzero : Excepcion division por cero\n");
	printf("\n/opcode : Excepcion opcode invalido\n");
	printf("\n/cpuvendor : ID de fabricante del CPU\n");
	printf("\n/roots : Calculo de raices de una funcion cuadratica.\nIngrese los 3 valores de la misma como argumentos.\n");
	printf("\n/dog : Imprime DOG-OS logo\n");
	printf("\n/ftcolour : Cambia el color del texto.\nPase el color como argumento usando la tabla de colores.\n\n");
	printf("\n/bgcolour : Cambia el color del fondo del texto.\nPase el color como argumento usando la tabla de colores.\n\n");
	printf("\n/exit : Finaliza la ejecucion.\n\n");
	
}



