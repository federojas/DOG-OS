#include <commands.h>
#include <stdint.h>
#include <libc.h>
#include <syscalls.h>
#include <RTCID.h>
#include <infoReg.h>

#define YEAR 20 //Debe escribir aca los digitos de su año (excepto los ultimos dos)
#define BYTES 32 //Cantidad de bytes para el mem dump

extern void _opcodeExp(void);

//returns current date and time
void getCurrentDayTime(int argc, char** args) {
	if (argc != 0) {
		printf("Cantidad invalida de argumentos.\n");
		return;
	}
    printf("Fecha de hoy: ");
    uint64_t result = _syscall(SYS_RTC_ID, DAY_RTC_ID, 0, 0, 0, 0);
    printf("%d", result);
	putChar('/');

	result = _syscall(SYS_RTC_ID, MONTH_RTC_ID, 0, 0, 0, 0);
	printf("%d", result);
	putChar('/');

	result = _syscall(SYS_RTC_ID, YEAR_RTC_ID, 0, 0, 0, 0);
	printf("%d", YEAR);
	printf("%d\n", result);

	printf("Hora: ");
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
	printf("%d\n", result); 	
}

static void print_feature(int feature, const char * string){
    if(feature != 0) {
        printf("%s", string);
        printf(": SI\n");
    }
    else {
        printf("%s", string);
        printf(": NO\n");
    }
}

void getCPUFeatures(int argc, char** args){
	if (argc != 0) {
		printf("Cantidad invalida de argumentos.\n");
		return;
	}
	uint8_t check = _syscall(SYS_CPUID_ID, 0, 0, 0, 0, 0);
	if(check) {
		printf("Este procesador soporta CPUID.\n");
	} else {
		printf("Este processor no soporta CPUID.\n");
		return ;
	}
	uint32_t features[4];
	_syscall(SYS_CPUFEATURES_ID, (uint64_t) features, 0, 0, 0, 0);

	newLine();
	printf("Caracteristicas del CPU:\n");
	newLine();

    print_feature(features[0] &  (1<<23), "MMX (Multi-Media Extension)");
    print_feature(features[0] &  (1<<25), "SSE (Streaming SIMD Extension 1)");
    print_feature(features[0] &  (1<<26), "SSE2 (Streaming SIMD Extension 2)");
    print_feature(features[1] &  (1<<0), "SSE3 (Streaming SMD Extension 3)");
    print_feature(features[1] &  (1<<19), "SSE41 (Streaming SIMD Extensions 4.1)");
    print_feature(features[1] &  (1<<20), "SSE42 (Streaming SIMD Extensions 4.2)");
    print_feature(features[1] &  (1<<25), "AES (Advanced Encryption Standard)");
    print_feature(features[1] &  (1<<1), "PCLMULQDQ (Carry-less Multiplication)");
    print_feature(features[1] &  (1<<28), "AVX (Advanced Vector Extensions)");
    print_feature(features[1] &  (1<<29), "F16C (half-precision) FP feature");
    print_feature(features[1] &  (1<<12), "FMA3 (Fused Multiply-Add 3-operand Form)");

	newLine();
    printf("Caracteristicas extendidas del CPU:\n");
	newLine();

    print_feature(features[2] &  (1<<5), "AVX2 (Advanced Vector Extensions 2)");
    print_feature(features[3] &  (1<<10), "VPCLMULQDQ (CLMUL instruction set (VEX-256/EVEX)");
    print_feature(features[3] &  (1<<9), "VAES (Vector AES instruction set (VEX-256/EVEX))");
}

void getInfoReg(int argc, char** args) {
	if (argc != 0) {
		printf("Cantidad invalida de argumentos.\n");
		return;
      }
	uint64_t* registers = (uint64_t*)_syscall(SYS_INFOREG_ID, 0, 0, 0, 0, 0);
	for (int i = 0; i < REGISTER_AMOUNT; i++) {
		printf("%s", registerNames[i]);
		printf("%x\n", registers[i]);
      }
	  newLine();
}

void getMem(int argc, char** args, char* dirHexa) {
	if (argc != 1) {
		printf("Cantidad invalida de argumentos.\n");
		return;
    }
	uint64_t memDir = strToHex(dirHexa);
	if(memDir == -1) {
		printf("El argumento ingresado es invalido. Use /help.\n");
        return;
	}
	printf("Dump de 32 bytes a partir de la direccion: %s\n\n", dirHexa);
	uint8_t buffer[BYTES];
	char print[10];
	_syscall(SYS_PRINTMEM_ID, memDir, (uint64_t) buffer, BYTES, 0, 0);
	for(int i = 0; i < BYTES; i++) {
		intToStr(buffer[i], print, 16);
		printf("%s ", print);
	}   
}

void divZero(int argc, char** args) {
	if (argc != 0) {
		printf("Cantidad invalida de argumentos.\n");
		return;
      }
	int x = 3/0;
}

// https://mudongliang.github.io/x86/html/file_module_x86_id_318.html
void opCode(int argc, char** args) {
	if (argc != 0) {
		printf("Cantidad invalida de argumentos.\n");
		return;
      }
	_opcodeExp();
}

// void getRoots(int argc, char** args, float a, float b, float c) {
// 	if (argc != 3) {
// 		printf("Cantidad invalida de argumentos.\n");
// 		return;
//     }
// 	if (a == 0) {
// 		printf("El coeficiente de x^2 no puede ser 0.\n");
// 		return;
//     }
// 	printf("Atencion, la funcion tiene una precision de 4 decimales.\n");
// 	float results[2];
// 	// basado en: https://www.programiz.com/c-programming/examples/quadratic-roots

//     float disc = b * b - 4 * a * c;
// 	if(disc < 0)
// 		disc *= -1;
// 	float x = disc;
  
//     float sqrtdisc;

  
//     for(int i = 0; i < 10 ; i++) {
    
//         // Calculate more closed x
//         sqrtdisc = 0.5 * (x + (disc / x));
  
//         // Update root
//         x = sqrtdisc;
//     }

	
// 	// condition for real and different roots
//     if (disc > 0) {
//         results[0] = (-b + sqrtdisc) / (2 * a);
//         results[1] = (-b - sqrtdisc) / (2 * a);
//     }
//     // condition for real and equal roots
//     else if (disc == 0) {
//         results[0] = results[1] = -b / (2 * a);
//     }
	

//     // if roots are not real
//     else {
//         float realPart = -b / (2 * a);
//         float imagPart = sqrtdisc / (2 * a);
// 		results[0] = realPart;
// 		results[1] = imagPart;
//     }
// 	printf("\nsqrtdisc: %d\n", sqrtdisc);
// 	printf("%d y %d\n", results[0], results[1]);
// 	// if( disc < 0) {
// 	// 	printf("Las raices son: i) %f + %f i ii) %f - %f i\n", results[0], results[1], results[0], results[1]);
// 	// 	return;
// 	// } 
// 	// printf("Las raices son: i) %f ii) %f\n", results[0], results[1]);	
// }

void help(int argc, char** args) {
	printf("Lista de comandos: \n");
	printf("/help : Listado de comandos\n");
    printf("/inforeg : Estado de todos los resgitros\n");
    printf("/cpufeatures : Caracteristicas del CPU\n");
	printf("/date&time : Fecha y hora actual\n");
	printf("/printmem : Volcado de memoria de 32 bytes a partir de\n direccion de memoria en hexa ingresada como argumento.\n");
	printf("/divzero : Excepcion division por cero\n");
	printf("/opcode : Excepcion opcode invalido\n");
	printf("/roots : Calculo de raices de una funcion cuadratica.\n Ingrese los 3 valores de la misma como argumentos.\n");
}

