#include <stdint.h>
#include <lib.h>
#include <moduleLoader.h>
#include <naiveConsole.h>
#include <idtLoader.h>
#include <keyboardDriver.h>
#include <videoDriver.h>
#include <memoryManager.h>
#include <processManager.h>

#define HEAP_MEMORY_SIZE 1024 * 1024 * 64 // 64MB

extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;

static const uint64_t PageSize = 0x1000;

static void * const sampleCodeModuleAddress = (void*)0x400000;
static void * const sampleDataModuleAddress = (void*)0x500000;
static void * const sampleCodeModuleHeapAddress = (void*)0x600000;

typedef int (*EntryPoint)();


void clearBSS(void * bssAddress, uint64_t bssSize) {
	memset(bssAddress, 0, bssSize);
}

void * getStackBase() {
	return (void*)(
		(uint64_t)&endOfKernel
		+ PageSize * 8				//The size of the stack itself, 32KiB
		- sizeof(uint64_t)			//Begin at the top of the stack
	);
}

void * initializeKernelBinary() {

	void * moduleAddresses[] = {
		sampleCodeModuleAddress,
		sampleDataModuleAddress
	};

	loadModules(&endOfKernelBinary, moduleAddresses);

	clearBSS(&bss, &endOfKernel - &bss);
	
	return getStackBase();
}


int main() {
	initializeMemoryManager((char *)sampleCodeModuleHeapAddress, HEAP_MEMORY_SIZE);
	initializeVideo();
	initializeProcessManager();
	char *argv2[] = {"Userland Init"};
    newProcess(sampleCodeModuleAddress, 1, argv2, FOREGROUND, 0);
	loadIdt();
    _hlt();
	printf("FATAL FAILURE");
	return 0;
}