#include <commands.h>
#include <stdint.h>
#include <libc.h>
#include <syscalls.h>
#include <RTCID.h>
#include <infoReg.h>

#define YEAR 20 //Debe escribir aca los digitos de su año (excepto los ultimos dos)

//returns current date and time
void getCurrentDayTime(int argc, char** args) {
      if (argc != 0) {
            printf("Cantidad invalida de argumentos.\n");
            return;
      }
    printf("Fecha de hoy: ");
    uint8_t result = _syscall(SYS_RTC_ID, DAY_RTC_ID, 0, 0, 0, 0);
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
	uint64_t check = _syscall(SYS_CPUID_ID,0 , 0, 0, 0, 0);
	if(check) {
		printf("Este procesador \\ soporta CPUID.\n");
	} else {
		printf("Este processor no soporta CPUID.\n");
		return ;
	}
	uint32_t features[4];
	_syscall(SYS_CPUFEATURES_ID, features, 0, 0, 0, 0);

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