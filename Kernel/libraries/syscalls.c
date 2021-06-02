#include <syscalls.h>
#include <keyboardDriver.h>
#include <videoDriver.h>

// https://wiki.osdev.org/CMOS#Format_of_Bytes
uint8_t getCurrentTime(uint64_t rtcID){
	uint8_t x = _getRTCInfo(rtcID);
	uint8_t result = ((x / 16) * 10) + (x & 0xf);
	return result;
}

//AGREGAR AL INFORME https://gist.github.com/macton/4dd5fec2113be284796e

void getCPUFeatures(uint32_t buffer[4]){

    // El arreglo buffer tendra los arreglos ecx y edx cuando corresponda con eax =1 
    // Consecuentes, estaran ebx y ecx cuando corresponda con eax = 7
    //con eax = 1: buffer[0] = edx buffer[1] = ecx
    //con eax = 7: buffer[2] = ebx buffer[3] = ecx

	_checkCPUFeatures(buffer); 
}

void getMem(uint64_t direc, uint8_t * buffer, uint64_t bytes) {
	for (uint8_t i = 0; i < bytes; i++) {
        buffer[i] = (uint8_t) _getMem(direc+i);
    }
}


void sys_write(char * str, uint8_t len, t_color bgColor, t_color ftColor) {
    if (str == 0 ||  len <= 0 || bgColor < 0 || ftColor < 0) 
        return ;
    
    for (int i = 0; str[i] != 0 && i < len; i++) 
        printChar(str[i], ftColor, bgColor, 1);     
}

uint64_t sys_read(uint64_t buffer, uint64_t len){
	if(len==0){
		return -1;
	}

	//OPCION CON ESPERA ACTIVA
		char *buff=(char* ) buffer;//casteo para evitar el warning
	return dumpBuffer(buff,len);
	
	
	//OPCION CON HLT (ESTA ANDANDO LENTO)
	// char *buff=(char *) buffer;
	// int i=0;
	// while(i<len){
	// 	buff[i++]=getChar();
	// }
	// buff[i]=0;
	// return i;
}