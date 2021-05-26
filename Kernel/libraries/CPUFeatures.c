#include <naiveConsole.h>
#include <stdint.h>
#include <CPUFeatures.h>

void getCPUFeatures(){

    int check = _checkCPUID();
	if(check) {
		ncPrint("This processor has CPUID features.");
	} else {
		ncPrint("This processor does not support CPUID.");
	}
    ncNewline();

	uint32_t buffer[4];
    // El arreglo buffer tendra los arreglos ecx y edx cuando corresponda con eax =1 
    // Consecuentes, estaran ebx y ecx cuando corresponda con eax = 7
    //con eax = 1: buffer[0] = ecx buffer[1] = edx
    //con eax = 7: buffer[2] = ebx buffer[3] = ecx

    

	_checkCPUFeatures(buffer); 
    
    ncPrint("CPU Features:");
    ncNewline();
    
    print_feature(buffer[0] &  (1<<23), "MMX (Multi-Media Extension)");
    print_feature(buffer[0] &  (1<<25), "SSE (Streaming SIMD Extension 1)");
    print_feature(buffer[0] &  (1<<26), "SSE2 (Streaming SIMD Extension 2)");
    print_feature(buffer[0] &  (1<<12), "FMA3 (Fused Multiply-Add 3-operand Form)");
    print_feature(buffer[1] &  (1<< 0), "SSE3 (Streaming SMD Extension 3)");
    print_feature(buffer[1] &  (1<<19), "SSE41 (Streaming SIMD Extensions 4.1)");
    print_feature(buffer[1] &  (1<<20), "SSE42 (Streaming SIMD Extensions 4.2)");
    print_feature(buffer[1] &  (1<<25), "AES (Advanced Encryption Standard)");
    print_feature(buffer[1] &  (1<<1), "VPCLMULQDQ (Carry-less Multiplication)");
    print_feature(buffer[1] &  (1<<28), "AVX (Advanced Vector Extensions)");
    print_feature(buffer[1] &  (1<< 29), "F16C (half-precision) FP feature");
    print_feature(buffer[1] &  (1<< 12), "Fused multiply-add (FMA3)");

    ncPrint("CPU Extended Features:");
    ncNewline();
    
    print_feature(buffer[2] &  (1<< 5), "AVX2 (Advanced Vector Extensions 2)");
    print_feature(buffer[3] &  (1<< 10), "VPCLMULQDQ (CLMUL instruction set (VEX-256/EVEX)");
    print_feature(buffer[3] &  (1<< 9), "VAES Vector AES instruction set (VEX-256/EVEX)");
}


void print_feature(int feature, const char * string){
    if(feature != 0) {
        ncPrint(string);
        ncPrint(": YES");
        ncNewline();
    }
    else {
        ncPrint(string);
        ncPrint(": NO");
        ncNewline();
    }
}