include ../Makefile.inc

MODULE=0000-sampleCodeModule.bin

SOURCES=$(wildcard [^_]*.c)
SOURCES_ASM=$(wildcard asm/*.asm)
SOURCES_LIBRARIES=$(wildcard libraries/*.c)
SOURCES_PROGRAMS=$(wildcard programs/*.c)
SOURCES_TEST_PROCESS=$(wildcard processTest/*.c)
SOURCES_TEST_SEMAPHORE=$(wildcard semaphoreTest/*.c)
SOURCES_TEST_MEMORY=$(wildcard memoryTest/*.c)


OBJECTS=$(SOURCES:.c=.o)
OBJECTS_PROGRAMS=$(SOURCES_PROGRAMS:.c=.o)
OBJECTS_ASM=$(SOURCES_ASM:.asm=.o)
OBJECTS_LIBRARIES=$(SOURCES_LIBRARIES:.c=.o)
OBJECTS_TEST_PROCESS=$(SOURCES_TEST_PROCESS:.c=.o)
OBJECTS_TEST_SEMAPHORE=$(SOURCES_TEST_SEMAPHORE:.c=.o)
OBJECTS_TEST_MEMORY=$(SOURCES_TEST_MEMORY:.c=.o)

all: $(MODULE)

$(MODULE): $(OBJECTS) $(OBJECTS_ASM) $(OBJECTS_LIBRARIES) $(OBJECTS_PROGRAMS) $(OBJECTS_TEST_PROCESS) $(OBJECTS_TEST_SEMAPHORE) $(OBJECTS_TEST_MEMORY)
	$(LD) $(LDFLAGS) -T sampleCodeModule.ld -o ../$(MODULE) $^

%.o: %.c
	$(GCC) $(GCCFLAGS) -I./include -c $< -o $@

%.o : %.asm
	$(ASM) $(ASMFLAGS) $< -o $@

clean:
	rm -rf *.o programs/*.o asm/*.o libraries/*.o  processTest/*.o syncTest/*.o memoryTest/*.o

.PHONY: all clean print




