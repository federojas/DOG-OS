#include <shell.h>
#include <libc.h>
#include <commands.h>
#include <stdint.h>
#include <memoryTest.h>
#include <processTest.h>
#include <priorityTest.h>
#include <semaphoreTest.h>
#include <userSyscalls.h>

static int getCommandArgs(char* userInput, char * argv[MAX_ARGUMENTS]);
static void shellWelcomeMessage();
static void shellExecute();
static int getCommandIdx(char * command);
static void initializeShell();
static void initializeCommands();
static void changeUser(int argc, char ** argv);
static void help(int argc, char ** argv);

static t_command commands[COMMAND_COUNT] = {
    {&help, "/help", "Listado de comandos"},
    {&clear, "/clear", "Limpia la pantalla actual"},
    {&changeUser, "/user", "Cambia el nombre de usuario, ingrese el nombre como un solo argumento"},
    {&getInfoReg, "/inforeg", "Estado de todos los resgitros, use ctrl + r para capturar los mismos"},
    {&getCPUFeatures, "/cpufeatures", "Caracteristicas del CPU"},
    {&exit, "/exit", "Finaliza la ejecucion"},
    {&opCode, "/opcode", "Excepcion opcode invalido"},
    {&getCurrentDayTime, "/date&time", "Fecha y hora actual"},
    {&getMem, "/printmem", "Volcado de memoria de 32 bytes a partir de\ndireccion de memoria en hexa ingresada como argumento."},
    {&divZero, "/divzero", "Excepcion division por cero"},
    {&getCPUVendor, "/cpuvendor", "ID de fabricante del CPU"},
    {&getRoots, "/roots", "Calculo de raices de una funcion cuadratica"},
    {&logo, "/dog", "Imprime DOG-OS logo"},
    {&changeBgColour, "/bgcolour", "Cambia el color del fondo del texto"},
    {&changeFtColour, "/ftcolour", "Cambia el color del texto"},  
    {&testMemoryWrapper, "/memtest", "Testeo de memory manager"},
    {&testProcessesWrapper, "/proctest", "Testeo de process manager"},
    {&testPriorityWrapper, "/priotest", "Testeo de prioridad de process manager"},
    {&testSyncWrapper, "/semtest", "Testeo de semaforos con uso"},
    {&testNoSyncWrapper, "/nosemtest", "Testeo de semaforos sin uso"},
    {&memStatusWrapper, "/mem", "Imprime el estado de la memoria"},
    {&semStatusWrapper, "/sem", "Imprime el estado de los semaforos"},
    {&killProcessWrapper, "/kill", "Imprime el estado de la memoria"},
    {&processStatusWrapper, "/ps", "Imprime el estado de los procesos"},
    {&setPriorityWrapper, "/nice", "Cambia la prioridad de un proceso"},
    {&blockProcessWrapper, "/block", "Cambia el estado de un proceso a bloqueado"},
    {&unblockProcessWrapper, "/unblock", "Cambia el estado de un proceso a listo"},
    {&cat, "/cat", "Imprime el texto ingresado por la shell"},
    {&loop, "/loop", "Imprime un saludo cada 3 segundos"},
};

static t_shell shellData;

void startShell(int argc, char **argv){ 
    shellWelcomeMessage();
    initializeShell();
    shellExecute();
}

void printUser() {
    int len=strlen(shellData.userName);
    len+=4;
    printf("$ ");
    sendUserData(shellData.userName, len);
    printf(" > ");
}

static void initializeShell() {
    strcpy(shellData.userName, "DefaultUser");
    initializeCommands();
}

static void initializeCommands() {
    for(int i = 0; i < COMMAND_COUNT; i++) {
        shellData.commands[i].commandFn = commands[i].commandFn;
        shellData.commands[i].name = commands[i].name;
        shellData.commands[i].description = commands[i].description;
    }
}

static void shellExecute() {
    char userInput[BUFFER_SIZE] = {0};
    int argc = 0;
    
    while (1) {
        printUser();

        argc = 0;
        userInput[0] = 0;
        char * argv [MAX_ARGUMENTS] = {0};

        scanf("%s", userInput);
        
        argc = getCommandArgs(userInput, argv);
        
        if(argc == -1) {
            printf("\nIngreso argumentos de mas.\nLa maxima cantidad de argumentos permitida es: %d.\n\n", MAX_ARGUMENTS);
        } 
        int commandIdx = getCommandIdx(argv[0]);
        
        if(commandIdx >= 0) {
            newProcess((void (*)(int, char**))shellData.commands[commandIdx].commandFn, argc, (char **)argv, FOREGROUND, NULL);     
        } else {
            printf("\nComando invalido: use /help\n\n");
        }
    }
    return ;
}

static int getCommandIdx(char * command) {
    for(int i = 0; i < COMMAND_COUNT; i++) {
        if ((strcmp(shellData.commands[i].name, command)) == 0) {
            return i;
        }
    }
    return -1;
}

static int getCommandArgs(char* userInput, char ** argv) {
    int argc = 0;

    if (*userInput != ' ' && *userInput != '\0') {
        argv[argc++] = userInput;
    }
            
  
    while(*userInput != 0) {
        if(*userInput == ' ') {
            *userInput = 0;
            if( (*(userInput + 1) != ' ') &&  (*(userInput + 1) != 0)) {
                if(argc >= MAX_ARGUMENTS) {
                    return -1;
                }
                argv[argc++] = userInput + 1;
            }
        }
        userInput++;
    }
    return argc;
}

static void shellWelcomeMessage() {
    printf("\n                 Le damos la bienvenida a\n\n");
    logo(1,0);
    printf("\n         Sistemas Operativos --- 2C 2021\n\n");
    printf("\n  Utilice el comando /help para obtener un manual de usuario.\n\n\n\n");
}

static void changeUser(int argc, char ** argv) {
	if (argc != 2) {
		printf("\nCantidad invalida de argumentos.\n\n");
		return;
    }
	if(strlen(argv[0]) > USER_SIZE - 1) {
		printf("\nEl nombre de usuario puede tener un maximo de %d caracteres.\n\n", USER_SIZE - 1);
		return;
	}
	strcpy(shellData.userName, argv[0]);
	setFirstChange(1);
}

static void help(int argc, char ** argv) {
	if (argc != 1) {
		printf("\nCantidad invalida de argumentos.\n\n");
		return;
    }
	printf("\nUse ctrl + tab para cambiar de pantalla.\n");	
	printf("\nTabla de colores: \n");
	printf("\nBLANCO | NEGRO | ROJO | VERDE | AZUL\n");
	printf("  1    |   2   |  3   |   4   |  5\n");
	printf("\nLista de comandos: \n");
	for(int i = 0; i < COMMAND_COUNT; i++) {
        printf("%s : %s\n", shellData.commands[i].name, shellData.commands[i].description);
    }

    //DAGOS EL TP PIDE UN APARTADO PARA LOS TESTS (PARA QUE SEAN MAS FACILES DE ENCONTRAR)

}
