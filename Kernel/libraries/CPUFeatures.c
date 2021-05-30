#include <stdint.h>
#include <CPUFeatures.h>

//AGREGAR AL INFORME https://gist.github.com/macton/4dd5fec2113be284796e

void getCPUFeatures(uint32_t buffer[4]){

    // El arreglo buffer tendra los arreglos ecx y edx cuando corresponda con eax =1 
    // Consecuentes, estaran ebx y ecx cuando corresponda con eax = 7
    //con eax = 1: buffer[0] = edx buffer[1] = ecx
    //con eax = 7: buffer[2] = ebx buffer[3] = ecx

	_checkCPUFeatures(buffer); 
    
}


